#include "mc/mc.hh"
#include "mc/places/places_encoding.hh"

namespace pnmc { namespace mc {

/*------------------------------------------------------------------------------------------------*/

mc::mc(const conf::pnmc_configuration& conf)
  : impl_(mk_impl(conf))
{}

/*------------------------------------------------------------------------------------------------*/

std::unique_ptr<mc_impl>
mc::mk_impl(const conf::pnmc_configuration& conf)
{
//  class unit_encoding;

//  if (conf.places_encoding)
//  {
  return std::unique_ptr<mc_impl>(new places_encoding(conf));
//  }
//  else if (conf.unit_encoding)
//  {
//
//  }
}

/*------------------------------------------------------------------------------------------------*/

void
mc::operator()(const pn::net& net)
const
{
  impl_->operator()(net);
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::mc
