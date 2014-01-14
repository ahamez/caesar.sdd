#include <chrono>
#include <ostream>

#include <sdd/values/bitset.hh>

#include "mc/places/concurrent_units.hh"

namespace pnmc { namespace mc { namespace places {

/*------------------------------------------------------------------------------------------------*/

namespace {

SDD
query(const pn::net& net, const sdd::order<sdd_conf>& order, unsigned int i, unsigned int j)
{
  SDD q = sdd::zero<sdd_conf>();
  for (const auto& pi : net.places_of_unit(i))
  {
    for (const auto& pj : net.places_of_unit(j))
    {
      q += SDD(order, [&](unsigned int id)
                          {
                            return id == pi.get().id or id == pj.get().id
                                 ? sdd::values::bitset<64>({1})
                                 : sdd::values::bitset<64>({0, 1});
                          });
    }
  }
  return q;
}

} // namespace anonymous

/*------------------------------------------------------------------------------------------------*/

void
compute_concurrent_units( const conf::pnmc_configuration& conf, const pn::net& net
                        , const sdd::order<sdd_conf>& o, SDD states)
{
  namespace chrono = std::chrono;
  chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();

  const auto nb_units = net.units_size();
  unsigned int j_end = 1;
  for (unsigned int i = 0; i < nb_units; ++i, ++j_end)
  {
    for (unsigned int j = 0; j < j_end; ++j)
    {
      if (i == j)
      {
        std::cout << "0 ";
      }
      else
      {
        std::cout << ((query(net, o, i, j) & states) == sdd::zero<sdd_conf>() ? "0 " : "1 ");
      }
    }
    std::cout << std::endl;
  }

  chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
  const std::size_t elapsed = chrono::duration_cast<chrono::seconds>(end-start).count();
  if (conf.show_time)
  {
    std::cout << "Concurrent units computation time: " << elapsed << "s" << std::endl;
  }
}

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::places
