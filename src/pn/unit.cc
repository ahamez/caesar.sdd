#include <algorithm>
#include <ostream>

#include "pn/unit.hh"

namespace pnmc { namespace pn {

/*------------------------------------------------------------------------------------------------*/

unit::unit( unsigned int id
          , std::vector<std::reference_wrapper<const place>>&& places
          , std::vector<std::reference_wrapper<const unit>>&& units
          , std::vector<unsigned int>&& subunits)
  : id(id), places(std::move(places)), units(std::move(units)), subunits(std::move(subunits))
{}

/*------------------------------------------------------------------------------------------------*/

namespace /* anonymous */ {

void
ostream_helper(std::ostream& os, const unit& u, unsigned int depth)
{
  const std::string indent(depth, ' ');
  os << indent << "U" << u.id << " : ";
  if (not u.places.empty())
  {
    std::for_each( u.places.begin(), std::prev(u.places.end())
                 , [&](const place& p){os << p.id << ", ";});
    os << std::prev(u.places.end())->get().id;;
  }
  os << std::endl;
  for (const auto& sub : u.units)
  {
    ostream_helper(os, sub, depth + 2);
  }
}

} // namespace anonymous

/// Export a unit to an output stream.
std::ostream&
operator<<(std::ostream& os, const unit& u)
{
  ostream_helper(os, u, 0);
  return os;
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::pn
