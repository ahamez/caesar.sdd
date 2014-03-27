#include <algorithm>  // any_of
#include <functional> // reference_wrapper
#include <iostream>
#include <map>
#include <string>
#include <tuple>

#include "parsers/bpn.hh"
#include "parsers/parse_error.hh"
#include "pn/unit.hh"

namespace pnmc { namespace parsers {

// BPN grammar
// see http://cadp.inria.fr/man/caesar.bdd.html
//
// <basic-petri-net> ::= 
//    places #<nb-of-places> <min-place-nb>...<max-place-nb>\n
//    initial place <init-place-nb>\n
//    units #<nb-of-units> <min-unit-nb>...<max-unit-nb>\n
//    root unit <root-unit-nb>\n
//    <unit-description>*\n
//    transitions #<nb-of-trans> <min-trans-nb>...<max-trans-nb>\n
//    <trans-description>*\n
// <unit-description> ::=
//    U<unit-nb>
//    #<nb-of-subplaces> <min-subplace-nb>...<max-subplace-nb>
//    #<nb-of-subunits> <subunit-list>\n
// <trans-description> ::=
//    T<transition-nb> 
//    #<nb-of-input-places> <input-place-list>
//    #<nb-of-output-places> <output-place-list>\n
// <input-place-list> ::= <place-nb>*
// <output-place-list> ::= <place-nb>*
// <subunit-list> ::= <unit-nb>*
// <nb-of-places>        ::= <unsigned-integer>
// <min-place-nb>        ::= <unsigned-integer>
// <max-place-nb>        ::= <unsigned-integer>
// <init-place-nb>       ::= <unsigned-integer>
// <place-nb>            ::= <unsigned-integer>
// <nb-of-units>         ::= <unsigned-integer>
// <min-unit-nb>         ::= <unsigned-integer>
// <max-unit-nb>         ::= <unsigned-integer>
// <root-unit-nb>        ::= <unsigned-integer>
// <unit-nb>             ::= <unsigned-integer>
// <nb-of-trans>         ::= <unsigned-integer>
// <min-trans-nb>        ::= <unsigned-integer>
// <max-trans-nb>        ::= <unsigned-integer>
// <nb-of-subplaces>     ::= <unsigned-integer>
// <min-subplace-nb>     ::= <unsigned-integer>
// <max-subplace-nb>     ::= <unsigned-integer>
// <nb-of-subunits>      ::= <unsigned-integer>
// <transition-nb>       ::= <unsigned-integer>
// <nb-of-input-places>  ::= <unsigned-integer>
// <nb-of-output-places> ::= <unsigned-integer>
//
// A valid BPN file should satisfy the following constraints: 
//
// In <basic-petri-net>:
//   1.  <nb-of-places> > 0     -- a net has at least one place
//   2.  <max-place-nb> - <min-place-nb> + 1 = <nb-of-places>
//   3.  <min-place-nb> <= <init-place-nb> <= <max-place-nb>
//   4.  <nb-of-units> > 0     -- a net has at least one unit
//   5.  <max-unit-nb> - <min-unit-nb> + 1 = <nb-of-units>
//   6.  <min-unit-nb> <= <root-unit-nb> <= <max-unit-nb>
//   7.  <nb-of-trans> >= 0    -- a net may have zero transition
//   8.  <nb-of-trans> = 0 => <min-trans-nb> = 1
//   9.  <nb-of-trans> = 0 => <max-trans-nb> = 0
//  10.  <max-trans-nb> - <min-trans-nb> + 1 = <nb-of-trans>
// In each <unit-description>:
//  11.  <min-unit-nb> <= <unit-nb> <= <max-unit-nb>
//  12.  <nb-of-subplaces> > 0 -- a unit has at least one local place
//  13.  <min-place-nb> <= <min-subplace-nb> <= <max-place-nb>
//  14.  <min-place-nb> <= <max-subplace-nb> <= <max-place-nb>
//  15.  <max-subplace-nb> - <min-sublace-nb> + 1 = <nb-of-subplaces>
//  16.  <nb-of-subunits> >= 0
//  17.  length (<subunit-list>) = <nb-of-subunits>
// Globally to all <unit-description>s:
//  18.  each <unit-nb> occurs once and only once after a "U"
//  19.  the sum of all <nb-of-subplaces> is equal to <nb-of-places>
//  20.  all intervals <min-subplace-nb>...<max-subplace-nb> form
//       a partition of <min-place-nb>...<max-place-nb>
//  21.  the sum of all <nb-of-subunits> is equal to <nb-of-units>
//  22.  <root-unit-number> and all non-empty <subunit-list>s form
//       a partition of <min-unit-nb>...<max-unit-nb>
// In each <subunit-list>:
//  23.  <min-unit-nb> <= <unit-nb> <= <max-unit-nb>
// In each <trans-description>:
//  24.  <min-trans-nb> <= <trans-nb> <= <max-trans-nb>
//  25.  length (<input-place-list>) = <nb-of-input-places>
//  26.  length (<output-place-list>) = <nb-of-output-places>
// Globally to all <trans-description>s:
//  27.  each <trans-nb> occurs once and only once after a "T"
// In each <input-place-list> and each <output-place-list>
//  28. <min-place-nb> <= <place-nb> <= <max-place-nb>

namespace /* anonymous */ {

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

  // The number of place. We don't need this information.
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

  // Set marking of initial place.
  std::for_each( net.initial_places().cbegin(), net.initial_places().cend()
               , [&](unsigned int p){net.update_place(p, 1);});

  // Create units.
  std::vector<unsigned int> empty_vec;
  unit_builder(net.units(), net.root_unit(), map, empty_vec);

  return net_ptr;
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::parsers
