#pragma once

#include <functional> // reference_wrapper
#include <unordered_map>
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
#include "pn/unit.hh"

namespace caesar { namespace pn {

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

  /// @brief The set of units.
  std::unordered_map<unsigned int, const unit> units_;

  /// @brief The initially marked place.
  const std::vector<unsigned int> initial_places_;

  /// @brief The unit that contains all other units.
  const unsigned int root_unit_;

public:

  /// @brief Default constructor.
  net(std::vector<unsigned int>&& initial_places, unsigned int root_unit);

  /// @brief Get initial places.
  const std::vector<unsigned int>&
  initial_places() const noexcept;

  /// @brief Get root unit.
  unsigned int
  root_unit() const noexcept;

  /// @brief Return all units.
  std::unordered_map<unsigned int, const unit>&
  units() noexcept;

  /// @brief Return all units.
  const std::unordered_map<unsigned int, const unit>&
  units() const noexcept;

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

  /// @brief Tell if i contains j or if j contains i.
  bool
  units_are_related(unsigned int i, unsigned int j) const;

  /// @brief Return all transitions.
  const transitions_type::index<id_index>::type&
  transitions() const noexcept;
};

/*------------------------------------------------------------------------------------------------*/

}} // namespace caesar::pn
