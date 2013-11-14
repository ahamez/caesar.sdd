#include <ostream>

#include "mc/post.hh"

namespace pnmc { namespace mc {

/*------------------------------------------------------------------------------------------------*/

sdd::values::bitset<64>
post::operator()(const sdd::values::bitset<64>& val)
const noexcept
{
  return val << 1;
}

/*------------------------------------------------------------------------------------------------*/

std::ostream&
operator<<(std::ostream& os, const post& p)
{
  return os << "post";
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::mc

namespace std
{

/*------------------------------------------------------------------------------------------------*/

std::size_t
hash<pnmc::mc::post>::operator()(const pnmc::mc::post& p)
const noexcept
{
  return std::hash<unsigned int>()(2564450027);
}

/*------------------------------------------------------------------------------------------------*/

} // namespace std
