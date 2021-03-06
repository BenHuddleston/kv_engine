add_library(mcbp STATIC
            ${Memcached_SOURCE_DIR}/include/mcbp/mcbp.h
            ${Memcached_SOURCE_DIR}/include/mcbp/protocol/datatype.h
            ${Memcached_SOURCE_DIR}/include/mcbp/protocol/feature.h
            ${Memcached_SOURCE_DIR}/include/mcbp/protocol/framebuilder.h
            ${Memcached_SOURCE_DIR}/include/mcbp/protocol/magic.h
            ${Memcached_SOURCE_DIR}/include/mcbp/protocol/opcode.h
            ${Memcached_SOURCE_DIR}/include/mcbp/protocol/request.h
            ${Memcached_SOURCE_DIR}/include/mcbp/protocol/response.h
            ${Memcached_SOURCE_DIR}/include/mcbp/protocol/status.h
            datatype.cc
            dcp_snapshot_marker_codec.cc
            dcp_stream_end_status.cc
            dump.cc
            ewb_encode.cc
            feature.cc
            gdb_dump_parser.cc
            lldb_dump_parser.cc
            magic.cc
            opcode.cc
            request.cc
            response.cc
            header.cc
            sla.cc
            status.cc
            )

target_link_libraries(mcbp mcd_util platform)
set_property(TARGET mcbp PROPERTY POSITION_INDEPENDENT_CODE 1)

add_executable(mcbp_packet_printer mcbp_packet_printer.cc)
target_link_libraries(mcbp_packet_printer mcbp platform)
add_sanitizers(mcbp_packet_printer)

add_executable(mcbp_info mcbp_info.cc)
target_link_libraries(mcbp_info mcbp)
add_sanitizers(mcbp_info)

if (COUCHBASE_KV_BUILD_UNIT_TESTS)
    add_executable(mcbp_unit_tests
                   feature_test.cc
                   framebuilder_test.cc
                   magic_test.cc
                   mcbp_dump_parser_test.cc
                   opcode_test.cc
                   request_test.cc
                   sla_test.cc
                   status_test.cc)
    target_link_libraries(mcbp_unit_tests mcbp gtest gtest_main)
    add_test(NAME mcbp_unit_tests
             WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
             COMMAND mcbp_unit_tests)
    add_sanitizers(mcbp_unit_tests)
endif ()
