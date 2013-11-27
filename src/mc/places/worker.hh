#ifndef _PNMC_MC_WORKER_HH_
#define _PNMC_MC_WORKER_HH_

#include "conf/configuration.hh"
#include "mc/mc_impl.hh"

namespace pnmc { namespace mc { namespace places {

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

}}} // namespace pnmc::mc::places

#endif // _PNMC_MC_WORKER_HH_
