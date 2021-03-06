#include <cstdio>
#include <fstream>
#include <iostream>

#include <boost/program_options/errors.hpp>

#include "conf/fill_configuration.hh"
#include "mc/mc.hh"
#include "parsers/parse.hh"
#include "parsers/parse_error.hh"
#include "pn/tina.hh"
#include "mc/units/exceptions.hh"

/*------------------------------------------------------------------------------------------------*/

namespace caesar {

struct unreadable_file
  : std::exception
{};

std::shared_ptr<std::istream>
file_or_cin(const conf::caesar_configuration& conf)
{
  if (conf.read_stdin)
  {
    std::ios_base::sync_with_stdio(false); // Standard input buffering.
    return std::shared_ptr<std::istream>(&std::cin, [](std::istream*){}); // Avoid to erase cin.
  }
  else
  {
    std::shared_ptr<std::istream> ptr(new std::ifstream(conf.file_name));
    if (not dynamic_cast<std::ifstream&>(*ptr).is_open())
    {
      throw unreadable_file();
    }
    return ptr;
  }
}

} // namespace caesar

/*------------------------------------------------------------------------------------------------*/

int
main(int argc, char** argv)
{
  using namespace caesar;

  boost::optional<conf::caesar_configuration> conf_opt;
  try
  {
    conf_opt = conf::fill_configuration(argc, argv);
  }
  catch (const boost::program_options::error& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  if (not conf_opt) // --help or --version
  {
    return 0;
  }

  const auto& conf = *conf_opt;
  try
  {
    auto in = file_or_cin(conf);
    const auto net_ptr = parsers::parse(*in);
    mc::mc worker(conf);
    worker(*net_ptr);
  }
  catch (const unreadable_file&)
  {
    std::cerr << "Can't open '" << conf.file_name << "'." << std::endl;
    return 2;
  }
  catch (const parsers::parse_error& p)
  {
    std::cerr << "Error when parsing input." << std::endl;
    std::cerr << p.what() << std::endl;
    return 3;
  }
  catch (const mc::units::bound_error& e)
  {
    std::cerr << "Petri net is not 1-safe, marking of place " << e.place << " is > 1\n";
    return 4;
  }
  return 0;
}

/*------------------------------------------------------------------------------------------------*/
