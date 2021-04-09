/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
 *     Copyright 2020-Present Couchbase, Inc.
 *
 *   Use of this software is governed by the Business Source License included
 *   in the file licenses/BSL-Couchbase.txt.  As of the Change Date specified
 *   in that file, in accordance with the Business Source License, use of this
 *   software will be governed by the Apache License, Version 2.0, included in
 *   the file licenses/APL2.txt.
 */

#pragma once

#include <memory>

/**
 * StoredDocKey using declaration provides a StoredDocKeyT with std::allocator
 * which is suitable for most purposes where additional allocation tracking is
 * not required. StoredDocKey allocations are /still/ counted towards overall
 * mem_used stats.
 */
template <template <class> class T>
class StoredDocKeyT;
using StoredDocKey = StoredDocKeyT<std::allocator>;
