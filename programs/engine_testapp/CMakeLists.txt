INCLUDE_DIRECTORIES(BEFORE SYSTEM ${hdr_histogram_SOURCE_DIR}/src)

add_library(mock_server STATIC
            mock_cookie.cc
            mock_cookie.h
            mock_engine.cc
            mock_engine.h
            mock_server.cc
            mock_server.h
)
set_property(TARGET mock_server PROPERTY POSITION_INDEPENDENT_CODE 1)
target_link_libraries(mock_server PUBLIC memcached_logger)
target_link_libraries(mock_server PRIVATE platform_headers)
target_link_libraries(mock_server INTERFACE
                      mcd_tracing
                      hdr_histogram_static
                      mcd_util
                      platform
                      xattr
                      ${MALLOC_LIBRARIES})

add_library(engine_testapp OBJECT engine_testapp.cc
        ${Memcached_SOURCE_DIR}/daemon/bucket_type.cc
        ${Memcached_SOURCE_DIR}/daemon/doc_pre_expiry.cc
        ${Memcached_SOURCE_DIR}/daemon/enginemap.cc
        ${Memcached_SOURCE_DIR}/daemon/enginemap.h
        ${Memcached_SOURCE_DIR}/include/memcached/bucket_type.h
)
target_link_libraries(engine_testapp PUBLIC Folly::headers memcached_logger)

# An object library will not propagate dependencies
# https://gitlab.kitware.com/cmake/cmake/issues/18090
# Here we create an interface library to bring the libraries that
# will be needed in a target
add_library(engine_testapp_dependencies INTERFACE)
target_link_libraries(engine_testapp_dependencies INTERFACE
                      phosphor
                      platform
                      mock_server
                      crash_engine
                      default_engine
                      ep
                      ewouldblock_engine
                      nobucket
                      ${COUCHBASE_NETWORK_LIBS})
add_sanitizers(engine_testapp)
