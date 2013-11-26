#ifndef _PNMC_MC_UNIT_HH_
#define _PNMC_MC_UNIT_HH_

#include "conf/configuration.hh"
#include "mc/places/sdd.hh"
#include "pn/net.hh"

namespace pnmc { namespace mc {

/*------------------------------------------------------------------------------------------------*/

void
compute_concurrent_units( const conf::pnmc_configuration&, const pn::net&
                        , const sdd::order<sdd_conf>&, SDD);

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::mc

#endif // _PNMC_MC_UNIT_HH_