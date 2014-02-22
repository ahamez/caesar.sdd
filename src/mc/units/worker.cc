#include <cassert>
#include <chrono>
#include <deque>
#include <iostream>
#include <set>
#include <utility> // pair

#include <boost/dynamic_bitset.hpp>

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
mk_order(const pn::net& net)
{
  std::vector<unsigned int> units;
  units.reserve(net.units().size());
  for (const auto& p : net.places())
  {
    if (not net.places_of_unit(p.unit).empty())
    {
      if (units.empty() or units.back() != p.unit)
      {
        units.push_back(p.unit);
      }
    }
  }
  return {order_builder(units.begin(), units.end())};
}

/*------------------------------------------------------------------------------------------------*/

SDD
initial_state(const order& o, const pn::net& net)
{
  using flat_set = sdd::values::flat_set<unsigned int>;
  return SDD(o, [&](unsigned int unit)
                   -> flat_set
                   {
                     return unit == net.root_unit() ? flat_set({net.initial_place() + 1})
                                                    : flat_set({0});
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
  operands.insert(Id<sdd_conf>());

  // When computing dead transitions, we need to start numbering them from 0 as we use this
  // value as an index in the bitset.
  unsigned int tindex = 0;

  for (const auto& transition : net.transitions())
  {
    homomorphism h_t = Id<sdd_conf>();

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
          = ValuesFunction<sdd_conf>( o, net.unit_of_place(arc_cit->first)
                                    , post_live(arc_cit->first, tindex++, transitions_bitset));
        h_t = Composition(h_t, sdd::carrier(o, net.unit_of_place(arc_cit->first), f));
      }
      else
      {
        homomorphism f
          = ValuesFunction<sdd_conf>(o, net.unit_of_place(arc_cit->first), post(arc_cit->first));
        h_t = Composition(h_t, sdd::carrier(o, net.unit_of_place(arc_cit->first), f));
      }
      for (++arc_cit; arc_cit != transition.post.end(); ++arc_cit)
      {
        homomorphism f
          = ValuesFunction<sdd_conf>(o, net.unit_of_place(arc_cit->first), post(arc_cit->first));
        h_t = Composition(h_t, sdd::carrier(o, net.unit_of_place(arc_cit->first), f));
      }
    }
    else if (conf.compute_dead_transitions)
    {
      // Target the same variable as the pre that is fired just before this fake post.
      const auto unit = transition.pre.begin()->first;

      const auto f
        = ValuesFunction<sdd_conf>( o, unit
                                  , nopost_live(tindex++, transitions_bitset));
      h_t = Composition(h_t, sdd::carrier(o, unit, f));
    }

    // pre actions.
    for (const auto& arc : transition.pre)
    {
      homomorphism f = ValuesFunction<sdd_conf>(o, net.unit_of_place(arc.first), pre(arc.first));
      h_t = Composition(h_t, sdd::carrier(o, net.unit_of_place(arc.first), f));
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
  const auto res = sdd::rewrite(o, Fixpoint(Sum<sdd_conf>(o, operands.cbegin(), operands.cend())));
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

  const auto o = mk_order(net);
  assert(not o.empty() && "Empty order");

  if (conf.order_show)
  {
    std::cout << o << std::endl;
  }

  const SDD m0 = initial_state(o, net);
  const homomorphism h = transition_relation(conf, o, net, transitions_bitset);

  if (conf.show_relation)
  {
    std::cout << h << std::endl;
  }

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

  if (conf.show_hash_tables_stats)
  {
    std::cout << manager << std::endl;
  }
}

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::units
