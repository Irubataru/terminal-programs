
/*
 * Created: 23-03-2017
 * Modified: Fri 24 Mar 2017 12:30:20 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef KOLMOGOROV_SMIRNOV_HPP
#define KOLMOGOROV_SMIRNOV_HPP

#include <boost/program_options.hpp>
#include <irubataru/io.hpp>
#include <irubataru/math.hpp>
#include <irubataru/math/statistics/kolmogorov_smirnov_test.hpp>

#include <iostream>
#include <iomanip>

namespace po = boost::program_options;

class nothing_to_do_exception : public std::exception
{
};

struct Software_Options
{
  std::string filename1;
  std::size_t column1;
  std::string filename2;
  std::size_t column2;
};

po::variables_map init_program_options(int argc, char *argv[]);
Software_Options parse_program_options(int argc, char *argv[]);

void print_help_message(po::options_description &opts);

#endif /* KOLMOGOROV_SMIRNOV_HPP */
