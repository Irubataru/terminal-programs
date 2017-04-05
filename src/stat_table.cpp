
/*
 * Created: 27-01-2017
 * Modified: Wed 05 Apr 2017 12:33:50 BST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#include "stat_table.hpp"

using namespace irubataru;

int main(int argc, char *argv[])
{
  auto options = Table_Mean_Options{};

  try {
    options = parse_program_options(argc, argv);
  } catch (nothing_to_do_exception &) {
    return 0;
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  io::Table_Reader<double> reader{options.comments};

  auto table =
      reader.Read(options.filename, options.skip_lines, options.read_every);

  bin_table(table, options.bin_size);

  switch (options.io_format) {
  case Io_Format::Human_Readable:
    print_human_readable(table);
    break;
  case Io_Format::Table:
    print_table_readable(table);
    break;
  }
}

po::variables_map init_program_options(int argc, char *argv[])
{
  po::options_description hidden{"Hidden options"};
  hidden.add_options()("input-file", po::value<std::string>(), "input file");

  po::positional_options_description pod;
  pod.add("input-file", 1);

  po::options_description input_opts{"Parse options"};

  // clang-format off

  input_opts.add_options()
    ("help", "produce this help message")
    ("skip,s", po::value<std::size_t>()->default_value(0),
     "skip the first n rows of the input file [default=0]")
    ("delta,d", po::value<std::size_t>()->default_value(1),
     "only include every n'th row of the input file [default=1]")
    ("comments,c", po::value<std::string>()->default_value("#"),
     "list of comment symbols [default=\"#\"]")
    ("human,h", "human readable output")
    ("col", po::value<std::size_t>()->multitoken(),
     "which columns to parse, will parse all of this option is unspecified")
    ("bin,b", po::value<std::size_t>()->default_value(1), "bin size");

  // clang-format on

  po::options_description cmd_line_options;
  cmd_line_options.add(input_opts).add(hidden);

  auto var_map = po::variables_map{};
  po::store(po::command_line_parser(argc, argv)
                .options(cmd_line_options)
                .positional(pod)
                .run(),
            var_map);

  if (var_map.count("help") or !var_map.count("input-file")) {
    std::cout << "Overview: Tool for computing various statistical properties "
                 "of data stored in a table format.\n\n"
              << "Usage: stat_table <options> [input file]\n\n"
              << input_opts << std::endl;
    throw nothing_to_do_exception{};
  }

  return var_map;
}

Table_Mean_Options parse_program_options(int argc, char *argv[])
{
  auto var_map = init_program_options(argc, argv);
  auto stat_table_options = Table_Mean_Options{};

  stat_table_options.filename = var_map["input-file"].as<std::string>();
  stat_table_options.skip_lines = var_map["skip"].as<std::size_t>();
  stat_table_options.read_every = var_map["delta"].as<std::size_t>();
  stat_table_options.bin_size = var_map["bin"].as<std::size_t>();

  auto comments_str = var_map["comments"].as<std::string>();
  stat_table_options.comments =
      std::vector<char>(comments_str.begin(), comments_str.end());

  if (var_map.count("human"))
    stat_table_options.io_format = Io_Format::Human_Readable;
  else
    stat_table_options.io_format = Io_Format::Table;

  return stat_table_options;
}

template <typename Container>
void print_human_readable(const Container &table)
{
  std::cout << "Read " << table.front().size() << " rows.\n";
  for (const auto &col : table)
    std::cout << math::statistics::mean(col.begin(), col.end()) << " ("
              << math::statistics::standard_deviation(col.begin(), col.end())
              << "), ";

  std::cout << std::endl;
}

template <typename Container>
void print_table_readable(const Container &table)
{
  for (const auto &col : table)
    std::cout << math::statistics::mean(col.begin(), col.end()) << " "
              << math::statistics::standard_deviation(col.begin(), col.end())
              << " ";
}

template <typename Container>
void bin_table(Container &table, std::size_t bin_size)
{
  for (auto &col : table)
    col = math::statistics::bin_data(col, bin_size);
}
