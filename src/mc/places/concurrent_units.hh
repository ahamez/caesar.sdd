#ifndef _PNMC_MC_PLACES_CONCURRENT_UNITS_HH_
#define _PNMC_MC_PLACES_CONCURRENT_UNITS_HH_

#include "conf/configuration.hh"
#include "mc/places/sdd.hh"
#include "pn/net.hh"

namespace pnmc { namespace mc { namespace places {

/*------------------------------------------------------------------------------------------------*/

void
compute_concurrent_units( const conf::pnmc_configuration&, const pn::net&
                        , const sdd::order<sdd_conf>&, SDD);

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::places

#endif // _PNMC_MC_PLACES_CONCURRENT_UNITS_HH_
