#include <istream>

#include "parsers/bpn.hh"
#include "parsers/parse.hh"

namespace pnmc { namespace parsers {
  
/*------------------------------------------------------------------------------------------------*/

std::shared_ptr<pn::net>
parse(std::istream& in)
{
  return parsers::bpn(in);
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::parsers
