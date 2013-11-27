#ifndef _PNMC_MC_POST_LIVE_HH_
#define _PNMC_MC_POST_LIVE_HH_

#include <functional> // hash
#include <iosfwd>

#include <boost/dynamic_bitset.hpp>

#include "sdd/values/bitset.hh"

namespace pnmc { namespace mc { namespace places {

/*------------------------------------------------------------------------------------------------*/

struct post_live
{
  const std::size_t index;
  boost::dynamic_bitset<>& bitset;

  post_live(std::size_t, boost::dynamic_bitset<>&);

  sdd::values::bitset<64>
  operator()(const sdd::values::bitset<64>&)
  const noexcept;
};

/// @brief Equality of two post_live.
bool
operator==(const post_live&, const post_live&)
noexcept;

/// @brief Textual output of a post_live.
std::ostream&
operator<<(std::ostream&, const post_live&);

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::places

namespace std
{

/*------------------------------------------------------------------------------------------------*/

template <>
struct hash<pnmc::mc::places::post_live>
{
  std::size_t operator()(const pnmc::mc::places::post_live&) const noexcept;
};

/*------------------------------------------------------------------------------------------------*/

} // namespace std

#endif // _PNMC_MC_POST_LIVE_HH_
