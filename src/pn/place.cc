#include <ostream>

#include "pn/place.hh"

namespace pnmc { namespace pn {

/*------------------------------------------------------------------------------------------------*/

place::place(unsigned int id, unsigned int m, unsigned int u)
  : id(id), marking(m), unit(u)
{}

/*------------------------------------------------------------------------------------------------*/

bool
operator<(const pn::place& lhs, const pn::place& rhs)
{
  return lhs.id < rhs.id;
}

/*------------------------------------------------------------------------------------------------*/

std::ostream&
operator<<(std::ostream& os, const pn::place& p)
{
  return os << "pl " << p.id  <<  " (" << p.marking << ") @" << p.unit;
}

/*------------------------------------------------------------------------------------------------*/

}} // pnmc::pn
