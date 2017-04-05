
/*
 * Created: 23-03-2017
 * Modified: Wed 05 Apr 2017 13:39:41 BST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#include <kolmogorov_smirnov.hpp>

using namespace irubataru;

int main(int argc, char *argv[])
{
  auto options = Software_Options{};

  try {
    options = parse_program_options(argc, argv);
  } catch (nothing_to_do_exception &) {
    return 0;
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  io::Table_Reader<double> reader{};

  auto vec1 =
      reader.Read(options.filename1, std::vector<std::size_t>{options.column1})
          .front();

  vec1 = math::statistics::bin_data(vec1, options.bin_size);

  auto vec2 =
      reader.Read(options.filename2, std::vector<std::size_t>{options.column2})
          .front();

  vec2 = math::statistics::bin_data(vec2, options.bin_size);

  auto test_result = math::statistics::Kolmogorov_Smirnov_Test(vec1, vec2);

  std::cout << std::setprecision(15) << "d-value: " << test_result.d_value
            << "\n"
            << "p-value: " << test_result.p_value << std::endl;
}

po::variables_map init_program_options(int argc, char *argv[])
{

  po::positional_options_description pod{};
  po::options_description pod_descriptions{"Input files"};
  po::options_description input_opts{"Parse options"};

  // clang-format off

  // Positional arguments so that I don't have to use --file1 --file2
  pod.add("file1", 1).add("file2", 2);

  pod_descriptions.add_options()
    ("file1", po::value<std::string>()->required(),
     "file where the data for the first distribution resides")
    ("file2", po::value<std::string>()->required(),
     "file where the data for the second distribution resides");

  input_opts.add_options()
    ("help", "produce this help message")
    ("col1", po::value<std::size_t>()->default_value(0),
     "column to read in file1")
    ("col2", po::value<std::size_t>()->default_value(0),
     "column to read in file2")
    ("bin,b", po::value<std::size_t>()->default_value(1),
     "bin size");

  // clang-format on

  po::options_description cmd_line_options;
  cmd_line_options.add(pod_descriptions).add(input_opts);

  auto var_map = po::variables_map{};

  po::store(po::command_line_parser(argc, argv)
                .options(cmd_line_options)
                .positional(pod)
                .run(),
            var_map);

  if (var_map.count("help")) {
    print_help_message(cmd_line_options);
    throw nothing_to_do_exception{};
  }

  // Check the input for parsing errors
  try {
    po::notify(var_map);
  } catch (std::exception &e) {
    std::cout << "Error: " << e.what() << "\n\n";
    print_help_message(cmd_line_options);
    throw nothing_to_do_exception{};
  }

  return var_map;
}

Software_Options parse_program_options(int argc, char *argv[])
{
  auto var_map = init_program_options(argc, argv);
  auto options = Software_Options{};

  options.filename1 = var_map["file1"].as<std::string>();
  options.column1 = var_map["col1"].as<std::size_t>();
  options.filename2 = var_map["file2"].as<std::string>();
  options.column2 = var_map["col2"].as<std::size_t>();
  options.bin_size = var_map["bin"].as<std::size_t>();

  return options;
}

void print_help_message(po::options_description &opts)
{
  std::cout
      << "Overview: Computes the p and d value of the Kolmogorov-Smirnov test\n"
      << "          of two sets of data stored in a tabular format.\n\n"
      << "Usage: kolmogorov_smirnov <options> [input file 1] [input file 2]\n"
      << opts << std::endl;
}
