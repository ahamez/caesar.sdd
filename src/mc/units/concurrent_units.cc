#include <chrono>
#include <ostream>
#include <unordered_set>

#include <sdd/values/bitset.hh>

#include "mc/units/concurrent_units.hh"

namespace pnmc { namespace mc { namespace units {

/*------------------------------------------------------------------------------------------------*/

namespace /* anonymous */{

struct nested_query_visitor
{
  using result_type = bool;

  const sdd::values::flat_set<unsigned int>& empty;
  const unsigned int j;
  mutable std::unordered_set<const sdd::flat_node<sdd_conf>*> cache;

  nested_query_visitor(const sdd::values::flat_set<unsigned int>& empty, unsigned int j)
    : empty(empty), j(j), cache()
  {}

  bool
  operator()(const sdd::zero_terminal<sdd_conf>&, const order&)
  const noexcept
  {
    assert(false && "|0|");
    __builtin_unreachable();
  }

  bool
  operator()(const sdd::one_terminal<sdd_conf>&, const order&)
  const noexcept
  {
    assert(false && "|1|");
    __builtin_unreachable();
  }

  bool
  operator()(const sdd::hierarchical_node<sdd_conf>&, const order&)
  const noexcept
  {
    assert(false && "Hierarchy");
    __builtin_unreachable();
  }

  bool
  operator()(const sdd::flat_node<sdd_conf>& node, const order& o)
  const noexcept
  {
    const auto insertion = cache.emplace(&node);
    if (insertion.second)
    {
      if (o.identifier().user() == j)
      {
        for (const auto& arc : node)
        {
          if (arc.valuation() != empty)
          {
            return true;
          }
        }
      }
      else
      {
        for (const auto& arc : node)
        {
          if (visit(*this, arc.successor(), o.next()))
          {
            return true;
          }
        }
      }
    }
    return false;
  }
};

struct query_visitor
{
  using result_type = bool;

  const sdd::values::flat_set<unsigned int>& empty;
  const unsigned int i;
  const unsigned int j;
  mutable std::unordered_set<const sdd::flat_node<sdd_conf>*> cache;

  query_visitor(const sdd::values::flat_set<unsigned int>& empty, unsigned int i, unsigned int j)
    : empty(empty), i(i), j(j), cache()
  {}

  bool
  operator()(const sdd::zero_terminal<sdd_conf>&, const order&)
  const noexcept
  {
    assert(false && "|0|");
    __builtin_unreachable();
  }

  bool
  operator()(const sdd::one_terminal<sdd_conf>&, const order&)
  const noexcept
  {
    assert(false && "|1|");
    __builtin_unreachable();
  }

  bool
  operator()(const sdd::hierarchical_node<sdd_conf>&, const order&)
  const noexcept
  {
    assert(false && "Hierarchy");
    __builtin_unreachable();
  }

  bool
  operator()(const sdd::flat_node<sdd_conf>& node, const order& o)
  const
  {
    const auto insertion = cache.emplace(&node);
    if (insertion.second)
    {
      if (o.identifier().user() == i)
      {
        for (const auto& arc : node)
        {
          if (arc.valuation() != empty)
          {
            if (visit(nested_query_visitor(empty, j), arc.successor(), o.next()))
            {
              return true;
            }
          }
        }
      }
      else
      {
        for (const auto& arc : node)
        {
          if (visit(*this, arc.successor(), o.next()))
          {
            return true;
          }
        }
      }
    }
    return false;
  }
};

bool
query(SDD states, const order& o, unsigned int i, unsigned int j)
{
  const sdd::values::flat_set<unsigned int> empty_unit = {0};
  if (o.node(i) < o.node(j)) // does 'i' is before 'j' in the order?
  {
    return visit(query_visitor(empty_unit, i, j), states, o);
  }
  else
  {
    return visit(query_visitor(empty_unit, j, i), states, o);
  }
}

} // namespace anonymous

/*------------------------------------------------------------------------------------------------*/

void
compute_concurrent_units( const conf::pnmc_configuration& conf, const pn::net& net
                        , const sdd::order<sdd_conf>& o, SDD states)
{
  namespace chrono = std::chrono;
  chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();

  const auto nb_units = net.units_size();
  unsigned int j_end = 1;
  for (unsigned int i = 0; i < nb_units; ++i, ++j_end)
  {
    for (unsigned int j = 0; j < j_end; ++j)
    {
      if (i == j or net.places_of_unit(i).empty() or net.places_of_unit(j).empty())
      {
        std::cout << "0 ";
      }
      else
      {
        std::cout << (query(states, o, i, j) ? "1 " : "0 ");
      }
    }
    std::cout << "\n";
  }

  chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
  const std::size_t elapsed = chrono::duration_cast<chrono::seconds>(end-start).count();
  if (conf.show_time)
  {
    std::cout << "Concurrent units computation time: " << elapsed << "s\n";
  }
}

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::units
