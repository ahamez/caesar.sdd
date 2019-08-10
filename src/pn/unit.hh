#pragma once

#include <functional>
#include <iosfwd>
#include <vector>

#include "pn/place.hh"

namespace caesar { namespace pn {

/*------------------------------------------------------------------------------------------------*/

/// @brief A BPN unit.
struct unit
{
  /// @brief This unit's id.
  const unsigned int id;

  ///
  const std::vector<std::reference_wrapper<const place>> places;

  ///
  const std::vector<std::reference_wrapper<const unit>> units;

  ///
  const std::vector<unsigned int> subunits;

  /// @brief Constructor.
  unit( unsigned int id
      , std::vector<std::reference_wrapper<const place>>&& places
      , std::vector<std::reference_wrapper<const unit>>&& units
      , std::vector<unsigned int>&& subunits);
};

/*------------------------------------------------------------------------------------------------*/

/// @brief Export a unit to an output stream.
std::ostream&
operator<<(std::ostream&, const unit&);

/*------------------------------------------------------------------------------------------------*/

}} // namespace caesar::pn
