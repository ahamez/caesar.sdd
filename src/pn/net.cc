#include "pn/net.hh"
#include "pn/place.hh"
#include "pn/transition.hh"

namespace pnmc { namespace pn {

namespace /* anonymous */ {

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

} // namespace anonymous

/*------------------------------------------------------------------------------------------------*/

net::net(unsigned int p, unsigned int r)
  : places_(), transitions_(), units_(), initial_place_(p), root_unit_(r)
{}

/*------------------------------------------------------------------------------------------------*/

unsigned int
net::initial_place()
const noexcept
{
  return initial_place_;
}

/*------------------------------------------------------------------------------------------------*/

unsigned int
net::root_unit()
const noexcept
{
  return root_unit_;
}

/*------------------------------------------------------------------------------------------------*/

/// @brief Return all units.
std::unordered_map<unsigned int, const unit>&
net::units()
noexcept
{
  return units_;
}

/*------------------------------------------------------------------------------------------------*/

/// @brief Return all units.
const std::unordered_map<unsigned int, const unit>&
net::units()
const noexcept
{
  return units_;
}

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
  std::vector<std::reference_wrapper<const place>> res(p.first, p.second);
  std::sort(res.begin(), res.end());
  return res;
}

/*------------------------------------------------------------------------------------------------*/

unsigned int
net::unit_of_place(unsigned int place)
const
{
  return places_.get<id_index>().find(place)->unit;
}

/*------------------------------------------------------------------------------------------------*/

unsigned int
net::unit_of_place(const place& p)
const
{
  return places_.get<id_index>().find(p.id)->unit;
}

/*------------------------------------------------------------------------------------------------*/

bool
net::units_are_related(unsigned int i, unsigned int j)
const
{
  const auto i_search = units_.find(i);
  const auto j_search = units_.find(j);
  assert(i_search != units_.end());
  assert(j_search != units_.end());
  const auto& i_unit = i_search->second;
  const auto& j_unit = j_search->second;
  if (std::find(i_unit.path.cbegin(), i_unit.path.cend(), j) !=  i_unit.path.cend())
  {
    return true;
  }
  else if (std::find(j_unit.path.cbegin(), j_unit.path.cend(), i) != j_unit.path.cend())
  {
    return true;
  }
  return false;
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
