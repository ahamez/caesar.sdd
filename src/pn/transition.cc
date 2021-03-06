#include <ostream>

#include "pn/transition.hh"

namespace caesar { namespace pn {

/*------------------------------------------------------------------------------------------------*/

transition::transition(unsigned int id)
  : id(id), pre(), post()
{}

/*------------------------------------------------------------------------------------------------*/

/// Compare two transitions using their ids.
bool
operator<(const transition& left, const transition& right)
noexcept
{
  return left.id < right.id;
}

/*------------------------------------------------------------------------------------------------*/

/// Export a transition to an output stream.
std::ostream&
operator<<(std::ostream& os, const transition& t)
{
  os << "tr " << t.id;
  for(auto p : t.pre)
  {
    os << " (" << p.first << ", " << p.second << ")";
  }
  os << " -> ";
  for(auto p : t.post)
  {
    os << " (" << p.first << ", " << p.second << ")";
  }
  return os;
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace caesar::pn
