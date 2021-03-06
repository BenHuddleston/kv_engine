/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
 *     Copyright 2019-Present Couchbase, Inc.
 *
 *   Use of this software is governed by the Business Source License included
 *   in the file licenses/BSL-Couchbase.txt.  As of the Change Date specified
 *   in that file, in accordance with the Business Source License, use of this
 *   software will be governed by the Apache License, Version 2.0, included in
 *   the file licenses/APL2.txt.
 */

#pragma once

#include <boost/dynamic_bitset.hpp>
#include <memcached/engine_common.h>
#include <memcached/vbucket.h>

#include <mutex>
#include <queue>
#include <unordered_set>

/**
 * VBReadyQueue is a std::queue wrapper for managing a queue of vbuckets that
 * are ready for some task to process. The queue does not allow duplicates and
 * the push_unique method enforces this.
 *
 * Internally a std::queue and std::set track the contents and the std::set
 * enables a fast exists method which is used by front-end threads.
 */
class VBReadyQueue {
public:
    /// Construct a VBReadyQueue with the specified maximum number of vBuckets.
    VBReadyQueue(size_t maxVBuckets);

    bool exists(Vbid vbucket);

    /**
     * Return true and set the ref-param 'frontValue' if the queue is not
     * empty. frontValue is set to the front of the queue.
     */
    bool popFront(Vbid& frontValue);

    /**
     * Pop the front item.
     * Safe to call on an empty list
     */
    void pop();

    /**
     * Push the vbucket only if it's not already in the queue.
     * @return true if the queue was previously empty (i.e. we have
     * transitioned from zero -> one elements in the queue).
     */
    bool pushUnique(Vbid vbucket);

    /**
     * Size of the queue.
     */
    size_t size() const;

    /**
     * @return true if empty
     */
    bool empty();

    /**
     * Clears the queue
     */
    void clear();

    /**
     * Pop the contents of the queue into another and return that
     */
    std::queue<Vbid> swap();

    void addStats(const std::string& prefix,
                  const AddStatFn& add_stat,
                  const void* c) const;

private:
    // Mutable so that we can lock in addStats (const) to copy the queue/set
    mutable std::mutex lock;

    /* a queue of vbuckets that are ready for producing */
    std::queue<Vbid> readyQueue;

    /**
     * maintain a set of values that are in the readyQueue.
     * find() is performed by front-end threads so we want it to be
     * efficient so just a set lookup is required.
     */
    boost::dynamic_bitset<> queuedValues;
};
