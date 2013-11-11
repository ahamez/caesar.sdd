#ifndef _PNMC_PN_PARSERS_HELPERS_HH_
#define _PNMC_PN_PARSERS_HELPERS_HH_

#include <istream>
#include <string>
#include <vector>

#include "parsers/helpers.hh"

namespace pnmc { namespace parsers {

/*------------------------------------------------------------------------------------------------*/

/// @brief Input stream manipulator to detect a keyword.
struct kw
{
  const std::string k_;
  
  kw(const std::string& k);

  friend
  std::istream&
  operator>>(std::istream& in, const kw& manip);
};

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::parsers

#endif // _PNMC_PN_PARSERS_HELPERS_HH_
