#include <algorithm>  // any_of
#include <functional> // reference_wrapper
#include <iostream>
#include <map>
#include <string>
#include <tuple>

#include "parsers/bpn.hh"
#include "parsers/parse_error.hh"
#include "pn/unit.hh"

namespace pnmc { namespace parsers { namespace /* anonymous */ {

/*------------------------------------------------------------------------------------------------*/

struct uint
{
  unsigned int& res_;

  uint(unsigned int& res)
    : res_(res)
  {}

  friend
  std::istream&
  operator>>(std::istream& in, const uint& manip)
  {
    std::string s;
    if (not (in >> s))
    {
      throw parse_error("Expected a value");
    }
    try
    {
      manip.res_ = std::stoi(s);
    }
    catch (const std::invalid_argument&)
    {
      throw parse_error("Expected a value, got " + s);
    }
    return in;
  }
};

/*------------------------------------------------------------------------------------------------*/

struct sharp
{
  unsigned int* res_;

  sharp(unsigned int& res)
    : res_(&res)
  {}

  sharp()
    : res_(nullptr)
  {}

  friend
  std::istream&
  operator>>(std::istream& in, const sharp& manip)
  {
    std::string s;
    if (not (in >> s))
    {
      throw parse_error("Expected a #value");
    }
    try
    {
      if (s[0] != '#')
      {
        throw parse_error("Expected a #value, got " + s);
      }
      const auto res = std::stoi(s.substr(1));
      if (manip.res_)
      {
        *manip.res_ = res;
      }
    }
    catch (const std::invalid_argument&)
    {
      throw parse_error("Expected a #value, got " + s);
    }
    return in;
  }
};

/*------------------------------------------------------------------------------------------------*/

struct kw
{
  const std::string keyword;

  kw(const std::string& k)
    : keyword(k)
  {}

  friend
  std::istream&
  operator>>(std::istream& in, const kw& manip)
  {
    std::string s;
    if (not (in >> s))
    {
      throw parse_error("Expected " + manip.keyword + ", got nothing.");
    }
    else if (s != manip.keyword)
    {
      throw parse_error("Expected " + manip.keyword + ", got " + s);
    }
    return in;
  }
};

/*------------------------------------------------------------------------------------------------*/

struct interval
{
  unsigned int* first_;
  unsigned int* last_;

  interval(unsigned int& first, unsigned int& last)
    : first_(&first), last_(&last)
  {}

  interval()
    : first_(nullptr), last_(nullptr)
  {}

  friend
  std::istream&
  operator>>(std::istream& in, const interval& manip)
  {
    unsigned int first, last;
    std::size_t pos;

    std::string s;
    if (not (in >> s))
    {
      throw parse_error("Expected an interval");
    }

    try
    {
      first = std::stoi(s, &pos);
      auto it = s.cbegin() + pos;
      if (std::distance(it, s.cend()) < 3 or std::any_of(it, it + 3, [](char c){return c != '.';}))
      {
        throw parse_error("Expected '...' in interval, got " + s);
      }
      std::advance(it, 3);
      last = std::stoi(std::string(it, s.cend()));
    }
    catch (const std::invalid_argument&)
    {
      throw parse_error("Expected a value in interval, got " + s);
    }

    if (manip.first_)
    {
      *manip.first_ = first;
      *manip.last_  = last;
    }
    return in;
  }
};

/*------------------------------------------------------------------------------------------------*/

struct prefix
{
  const char p_;
  unsigned int* res_;

  prefix(char p, unsigned int& res)
    : p_(p), res_(&res)
  {}

  prefix(char p)
    : p_(p), res_(nullptr)
  {}

