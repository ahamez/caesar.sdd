#include <ostream>

#include "mc/units/post_live.hh"

namespace pnmc { namespace mc { namespace units {

/*------------------------------------------------------------------------------------------------*/

post_live::post_live(unsigned int place, unsigned int transition, boost::dynamic_bitset<>& bitset)
  : place(place), transition(transition), bitset(bitset)
{}

/*------------------------------------------------------------------------------------------------*/

sdd::values::flat_set<unsigned int>
post_live::operator()(const sdd::values::flat_set<unsigned int>& val)
const noexcept
{
  bitset[transition] = true;
  return {place + 1};
}

/*------------------------------------------------------------------------------------------------*/

std::ostream&
operator<<(std::ostream& os, const post_live& p)
{
  return os << "post(" << p.place << ")";
}

/*------------------------------------------------------------------------------------------------*/

/// @brief Equality of two post_live.
bool
operator==(const post_live& lhs, const post_live& rhs) noexcept
{
  return lhs.place == rhs.place and lhs.transition == rhs.transition;
}

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::units

namespace std {

/*------------------------------------------------------------------------------------------------*/

std::size_t
hash<pnmc::mc::units::post_live>::operator()(const pnmc::mc::units::post_live& p)
const noexcept
{
  using namespace sdd::hash;
  return seed() (val(p.place)) (val(p.transition));
}

/*------------------------------------------------------------------------------------------------*/

} // namespace std
