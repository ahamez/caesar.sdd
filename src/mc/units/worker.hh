#pragma once

#include "conf/configuration.hh"
#include "mc/mc_impl.hh"

namespace caesar { namespace mc { namespace units {

/*------------------------------------------------------------------------------------------------*/

struct worker
  : public mc_impl
{
  const conf::caesar_configuration& conf;

  worker(const conf::caesar_configuration& c);

  void
  operator()(const pn::net& net) const;
};

/*------------------------------------------------------------------------------------------------*/

}}} // namespace caesar::mc::units
