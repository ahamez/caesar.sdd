#ifndef _PNMC_MC_UNIT_HH_
#define _PNMC_MC_UNIT_HH_

#include <iosfwd>

#include <sdd/sdd.hh>

#include "conf/configuration.hh"
#include "pn/net.hh"

namespace pnmc { namespace mc {

/*------------------------------------------------------------------------------------------------*/

using SDD = sdd::SDD<sdd::conf1>;
void
compute_concurrent_units( const conf::pnmc_configuration&,const pn::net&
                        , const sdd::order<sdd::conf1>&, SDD);

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::mc
//
//namespace std
//{

/*------------------------------------------------------------------------------------------------*/

//template <>
//struct hash<pnmc::mc::live>
//{
//  std::size_t operator()(const pnmc::mc::live&) const noexcept;
//};

/*------------------------------------------------------------------------------------------------*/

//} // namespace std

#endif // _PNMC_MC_UNIT_HH_
