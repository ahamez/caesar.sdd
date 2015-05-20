#pragma once

#include <functional> //hash

#include <boost/dynamic_bitset.hpp>

#include "sdd/values/flat_set.hh"

#include "mc/units/exceptions.hh"

namespace pnmc { namespace mc { namespace units {

/*------------------------------------------------------------------------------------------------*/

class post
{
public:

  post(unsigned int p)
    : m_place{p}
  {}

  sdd::values::flat_set<unsigned int>
  operator()(const sdd::values::flat_set<unsigned int>&)
  const noexcept
  {
    return {m_place + 1};
  }

  unsigned int
  place()
  const noexcept
  {
    return m_place;
  }

  friend
  bool
  operator==(const post& lhs, const post& rhs)
  noexcept
  {
    return lhs.m_place == rhs.m_place;
  }

private:

  const unsigned int m_place;
};

/*------------------------------------------------------------------------------------------------*/

template <typename T>
class post_live_mixin
  : public T
{
public:

  template <typename... Args>
  post_live_mixin(unsigned int t, boost::dynamic_bitset<>& bs, Args&&... args)
    : T{std::forward<Args>(args)...}, m_transition{t}, m_bitset{bs}
  {}

  sdd::values::flat_set<unsigned int>
  operator()(const sdd::values::flat_set<unsigned int>& val)
  const noexcept
  {
    m_bitset[m_transition] = true;
    return T::operator()(val);
  }

  std::size_t
  hash()
  const noexcept
  {
    using namespace sdd::hash;
    return seed(9657262313) (val(m_transition)) (val(static_cast<const T&>(*this)));
  }

  friend
  bool
  operator==(const post_live_mixin& lhs, const post_live_mixin& rhs)
  noexcept
  {
    return *static_cast<const T*>(&lhs) == *static_cast<const T*>(&rhs)
       and lhs.m_transition == rhs.m_transition;
  }

private:

  /// @brief Transition identifier.
  const unsigned int m_transition;

  /// @brief The bitset to update.
  boost::dynamic_bitset<>& m_bitset;
};

/*------------------------------------------------------------------------------------------------*/

template <typename T>
class post_safe_mixin
  : public T
{
public:

  using T::T;

  sdd::values::flat_set<unsigned int>
  operator()(const sdd::values::flat_set<unsigned int>& val)
  const
  {
    const auto place = this->place();
    if (val.count(place + 1))
    {
      throw bound_error(place);
    }
    return T::operator()(val);
  }

  std::size_t
  hash()
  const noexcept
  {
    using namespace sdd::hash;
    return seed(1557162437) (val(static_cast<const T&>(*this)));
  }

  friend
  bool
  operator==(const post_safe_mixin& lhs, const post_safe_mixin& rhs)
  noexcept
  {
    return *static_cast<const T*>(&lhs) == *static_cast<const T*>(&rhs);
  }
};

/*------------------------------------------------------------------------------------------------*/

using post_live = post_live_mixin<post>;
using post_safe = post_safe_mixin<post>;
using post_live_safe = post_safe_mixin<post_live_mixin<post>>;

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::units

namespace std {

/*------------------------------------------------------------------------------------------------*/

template<>
struct hash<pnmc::mc::units::post>
{
  std::size_t
  operator()(const pnmc::mc::units::post& p)
  const noexcept
  {
    using namespace sdd::hash;
    return seed(4667242363) (val(p.place()));
  }
};

/*------------------------------------------------------------------------------------------------*/
  
template<typename T>
struct hash<pnmc::mc::units::post_live_mixin<T>>
{
  std::size_t
  operator()(const pnmc::mc::units::post_live_mixin<T>& p)
  const noexcept
  {
    return p.hash();
  }
};

/*------------------------------------------------------------------------------------------------*/
  
template<typename T>
struct hash<pnmc::mc::units::post_safe_mixin<T>>
{
  std::size_t
  operator()(const pnmc::mc::units::post_safe_mixin<T>& p)
  const noexcept
  {
    return p.hash();
  }
};

/*------------------------------------------------------------------------------------------------*/

} // namespace std
