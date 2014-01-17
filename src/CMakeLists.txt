set(SOURCES
    main.cc
    conf/fill_configuration.cc
    mc/places/concurrent_units.cc
    mc/places/post.cc
    mc/places/post_live.cc
    mc/places/pre.cc
    mc/places/worker.cc
    mc/units/concurrent_units.cc
    mc/units/nopost_live.cc
    mc/units/post.cc
    mc/units/post_live.cc
    mc/units/pre.cc
    mc/units/worker.cc
    mc/mc.cc
    mc/mc_impl.cc
    parsers/bpn.cc
    parsers/helpers.cc
    parsers/parse_error.cc
    parsers/parse.cc
    pn/net.cc
    pn/place.cc
    pn/transition.cc
    )

add_executable(caesar.sdd ${SOURCES})
target_link_libraries(caesar.sdd ${Boost_LIBRARIES})
