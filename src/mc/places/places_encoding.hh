#ifndef _PNMC_MC_PLACES_ENCODING_HH_
#define _PNMC_MC_PLACES_ENCODING_HH_

#include "conf/configuration.hh"
#include "mc/mc_impl.hh"

namespace pnmc { namespace mc {

/*------------------------------------------------------------------------------------------------*/

struct places_encoding
  : public mc_impl
{
  const conf::pnmc_configuration& conf;

  places_encoding(const conf::pnmc_configuration& c);

  void
  operator()(const pn::net& net) const;
};

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::mc

#endif // _PNMC_MC_PLACES_ENCODING_HH_
