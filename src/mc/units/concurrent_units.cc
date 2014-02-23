#include <algorithm> // copy_if, sort
#include <chrono>
#include <ostream>
#include <unordered_map>
#include <utility>  //pair
#include <vector>

#include <sdd/values/bitset.hh>

#include "mc/units/concurrent_units.hh"

namespace pnmc { namespace mc { namespace units {

/*------------------------------------------------------------------------------------------------*/

namespace /* anonymous */{

struct active
{
  bool i;
  bool j;

  active(bool i, bool j)
    : i(i), j(j)
  {}

  bool
  all()
  const
  {
    return i and j;
  }
};

template <typename InputIterator>
struct query_visitor
{
  using result_type = active;

  const sdd::values::flat_set<unsigned int>& inactive_unit;

  const InputIterator i_units_end;
  const InputIterator j_units_end;

  mutable std::unordered_map<const sdd::flat_node<sdd_conf>*, active> cache;

  query_visitor( const sdd::values::flat_set<unsigned int>& empty
               , InputIterator i_units_end, InputIterator j_units_end)
    : inactive_unit(empty)
    , i_units_end(i_units_end), j_units_end(j_units_end)
    , cache()
  {}

  result_type
  operator()( const sdd::zero_terminal<sdd_conf>&, const order&
            , InputIterator, InputIterator, bool, bool)
  const noexcept
  {
    assert(false && "|0|");
    __builtin_unreachable();
  }

  result_type
  operator()( const sdd::one_terminal<sdd_conf>&, const order&
            , InputIterator, InputIterator, bool, bool)
  const noexcept
  {
    return active(false, false);
  }

  result_type
  operator()( const sdd::hierarchical_node<sdd_conf>&, const order&
            , InputIterator, InputIterator, bool, bool)
  const noexcept
  {
    assert(false && "Hierarchy");
    __builtin_unreachable();
  }

  result_type
  operator()( const sdd::flat_node<sdd_conf>& node, const order& o
            , InputIterator i_units_cit, InputIterator j_units_cit, bool i_active, bool j_active)
  const
  {
    assert((i_active ? not j_active : true) and (j_active ? not i_active : true));

    const auto insertion = cache.emplace(&node, active(false, false));
    auto& cache_result = insertion.first->second;

    if (insertion.second) // It's the first time that we see this node.
    {
      if (i_active) // only i or its subunits are active
      {
        assert(i_units_cit == i_units_end);
        if (on(j_units_cit, o)) // on j or its subunits
        {
          for (const auto& arc : node)
          {
            if (arc.valuation() != inactive_unit) // j is active
            {
              return active(true, true);
            }
            else // j is not active at this level
            {
              cache_result = visit( *this, arc.successor(), o.next()
                                  , i_units_end, ++j_units_cit, true, false);
              if (cache_result.all())
              {
                return cache_result;
              }
            }
          }
        }
        else // not on j or its subunits
        {
          for (const auto& arc : node)
          {
            cache_result = visit( *this, arc.successor(), o.next()
                                , i_units_end, j_units_cit, true, false);
            if (cache_result.all())
            {
              return cache_result;
            }
          }
        }
      }
      /* --------------- */
      else if (j_active)  // only j or its subunits are active
      {
        assert(j_units_cit == j_units_end);
        if (on(i_units_cit, o)) // on i or its subunits
        {
          for (const auto& arc : node)
          {
            if (arc.valuation() != inactive_unit) // j is active
            {
              return active(true, true);
            }
            else // i is not active at this level
            {
              cache_result = visit( *this, arc.successor(), o.next()
                                  , ++i_units_cit, j_units_end, false, true);
              if (cache_result.all())
              {
                return cache_result;
              }
            }
          }
        }
        else // not on i or its subunits
        {
          for (const auto& arc : node)
          {
            cache_result = visit( *this, arc.successor(), o.next()
                                , i_units_cit, j_units_end, false, true);
            if (cache_result.all())
            {
              return cache_result;
            }
          }
        }
      }
      /* --------------- */
      else // neither i (or its subunits) nor j (or its subunits) are active
      {
        if (on(i_units_cit, o)) // on i or its subunits
        {
          for (const auto& arc : node)
          {
            if (arc.valuation() != inactive_unit) // i is active
            {
              cache_result = visit( *this, arc.successor(), o.next()
                                  , i_units_end, j_units_cit, true, false);
              if (cache_result.all())
              {
                return cache_result;
              }
            }
            else // i is not active at this level
            {
              cache_result = visit( *this, arc.successor(), o.next()
                                  , ++i_units_cit, j_units_cit, false, false);
              if (cache_result.all())
              {
                return cache_result;
              }
            }
          }
        }
        else if (on(j_units_cit, o))
        {
          for (const auto& arc : node)
          {
            if (arc.valuation() != inactive_unit) // j is active
            {
              cache_result = visit( *this, arc.successor(), o.next()
                                  , i_units_cit, j_units_end, false, true);
              if (cache_result.all())
              {
                return cache_result;
              }
            }
            else // j is not active at this level
            {
              cache_result = visit( *this, arc.successor(), o.next()
                                  , i_units_cit, ++j_units_cit, false, false);
              if (cache_result.all())
              {
                return cache_result;
              }
            }
          }
        }
        else // neither on i (or its subunits) nor j (or its subunits)
        {
          for (const auto& arc : node)
          {
            cache_result = visit( *this, arc.successor(), o.next()
                                , i_units_cit, j_units_cit, false, false);
            if (cache_result.all())
            {
              return cache_result;
            }
          }
        }
      }
    }
    // Cache hit or not, the result is now in the cache.
    return cache_result;
  }

