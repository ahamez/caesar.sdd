#ifndef _PNMC_MC_SDD_HH_
#define _PNMC_MC_SDD_HH_

#include <sdd/sdd.hh>

namespace pnmc { namespace mc { namespace places {

/*------------------------------------------------------------------------------------------------*/

struct sdd_conf
  : public sdd::conf0
{
  using Identifier = unsigned int;
  using Variable   = unsigned int;
};

using SDD = sdd::SDD<sdd_conf>;

using homomorphism = sdd::homomorphism<sdd_conf>;
using sdd::Composition;
using sdd::Fixpoint;
using sdd::Id;
using sdd::Sum;
using sdd::ValuesFunction;

using order_identifier = sdd::order_identifier<sdd_conf>;
using order_builder    = sdd::order_builder<sdd_conf>;
using order            = sdd::order<sdd_conf>;

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::places

#endif // _PNMC_MC_SDD_HH_
