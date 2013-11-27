#ifndef _PNMC_MC_PRE_HH_
#define _PNMC_MC_PRE_HH_

#include <functional> // hash
#include <iosfwd>

#include "sdd/values/bitset.hh"
//#include "sdd/values_manager.hh"

namespace pnmc { namespace mc { namespace places {

/*------------------------------------------------------------------------------------------------*/

struct pre
{
  sdd::values::bitset<64>
  operator()(const sdd::values::bitset<64>&) const noexcept;
};

/// @brief Equality of two pre.
constexpr bool
operator==(const pre&, const pre&)
noexcept
{
  return true;
}

/// @brief Textual output of a pre.
std::ostream&
operator<<(std::ostream&, const pre&);

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::places

namespace std {

/*------------------------------------------------------------------------------------------------*/

template <>
struct hash<pnmc::mc::places::pre>
{
  std::size_t operator()(const pnmc::mc::places::pre&) const noexcept;
};

/*------------------------------------------------------------------------------------------------*/

} // namespace std

#endif // _PNMC_MC_PRE_HH_
