#include <cassert>
#include <chrono>
#include <deque>
#include <iostream>
#include <set>
#include <utility> // pair

#include "mc/units/sdd.hh"
#include "mc/units/worker.hh"

namespace pnmc { namespace mc { namespace units {

namespace chrono = std::chrono;

/*------------------------------------------------------------------------------------------------*/

worker::worker(const conf::pnmc_configuration& c)
  : conf(c)
{}

void
worker::operator()(const pn::net& net)
const
{
  auto manager = sdd::manager<sdd_conf>::init();
}

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::units
