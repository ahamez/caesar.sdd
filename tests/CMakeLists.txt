set(SOURCES
    tests.cc
    test_concurrent_units.cc
    ${PROJECT_SOURCE_DIR}/src/compress/compress.cc
    ${PROJECT_SOURCE_DIR}/src/mc/units/concurrent_units.cc
    ${PROJECT_SOURCE_DIR}/src/parsers/bpn.cc
    ${PROJECT_SOURCE_DIR}/src/parsers/parse.cc
    ${PROJECT_SOURCE_DIR}/src/parsers/parse_error.cc
    ${PROJECT_SOURCE_DIR}/src/pn/net.cc
    ${PROJECT_SOURCE_DIR}/src/pn/place.cc
    ${PROJECT_SOURCE_DIR}/src/pn/transition.cc
    ${PROJECT_SOURCE_DIR}/src/pn/unit.cc
    )

add_executable(tests ${SOURCES})
target_link_libraries(tests ${Boost_LIBRARIES})
add_test("UnitTests" tests)
