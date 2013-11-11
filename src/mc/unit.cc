#include <chrono>
#include <ostream>

#include <sdd/util/hash.hh>
#include <sdd/values/flat_set.hh>
#include <sdd/values_manager.hh>

#include "mc/unit.hh"

namespace pnmc { namespace mc {

/*------------------------------------------------------------------------------------------------*/

namespace {

SDD
query(const pn::net& net, const sdd::order<sdd::conf1>& order, std::size_t i, std::size_t j)
{
  SDD q = sdd::zero<sdd::conf1>();
  for (const auto& pi : net.units[i])
  {
    for (const auto& pj : net.units[j])
    {
      q += SDD(order, [&](const std::string& id)
                          -> sdd::values::flat_set<unsigned int>
                          {
                            return id == pi.get().id or id == pj.get().id
                                 ? sdd::values::flat_set<unsigned int>({1})
                                 : sdd::values::flat_set<unsigned int>({0, 1});
                          });
    }
  }
  return q;
}

} // namespace anonymous

/*------------------------------------------------------------------------------------------------*/

void
compute_concurrent_units( const conf::pnmc_configuration& conf, const pn::net& net
                        , const sdd::order<sdd::conf1>& o, SDD states)
{
  namespace chrono = std::chrono;
  chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();

  const std::size_t nb_units = net.units.size();
  std::size_t j_end = 1;
  for (std::size_t i = 0; i < nb_units; ++i)
  {
    for (std::size_t j = 0; j < j_end; ++j)
    {
      if (i == j)
      {
        std::cout << "0 ";
      }
      else
      {
        std::cout << ((query(net, o, i, j) & states) == sdd::zero<sdd::conf1>() ? "0 " : "1 ");
      }
    }
    std::cout << std::endl;
    ++j_end;
  }

  chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
  const std::size_t elapsed = chrono::duration_cast<chrono::seconds>(end-start).count();
  if (conf.show_time)
  {
    std::cout << "Concurrent units computation time: " << elapsed << "s" << std::endl;
  }
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::mc
