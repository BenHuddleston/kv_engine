add_library(statistics STATIC
            collector.cc
            cbstat_collector.cc
            definitions.cc
            labelled_collector.cc
            prometheus.cc
            prometheus_collector.cc
            )

add_sanitizers(statistics)

add_dependencies(statistics generated_source_files)

target_include_directories(statistics
        PUBLIC ${PROMETHEUS_INCLUDE_DIR}
               ${Memcached_BINARY_DIR}/engines/ep/src)

target_link_libraries(statistics mcd_util ${PROMETHEUS_LIBRARIES})
