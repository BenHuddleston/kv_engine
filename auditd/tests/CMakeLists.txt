include_directories(AFTER ${Memcached_SOURCE_DIR}/auditd/src)
add_executable(memcached_audit_test
               auditfile_test.cc
               auditconfig_test.cc
               eventdescriptor_test.cc
               ${Memcached_SOURCE_DIR}/auditd/src/eventdescriptor.cc
               ${Memcached_SOURCE_DIR}/auditd/src/eventdescriptor.h
               ${Memcached_SOURCE_DIR}/include/memcached/isotime.h)
target_link_libraries(memcached_audit_test
                      auditd
                      memcached_logger
                      mcd_time
                      platform
                      gtest
                      gtest_main
                      mcd_util)
add_test(NAME memcached-audit-test
         WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
         COMMAND memcached_audit_test)
add_sanitizers(memcached_audit_test)

add_executable(memcached_auditd_tests testauditd.cc)
target_link_libraries(memcached_auditd_tests
                      auditd memcached_logger mcd_util mcd_time platform gtest)
add_dependencies(memcached_auditd_tests generate_audit_descriptors)
add_sanitizers(memcached_auditd_tests)
add_test(NAME memcached-auditd-test
         COMMAND memcached_auditd_tests -e ${Memcached_BINARY_DIR}/auditd)
