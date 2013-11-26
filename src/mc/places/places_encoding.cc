#include <cassert>
#include <chrono>
#include <deque>
#include <iostream>
#include <set>
#include <utility> // pair

#include "mc/places/post.hh"
#include "mc/places/post_live.hh"
#include "mc/places/pre.hh"
#include "mc/places/sdd.hh"
#include "mc/places/concurrent_units.hh"
#include "mc/places/places_encoding.hh"

namespace pnmc { namespace mc {

namespace chrono = std::chrono;

/*------------------------------------------------------------------------------------------------*/

struct mk_order_visitor
  : public boost::static_visitor<std::pair<std::string, sdd::order_builder<sdd_conf>>>
{
  using result_type = std::pair<order_identifier, sdd::order_builder<sdd_conf>>;

  const conf::pnmc_configuration& conf;
  mutable unsigned int artificial_id_counter;

  mk_order_visitor(const conf::pnmc_configuration& c)
    : conf(c), artificial_id_counter(0)
  {}

  // Place: base case of the recursion, there's no more possible nested hierarchies.
  result_type
  operator()(const pn::place* p)
  const noexcept
  {
    return std::make_pair(order_identifier(p->id), order_builder());
  }

  // Hierarchy.
  result_type
  operator()(const pn::module_node& m)
  const noexcept
  {
    assert(not m.nested.empty());

    std::deque<result_type> tmp;

    for (const auto& h : m.nested)
    {
      const auto res = boost::apply_visitor(*this, *h);
      tmp.push_back(res);
    }

    std::size_t height = 0;
    for (const auto& p : tmp)
    {
      if (not p.second.empty())
      {
        height += p.second.height();
      }
      else
      {
        height += 1; // place
      }
    }

    order_builder ob;
    if (height <= conf.order_min_height)
    {
      order_identifier id;
      for (const auto& p : tmp)
      {
        if (not p.second.empty())
        {
          ob = p.second << ob;
        }
        else // place
        {
          ob.push(p.first, p.second);
        }
      }
      return result_type(id, ob);
    }
    else
    {
      for (const auto& p : tmp)
      {
        ob.push(p.first, p.second);
      }
    }

    return std::make_pair(order_identifier(m.id) , ob);
  }
};

/*------------------------------------------------------------------------------------------------*/

order
mk_order(const conf::pnmc_configuration& conf, const pn::net& net)
{
  if (not conf.order_force_flat and net.modules)
  {
    return order(boost::apply_visitor(mk_order_visitor(conf), *net.modules).second);
  }
  else
  {
    order_builder ob;
    for (const auto& place : net.places())
    {
      ob.push(place.id);
    }
    return order(ob);
  }
}

/*------------------------------------------------------------------------------------------------*/

SDD
initial_state(const order& o, const pn::net& net)
{
  return SDD(o, [&](const std::string& id)
                        -> sdd::values::bitset<64>
                       {
                         return {net.places_by_id().find(id)->marking};
                       });
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

  for (const auto& transition : net.transitions())
  {
    homomorphism h_t = Id<sdd_conf>();

    if (not transition.post.empty())
    {
      // post actions.
      auto arc_cit = transition.post.begin();
      if (conf.compute_dead_transitions)
      {
        const auto f = ValuesFunction<sdd_conf>( o, arc_cit->first
                                               , post_live(transition.index, transitions_bitset));
        h_t = Composition(h_t, sdd::carrier(o, arc_cit->first, f));
      }
      else
      {
        homomorphism f = ValuesFunction<sdd_conf>(o, arc_cit->first, post());
        h_t = Composition(h_t, sdd::carrier(o, arc_cit->first, f));
      }
      for (++arc_cit; arc_cit != transition.post.end(); ++arc_cit)
      {
        homomorphism f = ValuesFunction<sdd_conf>(o, arc_cit->first, post());
        h_t = Composition(h_t, sdd::carrier(o, arc_cit->first, f));
      }
    }

    // pre actions.
    for (const auto& arc : transition.pre)
    {
      homomorphism f = ValuesFunction<sdd_conf>(o, arc.first, pre());
      h_t = Composition(h_t, sdd::carrier(o, arc.first, f));
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
  const auto res = sdd::rewrite(Fixpoint(Sum<sdd_conf>(o, operands.cbegin(), operands.cend())), o);
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

places_encoding::places_encoding(const conf::pnmc_configuration& c)
  : conf(c)
{}

void
places_encoding::operator()(const pn::net& net)
const
{
  auto manager = sdd::manager<sdd_conf>::init();

  boost::dynamic_bitset<> transitions_bitset(net.transitions().size());

  const order o = mk_order(conf, net);
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

  if (conf.compute_dead_transitions)
  {
    for (std::size_t i = 0; i < net.transitions().size(); ++i)
    {
      std::cout << (!transitions_bitset[i]) << "\n";
    }
  }

  const auto n = sdd::count_combinations(m);
  std::cerr << n.template convert_to<long double>() << " states" << std::endl;

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

}} // namespace pnmc::mc
