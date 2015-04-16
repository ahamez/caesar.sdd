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
    return {0};
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
  using namespace sdd::hash;
  return seed(667871) (val(p.place));
}

/*------------------------------------------------------------------------------------------------*/

} // namespace std
