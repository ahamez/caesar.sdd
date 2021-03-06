#pragma once

#include <sdd/sdd.hh>

namespace caesar { namespace mc { namespace units {

/*------------------------------------------------------------------------------------------------*/

struct sdd_conf
  : public sdd::conf1
{
  using Identifier = unsigned int;
  using Variable   = unsigned int;
};

using SDD = sdd::SDD<sdd_conf>;

using homomorphism = sdd::homomorphism<sdd_conf>;
using sdd::composition;
using sdd::fixpoint;
using sdd::id;
using sdd::sum;
using sdd::function;

using order_identifier = sdd::order_identifier<sdd_conf>;
using order_builder    = sdd::order_builder<sdd_conf>;
using order            = sdd::order<sdd_conf>;

/*------------------------------------------------------------------------------------------------*/

}}} // namespace caesar::mc::units
