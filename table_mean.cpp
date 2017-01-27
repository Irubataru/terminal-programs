
/*
 * Created: 27-01-2017
 * Modified: Fri 27 Jan 2017 14:01:35 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#include <irubataru_io.hpp>
#include <irubataru_math.hpp>

#include <iostream>

using namespace irubataru;

void print_help(std::string program_name)
{
  std::cout
      << program_name << " [FILE] [OPTIONS]\n"
      << "computes the average and standard deviation of a table of "
         "numeric values\n"
      << "will result in an error if the array is staggered\n\n"
      << "after the filename the number of lines to skip at the "
         "beginning of the file,\nand the number of lines to skip between "
         "\"measurements\" can be specified\n\n"
      << "[OPTIONS] = [<lines to skip> = 0] [<use every n'th line> = 1]\n";
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    print_help(argv[0]);
    return 1;
  }
  else if (argv[1] == std::string{"-h"} or argv[1] == std::string{"--help"})
  {
    print_help(argv[0]);
    return 0;
  }

  auto skip_lines = 0ul;
  auto read_every = 1ul;

  if (argc > 2)
    skip_lines = boost::lexical_cast<std::size_t>(argv[2]);

  if (argc > 3)
    read_every = boost::lexical_cast<std::size_t>(argv[3]);

  io::TableReader<double> reader{};
  auto table = reader.Read(std::string{argv[1]}, skip_lines, read_every);

  std::cout << "Read " << table.front().size() << " rows.\n";
  for (const auto &col : table)
    std::cout << math::statistics::mean(col.begin(), col.end()) << " ("
              << math::statistics::standard_deviation(col.begin(), col.end()) /
                     std::sqrt(col.size())
              << "), ";

  std::cout << std::endl;
}
