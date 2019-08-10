#pragma once

#include <memory>

#include "conf/configuration.hh"
#include "mc/mc_impl.hh"
#include "pn/net.hh"

namespace pnmc { namespace mc {

/*------------------------------------------------------------------------------------------------*/

class mc
{
private:

  const std::unique_ptr<mc_impl> impl_;

public:

  mc(const conf::pnmc_configuration&);

  void
  operator()(const pn::net&)
  const;

private:

  static
  std::unique_ptr<mc_impl>
  mk_impl(const conf::pnmc_configuration&);
};

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::mc
