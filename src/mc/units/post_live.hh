#ifndef _PNMC_MC_UNITS_POST_LIVE_HH_
#define _PNMC_MC_UNITS_POST_LIVE_HH_

#include <functional> // hash
#include <iosfwd>

#include <boost/dynamic_bitset.hpp>

#include "sdd/values/flat_set.hh"
#include "sdd/values_manager.hh"

namespace pnmc { namespace mc { namespace units {

/*------------------------------------------------------------------------------------------------*/

struct post_live
{
  /// @brief Target place.
  const unsigned int place;

  /// @brief Transition index.
  const unsigned int transition;

  /// @brief The bitset to update.
  boost::dynamic_bitset<>& bitset;

  /// @brief Constructor.
  post_live(unsigned int place, unsigned int transition, boost::dynamic_bitset<>& bitset);

  /// @brief Apply this operation.
  sdd::values::flat_set<unsigned int>
  operator()(const sdd::values::flat_set<unsigned int>&) const noexcept;
};

/// @brief Equality of two post_live.
bool
operator==(const post_live& lhs, const post_live& rhs) noexcept;

/// @brief Textual output of a post.
std::ostream&
operator<<(std::ostream&, const post_live&);

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::units

namespace std {

/*------------------------------------------------------------------------------------------------*/

template <>
struct hash<pnmc::mc::units::post_live>
{
  std::size_t operator()(const pnmc::mc::units::post_live&) const noexcept;
};

/*------------------------------------------------------------------------------------------------*/

} // namespace std

#endif // _PNMC_MC_UNITS_POST_LIVE_HH_
