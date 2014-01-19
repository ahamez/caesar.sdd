#ifndef _PNMC_MC_PLACES_NOPOST_LIVE_HH_
#define _PNMC_MC_PLACES_NOPOST_LIVE_HH_

#include <functional> // hash
#include <iosfwd>

#include <boost/dynamic_bitset.hpp>

#include "sdd/values/bitset.hh"

namespace pnmc { namespace mc { namespace places {

/*------------------------------------------------------------------------------------------------*/

struct nopost_live
{
  /// @brief Transition index.
  const unsigned int transition;

  /// @brief The bitset to update.
  boost::dynamic_bitset<>& bitset;

  /// @brief Constructor.
  nopost_live(unsigned int transition, boost::dynamic_bitset<>& bitset);

  /// @brief Apply this operation.
  sdd::values::bitset<64>
  operator()(const sdd::values::bitset<64>&) const noexcept;
};

/// @brief Equality of two nopost_live.
bool
operator==(const nopost_live& lhs, const nopost_live& rhs) noexcept;

/// @brief Textual output of a nopost_live.
std::ostream&
operator<<(std::ostream&, const nopost_live&);

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::places

namespace std {

/*------------------------------------------------------------------------------------------------*/

template <>
struct hash<pnmc::mc::places::nopost_live>
{
  std::size_t operator()(const pnmc::mc::places::nopost_live&) const noexcept;
};

/*------------------------------------------------------------------------------------------------*/

} // namespace std

#endif // _PNMC_MC_PLACES_NOPOST_LIVE_HH_
