#pragma once

#include "conf/configuration.hh"
#include "mc/units/sdd.hh"
#include "pn/net.hh"

namespace caesar { namespace mc { namespace units {

/*------------------------------------------------------------------------------------------------*/

bool
query(const pn::net&, const SDD&, const order&, unsigned int, unsigned int);

/*------------------------------------------------------------------------------------------------*/

void
compute_concurrent_units( const conf::caesar_configuration&, const pn::net&
                        , const sdd::order<sdd_conf>&, const SDD&);

/*------------------------------------------------------------------------------------------------*/

}}} // namespace caesar::mc::units
