#include <ostream>

#include "mc/units/pre.hh"
#include "sdd/util/hash.hh"

namespace pnmc { namespace mc { namespace units {

/*------------------------------------------------------------------------------------------------*/

pre::pre(unsigned int p)
  : place(p)
{}

/*------------------------------------------------------------------------------------------------*/

sdd::values::flat_set<unsigned int>
pre::operator()(const sdd::values::flat_set<unsigned int>& val)
const noexcept
{
  if (val.find(place + 1) != val.end())
  {
    return {place + 1};
  }
  else
  {
    return {};
  }
}

/*------------------------------------------------------------------------------------------------*/

std::ostream&
operator<<(std::ostream& os, const pre& p)
{
  return os << "pre(" << p.place << ")";
}

/*------------------------------------------------------------------------------------------------*/

bool
operator==(const pre& lhs, const pre& rhs)
noexcept
{
  return lhs.place == rhs.place;
}

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::units

namespace std {

/*------------------------------------------------------------------------------------------------*/

std::size_t
hash<pnmc::mc::units::pre>::operator()(const pnmc::mc::units::pre& p)
const noexcept
{
  std::size_t seed = 667871;
  sdd::util::hash_combine(seed, p.place);
  return seed;
}

/*------------------------------------------------------------------------------------------------*/

} // namespace std
