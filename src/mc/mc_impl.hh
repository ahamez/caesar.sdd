#pragma once

#include "pn/net.hh"

namespace caesar { namespace mc {

/*------------------------------------------------------------------------------------------------*/

class mc_impl
{
public:

  virtual ~mc_impl();

  virtual
  void
  operator()(const pn::net&)
  const = 0;
};

/*------------------------------------------------------------------------------------------------*/

}} // namespace caesar::mc
