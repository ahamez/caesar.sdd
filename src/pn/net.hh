#ifndef _PNMC_PN_NET_HH_
#define _PNMC_PN_NET_HH_

#include <functional> // reference_wrapper
#include <vector>

#pragma GCC diagnostic push
#if defined(__GNUC__) && !defined(__clang__)
# pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#endif
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#pragma GCC diagnostic pop

#include "pn/place.hh"
#include "pn/transition.hh"

namespace pnmc { namespace pn {

/*------------------------------------------------------------------------------------------------*/

using namespace boost::multi_index;

/*------------------------------------------------------------------------------------------------*/

class net
{
private:

  /// @brief A tag to identify the view ordered by insertion order for boost::multi_index.
  struct insertion_index{};

  /// @brief A tag to identify the view ordered by idendifiers for boost::multi_index.
  struct id_index{};

  /// @brief A tag to identify the view ordered by markings for boost::multi_index.
  struct marking_index{};

  /// @brief
  struct unit_index{};

  /// @brief The type of the set of all places.
  typedef multi_index_container<
                place
  						, indexed_by<

                  // keep insertion order
                    sequenced<tag<insertion_index>>

                  // sort by id
                  ,  ordered_unique< tag<id_index>
                                   , member<place, const unsigned int, &place::id>>

                  // sort by unit
                  , ordered_non_unique< tag<unit_index>
                                      , member<place, const unsigned int, &place::unit>>
              >
          > places_type;

  /// @brief The type of the set of all transitions.
  typedef multi_index_container<
                transition
              , indexed_by<

                   // sort by id
                     ordered_unique< tag<id_index>
                                   , member<transition, const unsigned int, &transition::id>>

              >
         > transitions_type;

  /// @brief The set of places.
  places_type places_;

  /// @brief The set of transitions.
  transitions_type transitions_;

public:

  /// @brief The unit that contains all other units.
  unsigned int root_unit;

  /// @brief The initially marked place.
  unsigned int initial_place;

  /// @brief Default constructor.
  net();

  /// @brief Add a place.
  const place&
  add_place(unsigned int id, unsigned int marking, unsigned int unit);

  /// @brief Add a transition.
  ///
  /// If the transition already exists, no operation is done.
  const transition&
  add_transition(unsigned int tid);

  /// @brief Add a post place to a transition.
  void
  add_post_place(unsigned int id, unsigned int post, unsigned int marking);

  /// @brief Add a pre place to a transition.
  void
  add_pre_place(unsigned int id, unsigned int pre, unsigned int marking);

  /// @brief Update a place.
  void
  update_place(unsigned int id, unsigned int marking);

  /// @brief Return all places by insertion order.
  const places_type::index<insertion_index>::type&
  places() const noexcept;

  /// @brief Return all places by identifier.
  const places_type::index<id_index>::type&
  places_by_id() const noexcept;

  /// @brief Return all places of a unit.
  std::vector<std::reference_wrapper<const place>>
  places_of_unit(unsigned int unit) const;

  /// @brief Return the unit of a place, using its id.
  unsigned int
  unit_of_place(unsigned int place) const;

  /// @brief Return the unit of a place.
  unsigned int
  unit_of_place(const place&) const;

  /// @brief Return the number of units.
  std::size_t
  units_size() const noexcept;

  /// @brief Return all transitions.
  const transitions_type::index<id_index>::type&
  transitions() const noexcept;
};

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::pn

#endif // _PNMC_PN_NET_HH_
