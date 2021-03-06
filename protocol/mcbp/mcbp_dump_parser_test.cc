/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
 *     Copyright 2017-Present Couchbase, Inc.
 *
 *   Use of this software is governed by the Business Source License included
 *   in the file licenses/BSL-Couchbase.txt.  As of the Change Date specified
 *   in that file, in accordance with the Business Source License, use of this
 *   software will be governed by the Apache License, Version 2.0, included in
 *   the file licenses/APL2.txt.
 */
#include <folly/portability/GTest.h>
#include <mcbp/mcbp.h>
#include <platform/dirutils.h>
TEST(ParserTest, GdbOutput) {
    const auto file =
            cb::io::sanitizePath(SOURCE_ROOT "/protocol/mcbp/gdb_output.txt");
    const auto content = cb::io::loadFile(file);
    cb::byte_buffer buf = {
            const_cast<uint8_t*>(
                    reinterpret_cast<const uint8_t*>(content.data())),
            content.size()};
    auto data = cb::mcbp::gdb::parseDump(buf);

    const std::vector<uint8_t> bytes = {
            {0x81, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x55, 0x53, 0x5f, 0x56, 0x32,
             0x5f, 0x52, 0x45, 0x43, 0x55, 0x52, 0x52, 0x49, 0x4e, 0x47, 0x5f,
             0x54, 0x49, 0x45, 0x52, 0x5f, 0x43, 0x41, 0x52, 0x45, 0x45, 0x52,
             0x5f, 0x53, 0x54, 0x41, 0x52, 0x53, 0x5f, 0x55, 0x5f, 0x34, 0x30,
             0x37, 0x39, 0x37, 0x35, 0x33, 0x61, 0x3a, 0x31, 0x3a, 0x7b, 0x73,
             0x3a, 0x31, 0x31, 0x3a, 0x22, 0x57, 0x49, 0x4e, 0x54, 0x45, 0x52,
             0x5f, 0x32, 0x30, 0x31, 0x33, 0x22, 0x3b, 0x64}};

    EXPECT_EQ(bytes, data);
}

TEST(ParserTest, LldbOutput) {
    const auto file =
            cb::io::sanitizePath(SOURCE_ROOT "/protocol/mcbp/lldb_output.txt");
    const auto content = cb::io::loadFile(file);
    cb::byte_buffer buf = {
            const_cast<uint8_t*>(
                    reinterpret_cast<const uint8_t*>(content.data())),
            content.size()};
    auto data = cb::mcbp::lldb::parseDump(buf);

    const std::vector<uint8_t> bytes = {
            {0x81, 0x0d, 0x00, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x14, 0xbf, 0xf4, 0x26,
             0x8a, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x61,
             0x81, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 0x61,
             0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x74, 0x6f, 0x74, 0x61, 0x6c, 0x5f, 0x63,
             0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x73,
             0x33, 0x36, 0x81, 0x10, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00}};

    EXPECT_EQ(bytes, data);
}
