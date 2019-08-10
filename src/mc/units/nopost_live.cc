#include <ostream>

#include "mc/units/nopost_live.hh"

namespace caesar { namespace mc { namespace units {

/*------------------------------------------------------------------------------------------------*/

nopost_live::nopost_live(unsigned int transition, boost::dynamic_bitset<>& bitset)
  : transition(transition), bitset(bitset)
{}

/*------------------------------------------------------------------------------------------------*/

sdd::values::flat_set<unsigned int>
nopost_live::operator()(const sdd::values::flat_set<unsigned int>& val)
const noexcept
{
  bitset[transition] = true;
  return val;
}

/*------------------------------------------------------------------------------------------------*/

std::ostream&
operator<<(std::ostream& os, const nopost_live&)
{
  return os << "nopost()";
}

/*------------------------------------------------------------------------------------------------*/

/// @brief Equality of two nopost_live.
bool
operator==(const nopost_live& lhs, const nopost_live& rhs) noexcept
{
  return lhs.transition == rhs.transition;
}

/*------------------------------------------------------------------------------------------------*/

}}} // namespace caesar::mc::units

namespace std {

/*------------------------------------------------------------------------------------------------*/

std::size_t
hash<caesar::mc::units::nopost_live>::operator()(const caesar::mc::units::nopost_live& p)
const noexcept
{
  return sdd::hash::seed(p.transition);
}

/*------------------------------------------------------------------------------------------------*/

} // namespace std
