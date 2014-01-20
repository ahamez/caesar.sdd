#include <ostream>

#include "pn/unit.hh"

namespace pnmc { namespace pn {

/*------------------------------------------------------------------------------------------------*/

unit::unit( unsigned int id
          , std::vector<std::reference_wrapper<const place>>&& places
          , std::vector<std::reference_wrapper<const unit>>&& units)
  : id(id), places(places), units(units)
{}

/*------------------------------------------------------------------------------------------------*/

bool
operator<(const unit& left, const unit& right)
noexcept
{
  return left.id < right.id;
}

/*------------------------------------------------------------------------------------------------*/

/// Export a unit to an output stream.
std::ostream&
operator<<(std::ostream& os, const unit& u)
{
  return os << "u " << u.id;
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::pn
