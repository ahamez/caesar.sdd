#include <ostream>

#include "mc/places/nopost_live.hh"

namespace pnmc { namespace mc { namespace places {

/*------------------------------------------------------------------------------------------------*/

nopost_live::nopost_live(unsigned int transition, boost::dynamic_bitset<>& bitset)
  : transition(transition), bitset(bitset)
{}

/*------------------------------------------------------------------------------------------------*/

sdd::values::bitset<64>
nopost_live::operator()(const sdd::values::bitset<64>& val)
const noexcept
{
  bitset[transition] = true;
  return val;
}

/*------------------------------------------------------------------------------------------------*/

std::ostream&
operator<<(std::ostream& os, const nopost_live& p)
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

}}} // namespace pnmc::mc::places

namespace std {

/*------------------------------------------------------------------------------------------------*/

std::size_t
hash<pnmc::mc::places::nopost_live>::operator()(const pnmc::mc::places::nopost_live& p)
const noexcept
{
  return sdd::util::hash(p.transition);
}

/*------------------------------------------------------------------------------------------------*/

} // namespace std
