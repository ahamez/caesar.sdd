#ifndef _PNMC_PN_NET_HH_
#define _PNMC_PN_NET_HH_

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>

#include "pn/arc.hh"
#include "pn/module.hh"
#include "pn/place.hh"
#include "pn/transition.hh"

namespace pnmc { namespace pn {

/*------------------------------------------------------------------------------------------------*/

using namespace boost::multi_index;

/*------------------------------------------------------------------------------------------------*/

struct net
{
private:

  /// @brief A tag to identify index for boost::multi_index.
  struct id_index{};

  /// @brief A tag to identify index for boost::multi_index.
  struct label_index{};

  /// @brief A tag to identify index for boost::multi_index
  struct marking_index{};

public:

  /// @brief The type of the set of all places.
  typedef multi_index_container<
                place
  						, indexed_by<

                  // sort by id
                     ordered_unique<tag< id_index>
                                       , member<place, const std::string, &place::id>>

                  // sort by label
                  , ordered_non_unique< tag<label_index>
                                      , member<place, std::string, &place::label>>

                  // sort by marking
                  , ordered_non_unique< tag<marking_index>
                                      , member<place, unsigned int, &place::marking>>
              >
          > places_type;

  /// @brief The type of the set of all transitions.
  typedef multi_index_container<
                transition
              , indexed_by<

                   // sort by id
                     ordered_unique<tag< id_index>
                                       , member<transition, const std::string, &transition::id>>

                   // sort by label
                   , ordered_non_unique< tag<label_index>
                                       , member<transition, std::string, &transition::label>>
              >
         > transitions_type;

  /// @brief The net's name.
  std::string name;

  /// @brief The set of places.
  places_type places_set;

  /// @brief The set of transitions.
  transitions_type transitions_set;

  /// @brief
  module modules;

  net();

  const place&
  add_place(const std::string& id, const std::string& label, unsigned int marking);

  const transition&
  add_transition(const std::string& tid, const std::string& label);

  void
  add_post_place(const std::string& tid, const std::string& post, const arc& a);

  void
  add_pre_place(const std::string& tid, const std::string& post, const arc& a);

  const places_type::index<id_index>::type&
  places() const noexcept;

  const transitions_type::index<id_index>::type&
  transitions() const noexcept;

};

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::pn

#endif // _PNMC_PN_NET_HH_