  static
  bool
  on(InputIterator cit, const order& o)
  {
    // Units identifiers are sorted by their order of apparition in the SDD.
    // So we can safely check the head of units identifiers.
    return o.identifier().user() == *cit;
  }
};

bool
query(const pn::net& net, SDD states, const order& o, unsigned int i, unsigned int j)
{
  const sdd::values::flat_set<unsigned int> empty_unit = {0};

  const auto i_search = net.units().find(i);
  const auto j_search = net.units().find(j);
  assert(i_search != net.units().end());
  assert(j_search != net.units().end());
  const auto& i_unit = i_search->second;
  const auto& j_unit = j_search->second;

  const auto pred = [&net](unsigned int id){return not net.places_of_unit(id).empty();};

  // Copy all subunits of j and i itself
  std::vector<unsigned int> i_units;
  i_units.reserve(i_unit.subunits.size() + 1);
  std::copy_if(i_unit.subunits.cbegin(), i_unit.subunits.cend(), std::back_inserter(i_units), pred);
  i_units.push_back(i);

  // Copy all subunits of j and j itself.
  std::vector<unsigned int> j_units;
  j_units.reserve(j_unit.subunits.size() + 1);
  std::copy_if(j_unit.subunits.cbegin(), j_unit.subunits.cend(), std::back_inserter(j_units), pred);
  j_units.push_back(j);

  // Sort unit ids by their order of apparition in the SDD.
  const auto comp = [&o](unsigned int lhs, unsigned int rhs){return o.node(lhs) < o.node(rhs);};
  std::sort(i_units.begin(), i_units.end(), comp);
  std::sort(j_units.begin(), j_units.end(), comp);

  const auto res
    = visit( query_visitor<std::vector<unsigned int>::const_iterator>
               (empty_unit, i_units.cend(), j_units.cend())
           , states, o, i_units.cbegin(), j_units.cbegin(), false, false);

  return res.i and res.j;
}

} // namespace anonymous

/*------------------------------------------------------------------------------------------------*/

void
compute_concurrent_units( const conf::pnmc_configuration& conf, const pn::net& net
                        , const sdd::order<sdd_conf>& o, SDD states)
{
  namespace chrono = std::chrono;
  chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();

  const auto nb_units = net.units().size();
  unsigned int j_end = 1;
  for (unsigned int i = 0; i < nb_units; ++i, ++j_end)
  {
    for (unsigned int j = 0; j < j_end; ++j)
    {
      if ( i == j or net.places_of_unit(i).empty() or net.places_of_unit(j).empty()
          or net.units_are_related(i, j))
      {
        std::cout << "0 ";
      }
      else
      {
        std::cout << (query(net, states, o, i, j) ? "1 " : "0 ");
      }
    }
    std::cout << std::endl;
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
