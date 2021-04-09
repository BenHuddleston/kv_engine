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

#include "collections/flush.h"
#include "vbucket_state.h"

#include "libcouchstore/couch_db.h"

namespace Collections::VB {
class Manifest;
} // namespace Collections::VB

namespace VB {

/**
 * The VB::Commit class encapsulates data to be passed to KVStore::commit
 * and is then used by KVStore for the update of on-disk data.
 */
class Commit {
public:
    /**
     * Create the commit object to reference the given manifest and carry the
     * given vbucket_state. The  vbucket_state use a default parameter for test
     * code which just wants to all kvstore::commit
     */
    explicit Commit(Collections::VB::Manifest& manifest,
                    vbucket_state vbs = {},
                    SysErrorCallback sysErrorCallback = {});

    /// Object for updating the collection's meta-data during commit
    Collections::VB::Flush collections;

    /**
     * state to be used by the commit if successful, written to the KVStore
     * in-memory copy.
     * The commit itself may make changes to this object which will be
     * part of the in-memory update.
     */
    vbucket_state proposedVBState;

    /**
     * Executed at KVStore::commit if operation fails.
     * Allows EP to take decisions on how to react to the failure.
     */
    SysErrorCallback sysErrorCallback;
};

} // end namespace VB
