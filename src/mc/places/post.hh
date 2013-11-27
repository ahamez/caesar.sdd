#ifndef _PNMC_MC_POST_HH_
#define _PNMC_MC_POST_HH_

#include <functional> // hash
#include <iosfwd>

#include "sdd/values/bitset.hh"

namespace pnmc { namespace mc { namespace places {

/*------------------------------------------------------------------------------------------------*/

struct post
{
  sdd::values::bitset<64>
  operator()(const sdd::values::bitset<64>&)
  const noexcept;
};

/// @brief Equality of two post.
constexpr bool
operator==(const post&, const post&)
noexcept
{
  return true;
}

/// @brief Textual output of a post.
std::ostream&
operator<<(std::ostream&, const post&);

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::places

namespace std
{

/*------------------------------------------------------------------------------------------------*/

template <>
struct hash<pnmc::mc::places::post>
{
  std::size_t operator()(const pnmc::mc::places::post&) const noexcept;
};

/*------------------------------------------------------------------------------------------------*/

} // namespace std

#endif // _PNMC_MC_POST_HH_
