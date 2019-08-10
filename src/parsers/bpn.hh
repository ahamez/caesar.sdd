#pragma once

#include <iosfwd>
#include <memory>

#include "pn/net.hh"

namespace caesar { namespace parsers {

/*------------------------------------------------------------------------------------------------*/

std::shared_ptr<pn::net>
bpn(std::istream&);

/*------------------------------------------------------------------------------------------------*/

}} // caesar::parsers
