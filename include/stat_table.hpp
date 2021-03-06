
/*
 * Created: 06-02-2017
 * Modified: Wed 05 Apr 2017 12:32:54 BST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef TABLE_MEAN_HPP
#define TABLE_MEAN_HPP

#include <boost/program_options.hpp>
#include <irubataru/io.hpp>
#include <irubataru/math.hpp>

#include <iostream>

namespace po = boost::program_options;

enum class Io_Format
{
  Human_Readable,
  Table
};

struct Table_Mean_Options
{
  std::string filename;
  std::size_t skip_lines;
  std::size_t read_every;
  std::vector<char> comments;
  std::size_t bin_size;
  Io_Format io_format;
};

class nothing_to_do_exception : public std::exception
{
};

po::variables_map init_program_options(int argc, char *argv[]);
Table_Mean_Options parse_program_options(int argc, char *argv[]);

template <typename Container>
void bin_table(Container &, std::size_t);

template <typename Container>
void print_human_readable(const Container &);

template <typename Container>
void print_table_readable(const Container &);

#endif /* TABLE_MEAN_HPP */
