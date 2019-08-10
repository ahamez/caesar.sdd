#pragma once

#include <iosfwd>
#include <memory>

#include "conf/configuration.hh"
#include "pn/net.hh"

namespace caesar { namespace parsers {

/*------------------------------------------------------------------------------------------------*/

std::shared_ptr<pn::net>
parse(std::istream&);

/*------------------------------------------------------------------------------------------------*/

}} // namespace caesar::parsers
