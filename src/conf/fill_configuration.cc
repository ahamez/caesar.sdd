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

pn_encoding
encoding(const po::variables_map& vm)
{
  const bool places = vm.count("places");
  const bool units = vm.count("units");

  if (not (places or units))
  {
    return pn_encoding::units;
  }
  else if (not (places xor units))
  {
    throw po::error("Can specify only one encoding type.");
  }
  else
  {
    if (places)
    {
      return pn_encoding::places;
    }
    else // units
    {
      return pn_encoding::units;
    }
  }
}

/*------------------------------------------------------------------------------------------------*/

boost::optional<pnmc_configuration>
fill_configuration(int argc, char** argv)
{
  pnmc_configuration conf;

  po::options_description general_options("General options");
  general_options.add_options()
    ( "help"                  , "Show this help")
    ( "version"               , "Show version");

  po::options_description order_options("Order options");
  order_options.add_options()
    ("show-order"             , "Show the order")
    ("flat"                   , "Don't use hierarchy informations")
    ("order-min-height"       , po::value<unsigned int>()->default_value(1)
                              , "Minimal number of variables at every level of the SDD");

  po::options_description pn_options("Petri net options");
  pn_options.add_options()
    ("places"                 , "Use one variable per place")
    ("units"                  , "Use one variable per unit");

  po::options_description hom_options("Homomorphisms options");
  hom_options.add_options()
    ("show-relation"          , "Show the transition relation");

  po::options_description stats_options("Statistics options");
  stats_options.add_options()
    ("show-hash-stats"          , "Show the hash tables statistics")
    ("show-time"                , "Show miscellaneous execution times");

  po::options_description cadp_options("CADP options");
  cadp_options.add_options()
    ("dead"         , "Compute dead transitions")
    ("unit"         , "Compute concurrent units");

  po::options_description hidden_options("Hidden options");
  hidden_options.add_options()
    ("input-file", po::value<std::string>(), "The Petri net file to analyse");

  po::positional_options_description p;
  p.add("input-file", 1);
  
  po::options_description cmdline_options;
  cmdline_options
  	.add(general_options)
    .add(order_options)
    .add(hom_options)
    .add(pn_options)
    .add(cadp_options)
    .add(stats_options)
  	.add(hidden_options);
  
  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(argc, argv)
                                    .options(cmdline_options)
                                    .style ( po::command_line_style::default_style
                                           | po::command_line_style::allow_long_disguise)
                                    .positional(p)
                                    .allow_unregistered()
                                    .run();
  po::store(parsed, vm);
  po::notify(vm);
  
  std::vector<std::string> unrecognized
    = po::collect_unrecognized(parsed.options,po::exclude_positional);
  
  if (vm.count("help") or unrecognized.size() > 0)
  {
    std::cout << version << std::endl;
    std::cout << "Usage: " << argv[0] << " [options] file " << std::endl << std::endl;
    std::cout << general_options << std::endl;
    std::cout << order_options << std::endl;
    std::cout << hom_options << std::endl;
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
  conf.order_show = vm.count("show-order");
  conf.encoding = encoding(vm);
  conf.order_force_flat = vm.count("flat");
  conf.order_min_height = vm["order-min-height"].as<unsigned int>();
  conf.show_relation = vm.count("show-relation");
  conf.show_hash_tables_stats = vm.count("show-hash-stats");
  conf.show_time = vm.count("show-time");
  conf.compute_dead_transitions = vm.count("dead");
  conf.compute_concurrent_units = vm.count("unit");

  return conf;
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::conf
