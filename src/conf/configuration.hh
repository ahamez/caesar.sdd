#ifndef _PNMC_CONF_CONFIGURATION_HH_
#define _PNMC_CONF_CONFIGURATION_HH_

#include <string>

namespace pnmc { namespace conf {
  
/*------------------------------------------------------------------------------------------------*/

enum class pn_encoding {places, units};

struct pnmc_configuration
{
  std::string file_name;
  bool read_stdin;
  bool export_to_tina;
  std::string tina_file_name;

  pn_encoding encoding;

  bool order_show;
  bool order_force_flat;
  unsigned int order_min_height;

  bool show_relation;
  bool show_time;
  bool show_hash_tables_stats;

  bool compute_dead_transitions;
  bool compute_concurrent_units;
};

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::configuration

#endif // _PNMC_CONF_CONFIGURATION_HH_
