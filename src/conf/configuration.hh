#ifndef _PNMC_CONF_CONFIGURATION_HH_
#define _PNMC_CONF_CONFIGURATION_HH_

#include <string>

namespace pnmc { namespace conf {
  
/*------------------------------------------------------------------------------------------------*/

struct pnmc_configuration
{
  std::string file_name;
  bool read_stdin;
  bool export_to_tina;
  std::string tina_file_name;

  bool order_show;
  bool order_force;

  bool show_nb_states;
  bool show_time;

  bool compute_dead_transitions;
  bool compute_concurrent_units;

  bool check_one_safe;
};

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::configuration

#endif // _PNMC_CONF_CONFIGURATION_HH_
