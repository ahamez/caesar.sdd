set(SOURCES
    main.cc
    conf/fill_configuration.cc
    mc/post.cc
    mc/post_live.cc
    mc/pre.cc
    mc/unit.cc
    mc/work.cc
    parsers/bpn.cc
    parsers/helpers.cc
    parsers/parse_error.cc
    parsers/parse.cc
    pn/module.cc
    pn/net.cc
    pn/place.cc
    pn/transition.cc
    )

add_executable(caesar.sdd ${SOURCES})
target_link_libraries(caesar.sdd ${Boost_LIBRARIES})
