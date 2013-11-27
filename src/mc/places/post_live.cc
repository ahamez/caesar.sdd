#include <ostream>

#include <sdd/util/hash.hh>

#include "mc/places/post_live.hh"

namespace pnmc { namespace mc { namespace places {

/*------------------------------------------------------------------------------------------------*/

post_live::post_live(std::size_t index, boost::dynamic_bitset<>& bitset)
  : index(index), bitset(bitset)
{}

/*------------------------------------------------------------------------------------------------*/

sdd::values::bitset<64>
post_live::operator()(const sdd::values::bitset<64>& val)
const noexcept
{
  bitset[index] = true;
  return val << 1;
}

/*------------------------------------------------------------------------------------------------*/

bool
operator==(const post_live& lhs, const post_live& rhs)
noexcept
{
  return lhs.index == rhs.index;
}

/*------------------------------------------------------------------------------------------------*/

std::ostream&
operator<<(std::ostream& os, const post_live& l)
{
  return os << "post_live(" << l.index << ")";
}

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::places

namespace std
{

/*------------------------------------------------------------------------------------------------*/

std::size_t
hash<pnmc::mc::places::post_live>::operator()(const pnmc::mc::places::post_live& l)
const noexcept
{
  std::size_t seed = 49979687;
  sdd::util::hash_combine(seed, l.index);
  return seed;
}

/*------------------------------------------------------------------------------------------------*/

} // namespace std
