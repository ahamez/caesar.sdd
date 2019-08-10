#pragma once

#include <memory>

#include "conf/configuration.hh"
#include "mc/mc_impl.hh"
#include "pn/net.hh"

namespace caesar { namespace mc {

/*------------------------------------------------------------------------------------------------*/

class mc
{
private:

  const std::unique_ptr<mc_impl> impl_;

public:

  mc(const conf::caesar_configuration&);

  void
  operator()(const pn::net&)
  const;

private:

  static
  std::unique_ptr<mc_impl>
  mk_impl(const conf::caesar_configuration&);
};

/*------------------------------------------------------------------------------------------------*/

}} // namespace caesar::mc
