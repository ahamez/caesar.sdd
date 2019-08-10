#pragma once

#include <functional> // hash
#include <iosfwd>

#include <boost/dynamic_bitset.hpp>

#include "sdd/values/flat_set.hh"
#include "sdd/values_manager.hh"

namespace pnmc { namespace mc { namespace units {

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
  sdd::values::flat_set<unsigned int>
  operator()(const sdd::values::flat_set<unsigned int>&) const noexcept;
};

/// @brief Equality of two nopost_live.
bool
operator==(const nopost_live& lhs, const nopost_live& rhs) noexcept;

/// @brief Textual output of a nopost_live.
std::ostream&
operator<<(std::ostream&, const nopost_live&);

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::units

namespace std {

/*------------------------------------------------------------------------------------------------*/

template <>
struct hash<pnmc::mc::units::nopost_live>
{
  std::size_t operator()(const pnmc::mc::units::nopost_live&) const noexcept;
};

/*------------------------------------------------------------------------------------------------*/

} // namespace std
