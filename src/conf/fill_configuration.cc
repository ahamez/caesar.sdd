#include <iostream>
#include <string>

#pragma GCC diagnostic push
#if defined(__GNUC__) && !defined(__clang__)
# pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#endif
#include <boost/program_options.hpp>
#pragma GCC diagnostic pop

#include "conf/configuration.hh"
#include "conf/fill_configuration.hh"

namespace pnmc { namespace conf {

/*------------------------------------------------------------------------------------------------*/

namespace po = boost::program_options;

const std::string version
  = "Caesar.SDD (built " + std::string(__DATE__) + " " + std::string(__TIME__)  + ")";

/*------------------------------------------------------------------------------------------------*/

// General options
constexpr auto help_str = "help";
constexpr auto version_str = "version";

// Order options
constexpr auto order_show_str = "order-show";
constexpr auto order_force_str = "order-force";
constexpr auto order_force_iterations_str = "order-force-iterations";

// Petri net options
constexpr auto check_safe_net_str = "check";

// Statistics options
constexpr auto show_nb_states_str = "show-nb-states";
constexpr auto show_time_str = "show-time";

// CADP options
constexpr auto dead_str = "dead-transitions";
constexpr auto unit_str = "concurrent-units";

boost::optional<pnmc_configuration>
fill_configuration(int argc, char** argv)
{
  pnmc_configuration conf;

  po::options_description general_options("General options");
  general_options.add_options()
    ( help_str                  , "Show this help")
    ( version_str               , "Show version");

  po::options_description order_options("Order options");
  order_options.add_options()
    (order_show_str             , "Show the order")
    (order_force_str            , "Use the FORCE ordering heuristic")
    (order_force_iterations_str , po::value<unsigned int>()->default_value(100)
                                , "Number of FORCE iterations")
  ;

  po::options_description pn_options("Petri net options");
  pn_options.add_options()
    (check_safe_net_str           , "Interrupt computation if a marking > 1")
  ;

  po::options_description stats_options("Statistics options");
  stats_options.add_options()
    (show_nb_states_str           , "Show the number of states")
    (show_time_str                , "Show miscellaneous execution times")
    ;

  po::options_description cadp_options("CADP options");
  cadp_options.add_options()
    (dead_str         , "Compute dead transitions")
    (unit_str         , "Compute concurrent units");

  po::options_description hidden_options("Hidden options");
  hidden_options.add_options()
    ("input-file", po::value<std::string>(), "The Petri net file to analyse");

  po::positional_options_description p;
  p.add("input-file", 1);
  
  po::options_description cmdline_options;
  cmdline_options
  	.add(general_options)
    .add(order_options)
    .add(pn_options)
    .add(cadp_options)
    .add(stats_options)
  	.add(hidden_options);
  
  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(argc, argv)
                                    .options(cmdline_options)
                                    .style (po::command_line_style::default_style)
                                    .positional(p)
                                    .allow_unregistered()
                                    .run();
  po::store(parsed, vm);
  po::notify(vm);
  
  std::vector<std::string> unrecognized
    = po::collect_unrecognized(parsed.options,po::exclude_positional);
  
  if (vm.count(help_str) or unrecognized.size() > 0)
  {
    std::cout << version << std::endl;
    std::cout << "Usage: " << argv[0] << " [options] file " << std::endl << std::endl;
    std::cout << general_options << std::endl;
    std::cout << order_options << std::endl;
    std::cout << pn_options << std::endl;
    std::cout << cadp_options << std::endl;
    std::cout << stats_options << std::endl;
    return boost::optional<pnmc_configuration>();
  }
  
  if (vm.count("version"))
  {
    std::cout << version << std::endl;
    return boost::optional<pnmc_configuration>();
  }

  conf.read_stdin = (not vm.count("input-file") or vm["input-file"].as<std::string>() == "-");
  if (not conf.read_stdin)
  {
    conf.file_name = vm["input-file"].as<std::string>();
  }
  conf.show_nb_states = vm.count(show_nb_states_str);
  conf.order_show = vm.count(order_show_str);
  conf.order_force = vm.count(order_force_str);
  conf.show_time = vm.count(show_time_str);
  conf.compute_dead_transitions = vm.count(dead_str);
  conf.compute_concurrent_units = vm.count(unit_str);
  conf.check_one_safe = vm.count(check_safe_net_str);

  return conf;
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::conf
