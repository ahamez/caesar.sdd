#include <ostream>

#include "mc/pre.hh"

namespace pnmc { namespace mc {

/*------------------------------------------------------------------------------------------------*/

sdd::values::bitset<64>
pre::operator()(const sdd::values::bitset<64>& val)
const noexcept
{
  return val >> 1;
}

/*------------------------------------------------------------------------------------------------*/

std::ostream&
operator<<(std::ostream& os, const pre& p)
{
  return os << "pre";
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::mc

namespace std {

/*------------------------------------------------------------------------------------------------*/

std::size_t
hash<pnmc::mc::pre>::operator()(const pnmc::mc::pre& p)
const noexcept
{
  return std::hash<unsigned int>()(3464152273);
}

/*------------------------------------------------------------------------------------------------*/

} // namespace std
