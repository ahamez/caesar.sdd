#pragma once

#include <iosfwd>
#include <string>

#include "pn/net.hh"

namespace caesar { namespace pn {

/*------------------------------------------------------------------------------------------------*/

/// @brief Export net to TINA format.
void
tina(std::ostream&, const net&);

/*------------------------------------------------------------------------------------------------*/

}} // namespace caesar::pn
