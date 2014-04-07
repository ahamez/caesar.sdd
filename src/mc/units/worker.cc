#include <cassert>
#include <chrono>
#include <deque>
#include <iostream>
#include <set>
#include <unordered_set>
#include <utility> // pair

#include <boost/dynamic_bitset.hpp>
#include <sdd/order/strategies/force.hh>

#include "mc/units/concurrent_units.hh"
#include "mc/units/nopost_live.hh"
#include "mc/units/post.hh"
#include "mc/units/post_live.hh"
#include "mc/units/pre.hh"
#include "mc/units/sdd.hh"
#include "mc/units/worker.hh"


namespace pnmc { namespace mc { namespace units {

namespace chrono = std::chrono;

/*------------------------------------------------------------------------------------------------*/

order
mk_order(const conf::pnmc_configuration& conf, const pn::net& net)
{
  std::vector<unsigned int> units;
  units.reserve(net.units().size());

  std::unordered_set<unsigned int> units_added;
  units_added.reserve(net.units().size());

  // Iteration on places because we want to put units in the same order as the places they
  // contain.
  for (const auto& p : net.places())
  {
    if (not net.places_of_unit(p.unit).empty()) // Don't add empty units.
    {
      if (units_added.emplace(p.unit).second) // Check if unit has already been added.
      {
        units.push_back(p.unit);
      }
    }
  }

  if (not conf.order_force)
  {
    return {order_builder(units.begin(), units.end())};
  }
  else
  {
    // The hypergraph that stores connections between the places of the Petri net.
    sdd::force::hypergraph<sdd_conf> graph(units.cbegin(), units.cend());

    std::vector<unsigned int> identifiers;
    std::unordered_set<unsigned int> identifiers_added;
    for (const auto& transition : net.transitions())
    {
//    // First check if we should add this transition.
//    // We don't care about transitions that only initialize the Petri net.
//    bool add_transition = true;
//    for (const auto& arc : transition.pre)
//    {
//      assert(net.places().find(arc.first) != net.places().end());
//      const auto& place = *net.places().find(arc.first);
//      /// @todo Check that the place has no input transitions.
//      if (not place.initial())
//      {
//        add_transition = false;
//        break;
//      }
//    }
//
//    if (not add_transition)
//    {
//      continue;
//    }
      for (const auto& arc : transition.pre)
      {
        const auto u = net.unit_of_place(arc.first);
        if (identifiers_added.emplace(arc.first).second)
        {
          identifiers.push_back(u);
        }
      }
      
      for (const auto& arc : transition.post)
      {
        const auto u = net.unit_of_place(arc.first);
        if (identifiers_added.emplace(arc.first).second)
        {
          identifiers.push_back(u);
        }
      }
      graph.add_hyperedge(identifiers.cbegin(), identifiers.cend());
      
      // We use this container again in the next loop.
      identifiers.clear();
      identifiers_added.clear();
    }
    // Apply the FORCE ordering strategy.
    auto force = sdd::force::worker<sdd_conf>(graph);
    return force();
  }
}

/*------------------------------------------------------------------------------------------------*/

SDD
initial_state(const order& o, const pn::net& net)
{
  using flat_set = sdd::values::flat_set<unsigned int>;
  return SDD(o, [&](unsigned int unit) -> flat_set
                   {
                     std::vector<unsigned int> marked_places;
                     for (const auto& p : net.places_of_unit(unit))
                     {
                       if (p.get().initial())
                       {
                         marked_places.push_back(p.get().id);
                       }
                     }
                     if (marked_places.size() == 0)
                     {
                       return flat_set({0});
                     }
                     else if (marked_places.size() == 1)
                     {
                       return flat_set({marked_places[0] + 1});
                     }
                     else
                     {
                       throw std::runtime_error("More that one initial place in an unit.");
                     }
                   }
            );
}

/*------------------------------------------------------------------------------------------------*/

homomorphism
transition_relation( const conf::pnmc_configuration& conf, const order& o
                   , const pn::net& net, boost::dynamic_bitset<>& transitions_bitset)
{
  chrono::time_point<chrono::system_clock> start;
  chrono::time_point<chrono::system_clock> end;
  std::size_t elapsed;

  start = chrono::system_clock::now();
  std::set<homomorphism> operands;
  operands.insert(id<sdd_conf>());

  // When computing dead transitions, we need to start numbering them from 0 as we use this
  // value as an index in the bitset.
  unsigned int tindex = 0;

  for (const auto& transition : net.transitions())
  {
    homomorphism h_t = id<sdd_conf>();

    if (transition.post.empty() and transition.pre.empty() and conf.compute_dead_transitions)
    {
      // Empty transition, but we need to increment the transition counter.
      ++tindex;
    }
    else if (not transition.post.empty())
    {
      // post actions.
      auto arc_cit = transition.post.begin();
      if (conf.compute_dead_transitions)
      {
        const auto f
          = function( o, net.unit_of_place(arc_cit->first)
                                    , post_live(arc_cit->first, tindex++, transitions_bitset));
        h_t = composition(h_t, sdd::carrier(o, net.unit_of_place(arc_cit->first), f));
      }
      else
      {
        homomorphism f
          = function(o, net.unit_of_place(arc_cit->first), post(arc_cit->first));
        h_t = composition(h_t, sdd::carrier(o, net.unit_of_place(arc_cit->first), f));
      }
      for (++arc_cit; arc_cit != transition.post.end(); ++arc_cit)
      {
        homomorphism f
          = function(o, net.unit_of_place(arc_cit->first), post(arc_cit->first));
        h_t = composition(h_t, sdd::carrier(o, net.unit_of_place(arc_cit->first), f));
      }
    }
    else if (conf.compute_dead_transitions)
    {
      // Target the same variable as the pre that is fired just before this fake post.
      const auto unit = transition.pre.begin()->first;

      const auto f = function(o, unit, nopost_live(tindex++, transitions_bitset));
      h_t = composition(h_t, sdd::carrier(o, unit, f));
    }

    // pre actions.
    for (const auto& arc : transition.pre)
    {
      homomorphism f = function(o, net.unit_of_place(arc.first), pre(arc.first));
      h_t = composition(h_t, sdd::carrier(o, net.unit_of_place(arc.first), f));
    }

    operands.insert(h_t);
  }
  end = chrono::system_clock::now();
  elapsed = chrono::duration_cast<chrono::seconds>(end-start).count();

  if (conf.show_time)
  {
    std::cout << "Transition relation time: " << elapsed << "s" << std::endl;
  }

  start = chrono::system_clock::now();
  const auto res = sdd::rewrite(o, fixpoint(sum(o, operands.cbegin(), operands.cend())));
  end = chrono::system_clock::now();
  elapsed = chrono::duration_cast<chrono::seconds>(end-start).count();

  if (conf.show_time)
  {
    std::cout << "Rewrite time: " << elapsed << "s" << std::endl;
  }

  return res;
}

/*------------------------------------------------------------------------------------------------*/

SDD
state_space( const conf::pnmc_configuration& conf, const order& o, SDD m
           , homomorphism h)
{
  chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
  const auto res = h(o, m);
  chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
  const std::size_t elapsed = chrono::duration_cast<chrono::seconds>(end-start).count();
  if (conf.show_time)
  {
    std::cout << "State space computation time: " << elapsed << "s" << std::endl;
  }
  return res;
}

/*------------------------------------------------------------------------------------------------*/

worker::worker(const conf::pnmc_configuration& c)
  : conf(c)
{}

/*------------------------------------------------------------------------------------------------*/

void
worker::operator()(const pn::net& net)
const
{
  auto manager = sdd::manager<sdd_conf>::init();
  boost::dynamic_bitset<> transitions_bitset(net.transitions().size());

  const auto o = mk_order(conf, net);
  assert(not o.empty() && "Empty order");

  if (conf.order_show)
  {
    std::cout << o << std::endl;
  }

  const SDD m0 = initial_state(o, net);
  const homomorphism h = transition_relation(conf, o, net, transitions_bitset);

  const SDD m = state_space(conf, o, m0, h);

  if (conf.show_nb_states)
  {
    std::cout << m.size().template convert_to<long double>() << " states" << std::endl;
  }

  if (conf.compute_dead_transitions)
  {
    for (std::size_t i = 0; i < net.transitions().size(); ++i)
    {
      std::cout << (!transitions_bitset[i]) << "\n";
    }
  }

  if (conf.compute_concurrent_units)
  {
    compute_concurrent_units(conf, net, o, m);
  }
}

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::units
