#include <sstream>

#include <catch.hpp>

#include "mc/units/concurrent_units.hh"
#include "mc/units/sdd.hh"
#include "parsers/bpn.hh"

/*------------------------------------------------------------------------------------------------*/

using namespace caesar::mc::units;

/*------------------------------------------------------------------------------------------------*/

sdd_conf
mk_small_conf()
{
  sdd_conf c;
  c.sdd_unique_table_size = 1000;
  c.sdd_difference_cache_size = 1000;
  c.sdd_intersection_cache_size = 1000;
  c.sdd_sum_cache_size = 1000;
  c.hom_unique_table_size = 1000;
  c.hom_cache_size = 1000;
  return c;
}

/*------------------------------------------------------------------------------------------------*/

TEST_CASE("No nested units")
{
  const auto m = sdd::init(mk_small_conf());
  const auto zero = sdd::zero<sdd_conf>();
  const auto one = sdd::one<sdd_conf>();

  const auto bpn_desc0 = "\
    places #4 0...3\n\
    initial place 0\n\
    units #4 0...3\n\
    root unit 0\n\
    U3 #1 3...3 #0\n\
    U2 #1 2...2 #0\n\
    U1 #1 1...1 #0\n\
    U0 #1 0...0 #3 1 2 3\n\
    transitions #0 0...0\n\
  ";

  auto stream0 = std::istringstream{bpn_desc0};
  const auto net0_ptr = caesar::parsers::bpn(stream0);

  {
    const auto o = order_builder{3, 2, 1, 0};
    const auto x = SDD{3, {1}, SDD{2, {0}, SDD{1, {1}, SDD{0, {0}, one}}}};

    // Don't test with 0, it contains every other units.
    REQUIRE(query(*net0_ptr, x, o, 3, 1));
    REQUIRE(query(*net0_ptr, x, o, 1, 3));
    REQUIRE_FALSE(query(*net0_ptr, x, o, 1, 2));
    REQUIRE_FALSE(query(*net0_ptr, x, o, 2, 1));
    REQUIRE_FALSE(query(*net0_ptr, x, o, 2, 3));
    REQUIRE_FALSE(query(*net0_ptr, x, o, 3, 2));
  }
}

/*------------------------------------------------------------------------------------------------*/
