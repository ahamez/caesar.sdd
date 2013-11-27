#include <ostream>

#include "mc/places/post.hh"

namespace pnmc { namespace mc { namespace places {

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

}}} // namespace pnmc::mc::places

namespace std
{

/*------------------------------------------------------------------------------------------------*/

std::size_t
hash<pnmc::mc::places::post>::operator()(const pnmc::mc::places::post& p)
const noexcept
{
  return std::hash<unsigned int>()(2564450027);
}

/*------------------------------------------------------------------------------------------------*/

} // namespace std
