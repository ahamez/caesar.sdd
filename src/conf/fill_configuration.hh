#pragma once

#include <boost/optional.hpp>

#include "conf/configuration.hh"

namespace caesar { namespace conf {

/*------------------------------------------------------------------------------------------------*/

boost::optional<caesar_configuration>
fill_configuration(int, char**);

/*------------------------------------------------------------------------------------------------*/

}} // namespace caesar::conf
