#include <sstream>

#include "gtest/gtest.h"

#include "mc/units/concurrent_units.hh"
#include "parsers/bpn.hh"

/*------------------------------------------------------------------------------------------------*/

using namespace caesar::mc::units;

/*------------------------------------------------------------------------------------------------*/

struct concurrent_units_test
  : public testing::Test
{
  const sdd_conf conf;

  sdd::manager<sdd_conf> m;

  const sdd::SDD<sdd_conf> zero;
  const sdd::SDD<sdd_conf> one;

  concurrent_units_test()
    : conf(mk_small_conf())
    , m(sdd::manager<sdd_conf>::init(conf))
    , zero(sdd::zero<sdd_conf>())
    , one(sdd::one<sdd_conf>())
  {}

  static
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
};

/*------------------------------------------------------------------------------------------------*/

TEST_F(concurrent_units_test, no_nested_units)
{
  const std::string bpn_desc0 =
    "\
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
  std::istringstream stream0(bpn_desc0);
  const auto net0_ptr = caesar::parsers::bpn(stream0);

  {
    const order o(order_builder{3,2,1,0});
    const SDD x(3, {1}, SDD(2, {0}, SDD(1, {1}, SDD(0, {0}, one))));

    // Don't test with 0, it contains every other units.
    ASSERT_TRUE(query(*net0_ptr, x, o, 3, 1));
    ASSERT_TRUE(query(*net0_ptr, x, o, 1, 3));
    ASSERT_FALSE(query(*net0_ptr, x, o, 1, 2));
    ASSERT_FALSE(query(*net0_ptr, x, o, 2, 1));
    ASSERT_FALSE(query(*net0_ptr, x, o, 2, 3));
    ASSERT_FALSE(query(*net0_ptr, x, o, 3, 2));
  }
}

/*------------------------------------------------------------------------------------------------*/
