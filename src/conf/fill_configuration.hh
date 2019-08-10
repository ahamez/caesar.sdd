#pragma once

#include <boost/optional.hpp>

#include "conf/configuration.hh"

namespace pnmc { namespace conf {

/*------------------------------------------------------------------------------------------------*/

boost::optional<pnmc_configuration>
fill_configuration(int, char**);

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::conf
