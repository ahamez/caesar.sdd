#ifndef _PNMC_PN_PLACE_HH_
#define _PNMC_PN_PLACE_HH_

#include <iosfwd>
#include <map>
#include <string>

namespace pnmc { namespace pn {

/*------------------------------------------------------------------------------------------------*/

struct place
{
  /// @brief This place's id.
  const unsigned int id;

  /// @brief This place's marking.
  unsigned int marking;

  /// @brief The unit this places belongs to.
  const unsigned int unit;

  /// @brief Constructor.
  place(unsigned int id, unsigned int marking, unsigned int unit);

  /// @brief Pre transitions.
  std::multimap<unsigned int, unsigned int> pre;

  /// @brief Post transitions.
  std::multimap<unsigned int, unsigned int> post;

  /// @brief Tells if this place has an initial marking.
  bool
  initial() const noexcept;

  /// @brief Tell if this place is connected to one or more transitions.
  bool connected() const noexcept;
};

/*------------------------------------------------------------------------------------------------*/

/// @brief Compare two places using their ids.
bool
operator<(const place& left, const place& right);

/*------------------------------------------------------------------------------------------------*/

/// @brief Export a place to an output stream.
std::ostream&
operator<<(std::ostream& os, const place& p);

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::pn

#endif // _PNMC_PN_PLACE_HH_
