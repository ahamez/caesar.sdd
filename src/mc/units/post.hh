#ifndef _PNMC_MC_UNITS_POST_HH_
#define _PNMC_MC_UNITS_POST_HH_

#include <functional> // hash
#include <iosfwd>

#include "sdd/values/flat_set.hh"
#include "sdd/values_manager.hh"

namespace pnmc { namespace mc { namespace units {

/*------------------------------------------------------------------------------------------------*/

struct post
{
  /// @brief Target place.
  const unsigned int place;

  /// @brief Constructor.
  post(unsigned int place);

  /// @brief Apply this operation.
  sdd::values::flat_set<unsigned int>
  operator()(const sdd::values::flat_set<unsigned int>&) const noexcept;
};

/// @brief Equality of two post.
bool
operator==(const post& lhs, const post& rhs) noexcept;

/// @brief Textual output of a post.
std::ostream&
operator<<(std::ostream&, const post&);

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::units

namespace std {

/*------------------------------------------------------------------------------------------------*/

template <>
struct hash<pnmc::mc::units::post>
{
  std::size_t operator()(const pnmc::mc::units::post&) const noexcept;
};

/*------------------------------------------------------------------------------------------------*/

} // namespace std

#endif // _PNMC_MC_UNITS_POST_HH_
