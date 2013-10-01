#include <ostream>

#include <sdd/util/hash.hh>

#include "mc/post.hh"

namespace pnmc { namespace mc {

/*------------------------------------------------------------------------------------------------*/

post::post(unsigned int v)
  : valuation(v)
{}

/*------------------------------------------------------------------------------------------------*/

sdd::values::flat_set<unsigned int>
post::operator()(const sdd::values::flat_set<unsigned int>& val)
const
{
  sdd::values::flat_set<unsigned int> new_val;
  for (const auto& v : val)
  {
    new_val.insert(v + valuation);
  }
  return new_val;
}

/*------------------------------------------------------------------------------------------------*/

bool
operator==(const post& lhs, const post& rhs)
noexcept
{
  return lhs.valuation == rhs.valuation;
}

/*------------------------------------------------------------------------------------------------*/

std::ostream&
operator<<(std::ostream& os, const post& p)
{
  return os << "post(" << p.valuation << ")";
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
  std::size_t seed = 2564450027;
  sdd::util::hash_combine(seed, p.valuation);
  return seed;
}

/*------------------------------------------------------------------------------------------------*/

} // namespace std