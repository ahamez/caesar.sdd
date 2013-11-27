#ifndef _PNMC_MC_UNITS_WORKER_HH_
#define _PNMC_MC_UNITS_WORKER_HH_

#include "conf/configuration.hh"
#include "mc/mc_impl.hh"

namespace pnmc { namespace mc { namespace units {

/*------------------------------------------------------------------------------------------------*/

struct worker
  : public mc_impl
{
  const conf::pnmc_configuration& conf;

  worker(const conf::pnmc_configuration& c);

  void
  operator()(const pn::net& net) const;
};

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::units

#endif // _PNMC_MC_UNITS_WORKER_HH_
