#include <ostream>

#include "mc/units/post.hh"

namespace pnmc { namespace mc { namespace units {

/*------------------------------------------------------------------------------------------------*/

post::post(unsigned int p)
  : place(p)
{}

/*------------------------------------------------------------------------------------------------*/

sdd::values::flat_set<unsigned int>
post::operator()(const sdd::values::flat_set<unsigned int>& val)
const noexcept
{
  return {place + 1};
}

/*------------------------------------------------------------------------------------------------*/

std::ostream&
operator<<(std::ostream& os, const post& p)
{
  return os << "post(" << p.place << ")";
}

/*------------------------------------------------------------------------------------------------*/

/// @brief Equality of two post.
bool
operator==(const post& lhs, const post& rhs) noexcept
{
  return lhs.place == rhs.place;
}

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::units

namespace std {

/*------------------------------------------------------------------------------------------------*/

std::size_t
hash<pnmc::mc::units::post>::operator()(const pnmc::mc::units::post& p)
const noexcept
{
  using namespace sdd::hash;
  return seed(9867241367) (val(p.place));
}

/*------------------------------------------------------------------------------------------------*/

} // namespace std