  friend
  std::istream&
  operator>>(std::istream& in, const prefix& manip)
  {
    std::string s;

    if (not (in >> s))
    {
      throw parse_error("Expected a prefixed value");
    }
    if (s[0] != manip.p_)
    {
      throw parse_error("Expected a prefixed value, got " + s);
    }
    try
    {
      *manip.res_ = std::stoi(s.substr(1));
    }
    catch (const std::invalid_argument&)
    {
      throw parse_error("Expected a value, got " + s);
    }
    return in;
  }
};

/*------------------------------------------------------------------------------------------------*/

using units_maker_type = std::map< unsigned int
                                 , std::tuple< std::vector<unsigned int> // nested units' ids
                                             , std::vector<std::reference_wrapper<const pn::place>>
                                             >
                                 >;

const pn::unit&
unit_builder( std::unordered_map<unsigned int, const pn::unit>& res
            , unsigned int id
            , units_maker_type& map
            , std::vector<unsigned int>& subunits)
{
  std::vector<std::reference_wrapper<const pn::unit>> units;
  std::vector<unsigned int> local_subnits;
  subunits.push_back(id);
  const auto search = map.find(id);
  assert(search != map.end() && "Unit id not found.");
  auto& subids = std::get<0>(search->second);
  auto& places = std::get<1>(search->second);
  for (const auto id : subids)
  {
    units.emplace_back(unit_builder(res, id, map, local_subnits));
  }
  subunits.insert(subunits.end(), local_subnits.cbegin(), local_subnits.cend());
  return
    res.emplace(id, pn::unit(id, std::move(places), std::move(units), std::move(local_subnits)))
           .first->second;
}

/*------------------------------------------------------------------------------------------------*/

} // namespace anonymous

/*------------------------------------------------------------------------------------------------*/

std::shared_ptr<pn::net>
bpn(std::istream& in)
{
  // Temporarily stores nested units and places of a unit, indexed by its id.
  units_maker_type map;

  // Temporary placeholders.
  std::string s0;
  unsigned int ui1;

  // Pragmas.
  while ((in >> std::ws).peek() == std::char_traits<char>::to_int_type('!'))
  {
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  // The number of places. We don't need this information.
  in >> kw("places") >> sharp() >> interval();

  // Initial place(s).
  in >> kw("initial") >> s0;
  unsigned int nb_initial_places = 0;
  if (s0 == "place")
  {
    nb_initial_places = 1;
  }
  else if (s0 == "places")
  {
    in >> sharp(nb_initial_places);
  }
  else
  {
    throw parse_error("Expected 'place' or 'places' got " + s0);
  }

  std::vector<unsigned int> initial_places;
  initial_places.reserve(nb_initial_places);
  for (; nb_initial_places > 0; --nb_initial_places)
  {
    unsigned int u;
    in >> uint(u);
    initial_places.push_back(u);
  }

  // The number of units.
  unsigned int nb_units;
  in >> kw("units") >> sharp(nb_units) >> interval();

  // The root unit.
  in >> kw("root") >> kw("unit") >> uint(ui1);

  // Create the Petri net.
  auto net_ptr = std::make_shared<pn::net>(std::move(initial_places), ui1);
  auto& net = *net_ptr;

  // Units
  for (; nb_units > 0; --nb_units)
  {
    unsigned int unit_id, nb_nested_units, first, last, nb_places;
    in >> prefix('U', unit_id) >> sharp(nb_places) >> interval(first, last)
       >> sharp(nb_nested_units);

    auto insertion
      = map.emplace( unit_id
                   , std::make_tuple( std::vector<unsigned int>()
                                    , std::vector<std::reference_wrapper<const pn::place>>()));
    auto& subids = std::get<0>(insertion.first->second);
    auto& places = std::get<1>(insertion.first->second);

    // Places of this unit.
    if (nb_places > 0)
    {
      for (unsigned int i = first; i <= last; ++i)
      {
        const auto& place = net.add_place(i, 0, unit_id);
        places.emplace_back(place);
      }
    }

    // Nested units.
    for (unsigned int i = 0; i < nb_nested_units; ++i)
    {
      subids.emplace_back();
      in >> uint(subids.back());
    }
  }

  // Transitions
  unsigned int nb_transitions;
  in >> kw("transitions") >> sharp(nb_transitions) >> interval();
  for(; nb_transitions > 0; --nb_transitions)
  {
    unsigned int nb_places, transition_id;

    in >> prefix('T', transition_id) >> sharp(nb_places);
    net.add_transition(transition_id);

    // Input places.
    for (; nb_places > 0; --nb_places)
    {
      unsigned int place_id;
      in >> uint(place_id);
      net.add_pre_place(transition_id, place_id, 1 /* arc valuation */);
    }

    // Output places.
    in >> sharp(nb_places);
    for (; nb_places > 0; --nb_places)
    {
      unsigned int place_id;
      in >> uint(place_id);
      net.add_post_place(transition_id, place_id, 1 /* arc valuation */);
    }
  }

  // Set marking of initial places.
  std::for_each( net.initial_places().cbegin(), net.initial_places().cend()
               , [&](unsigned int p){net.update_place(p, 1);});

  // Create units.
  std::vector<unsigned int> empty_vec;
  unit_builder(net.units(), net.root_unit(), map, empty_vec);

  return net_ptr;
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::parsers
