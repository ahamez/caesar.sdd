#pragma once

#include <iosfwd>
#include <map>

namespace caesar { namespace pn {

/*------------------------------------------------------------------------------------------------*/

/// @brief A transition in a Petri Net.
struct transition
{
  /// @brief This transition's id.
  const unsigned int id;

  /// @brief Pre-places: place_id x valuation
  std::multimap<unsigned int, unsigned int> pre;

  /// @brief Post-places: place_id x valuation
  std::multimap<unsigned int, unsigned int> post;

  /// @brief Constructor.
  transition(unsigned int id);
};

/*------------------------------------------------------------------------------------------------*/

/// @brief Compare two transitions using their ids.
bool
operator<(const transition&, const transition&) noexcept;

/*------------------------------------------------------------------------------------------------*/

/// @brief Export a transition to an output stream.
std::ostream&
operator<<(std::ostream&, const transition&);

/*------------------------------------------------------------------------------------------------*/

}} // namespace caesar::pn
