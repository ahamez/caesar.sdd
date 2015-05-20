#pragma once

#include <exception>
#include <string>

namespace pnmc { namespace mc { namespace units {

/*------------------------------------------------------------------------------------------------*/

struct bound_error final
  : public std::exception
{
  const unsigned int place;

  bound_error(unsigned int p)
    : place(p)
  {}
};

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::units
