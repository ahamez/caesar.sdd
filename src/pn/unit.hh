#ifndef _PNMC_PN_UNIT_HH_
#define _PNMC_PN_UNIT_HH_

#include <functional>
#include <iosfwd>
#include <vector>

#include "pn/place.hh"

namespace pnmc { namespace pn {

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

  /// @brief Constructor.
  unit( unsigned int id
      , std::vector<std::reference_wrapper<const place>>&& places
      , std::vector<std::reference_wrapper<const unit>>&& units);
};

/*------------------------------------------------------------------------------------------------*/

/// @brief Compare two units.
bool
operator<(const unit&, const unit&) noexcept;

/*------------------------------------------------------------------------------------------------*/

/// @brief Export a unit to an output stream.
std::ostream&
operator<<(std::ostream&, const unit&);

/*------------------------------------------------------------------------------------------------*/
  
}} // namespace pnmc::pn

#endif // _PNMC_PN_UNIT_HH_
