#pragma once

#include <functional> // hash
#include <iosfwd>

#include "sdd/values/flat_set.hh"
#include "sdd/values_manager.hh"

namespace pnmc { namespace mc { namespace units {

/*------------------------------------------------------------------------------------------------*/

struct pre
{
  unsigned int place;

  pre(unsigned int);

  sdd::values::flat_set<unsigned int>
  operator()(const sdd::values::flat_set<unsigned int>&) const noexcept;
};

/// @brief Equality of two pre.
bool
operator==(const pre&, const pre&) noexcept;

/// @brief Textual output of a pre.
std::ostream&
operator<<(std::ostream&, const pre&);

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::units

namespace std {

/*------------------------------------------------------------------------------------------------*/

template <>
struct hash<pnmc::mc::units::pre>
{
  std::size_t operator()(const pnmc::mc::units::pre&) const noexcept;
};

/*------------------------------------------------------------------------------------------------*/

} // namespace std
