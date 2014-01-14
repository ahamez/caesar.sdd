#include "pn/net.hh"
#include "pn/place.hh"
#include "pn/transition.hh"

namespace pnmc { namespace pn {

/*------------------------------------------------------------------------------------------------*/

/// @brief Used by Boost.MultiIndex.
struct add_post_place_to_transition
{
  const unsigned int new_valuation;
  const unsigned int new_place_id;

  add_post_place_to_transition(unsigned int valuation, unsigned int id)
	  : new_valuation(valuation), new_place_id(id)
  {}

  void
  operator()(transition& t)
  const
  {
    t.post.insert(std::make_pair(new_place_id , new_valuation));
  }
};

/*------------------------------------------------------------------------------------------------*/

/// @brief Used by Boost.MultiIndex.
struct add_pre_place_to_transition
{
  const unsigned int new_valuation;
  const unsigned int new_place_id;

  add_pre_place_to_transition(unsigned int valuation, unsigned int id)
  	: new_valuation(valuation), new_place_id(id)
  {}

  void
  operator()(transition& t)
  const
  {
    t.pre.insert(std::make_pair(new_place_id, new_valuation));
  }
};

/*------------------------------------------------------------------------------------------------*/

/// @brief Used by Boost.MultiIndex.
struct update_place_helper
{
  const unsigned int marking;

  update_place_helper(unsigned int m)
  	: marking(m)
  {}

  void
  operator()(place& p)
  const
  {
    p.marking = marking;
  }
};

/*------------------------------------------------------------------------------------------------*/

net::net()
  : places_(), transitions_(), root_unit(), initial_place()
{}

/*------------------------------------------------------------------------------------------------*/

const place&
net::add_place(unsigned int pid, unsigned int marking, unsigned int unit)
{
  assert(places_by_id().find(pid) == places_by_id().cend());
  return *places_.get<insertion_index>().push_back({pid, marking, unit}).first;
}

/*------------------------------------------------------------------------------------------------*/

void
net::update_place(unsigned int id, unsigned int marking)
{
  const auto cit = places_by_id().find(id);
  assert(cit != places_by_id().cend());
  places_.get<id_index>().modify(cit, update_place_helper(marking));
}

/*------------------------------------------------------------------------------------------------*/

const transition&
net::add_transition(unsigned int id)
{
  const auto cit = transitions_.get<id_index>().find(id);
  if (cit == transitions_.get<id_index>().cend())
  {
    return *transitions_.get<id_index>().insert(transition(id)).first;
  }
  else
  {
    return *cit;
  }
}

/*------------------------------------------------------------------------------------------------*/

void
net::add_post_place(unsigned int id, unsigned int post, unsigned int valuation)
{
  assert(places_by_id().find(post) != places_by_id().end());
  const auto it = transitions_.get<id_index>().find(id);
  transitions_.modify(it, add_post_place_to_transition(valuation, post));
}

/*------------------------------------------------------------------------------------------------*/

void
net::add_pre_place(unsigned int id, unsigned int pre, unsigned int valuation)
{
  assert(places_by_id().find(pre) != places_by_id().end());
  const auto it = transitions_.get<id_index>().find(id);
  transitions_.modify(it, add_pre_place_to_transition(valuation, pre));
}

/*------------------------------------------------------------------------------------------------*/

const net::places_type::index<net::insertion_index>::type&
net::places()
const noexcept
{
  return places_.get<insertion_index>();
}

/*------------------------------------------------------------------------------------------------*/

const net::places_type::index<net::id_index>::type&
net::places_by_id()
const noexcept
{
  return places_.get<id_index>();
}

/*------------------------------------------------------------------------------------------------*/

std::vector<std::reference_wrapper<const place>>
net::places_of_unit(unsigned int i)
const
{
  const auto p = places_.get<unit_index>().equal_range(i);
  return std::vector<std::reference_wrapper<const place>>(p.first, p.second);
}

/*------------------------------------------------------------------------------------------------*/

unsigned int
net::unit_of_place(unsigned int place)
const
{
  return places_.get<id_index>().find(place)->unit;
}

/*------------------------------------------------------------------------------------------------*/

std::size_t
net::units_size()
const noexcept
{
  const auto& places = places_.get<unit_index>();
  return places.empty() ? 0 : std::prev(places_.get<unit_index>().end())->unit + 1;
}

/*------------------------------------------------------------------------------------------------*/

const net::transitions_type::index<net::id_index>::type&
net::transitions()
const noexcept
{
  return transitions_.get<id_index>();
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::pn
