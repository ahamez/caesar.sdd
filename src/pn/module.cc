#include "pn/module.hh"

namespace pnmc { namespace pn {

/*------------------------------------------------------------------------------------------------*/

module_node::module_node(unsigned int id)
  : nested(), id(id)
{}

/*------------------------------------------------------------------------------------------------*/

void
module_node::add_module(const module& m)
{
  nested.push_back(m);
}

/*------------------------------------------------------------------------------------------------*/

module
make_module(const place& p)
{
  return std::make_shared<boost::variant<module_node, const place*>>(&p);
}

/*------------------------------------------------------------------------------------------------*/

module
make_module(const module_node& m)
{
  return std::make_shared<boost::variant<module_node, const place*>>(m);
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::pn
