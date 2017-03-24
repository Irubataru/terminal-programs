
/*
 * Created: 27-01-2017
 * Modified: Fri 24 Mar 2017 13:21:51 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef IRUBATARU_IO_HPP
#define IRUBATARU_IO_HPP

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <vector>

namespace irubataru {
namespace io {

template <typename Value_Type>
class Table_Reader
{
public:
  using return_type = std::vector<std::vector<Value_Type>>;

  explicit Table_Reader() : Table_Reader({'#'})
  {
  }

  explicit Table_Reader(std::vector<char> comments)
      : Table_Reader{std::move(comments), {}}
  {
  }

  explicit Table_Reader(std::vector<char> comments,
                        std::vector<std::size_t> columns)
      : comment_chars{std::move(comments)}, columns{std::move(columns)}
  {
  }

  void set_columns(std::vector<std::size_t> cols)
  {
    columns = std::move(cols);
  }

  void set_comments(std::vector<std::size_t> comms)
  {
    comment_chars = std::move(comms);
  }

  return_type Read(std::string filename, std::vector<std::size_t> cols, std::size_t skip_lines = 0,
                   std::size_t read_every = 1)
  {
    columns = cols;
    return Read(filename, skip_lines, read_every);
  }

  return_type Read(std::string filename, std::size_t skip_lines = 0,
                   std::size_t read_every = 1)
  {
    ifs.open(filename);
    if (!ifs)
      throw std::runtime_error{"Cannot open file \"" + filename +
                               "\" for reading."};

    read_preamble(skip_lines);
    if (!ifs) {
      ifs.close();
      return {};
    }

    auto result = read_body(read_every);

    ifs.close();
    return result;
  }

private:
  void read_preamble(std::size_t skip_lines)
  {
    auto lines_read = std::size_t{0ul};

    while (ifs and lines_read < skip_lines) {
      if (!is_comment())
        ++lines_read;

      discard_line();
    }

    discard_comments();
  }

  return_type read_body(std::size_t read_every)
  {
    auto table = allocate_table();
    auto number_of_columns = determine_number_of_columns();

    auto read_lines = std::size_t{0ul};
    while (ifs) {
      if (is_comment()) {
        discard_line();
        continue;
      }

      ++read_lines;
      if ((read_lines - 1) % read_every != 0) {
        discard_line();
        continue;
      }

      auto row = read_row(number_of_columns);

      if (row.empty())
        continue;

      push_back_row(table, row);
    }

    return table;
  }

  std::vector<Value_Type> read_row(std::size_t columns)
  {
    std::string single_line{};
    std::getline(ifs, single_line);

    single_line = sanitise_row(single_line);

    if (single_line.empty())
      return {};

    auto str_vec = std::vector<std::string>{};
    boost::split(str_vec, single_line, boost::is_any_of(" "), boost::token_compress_on);

    if (str_vec.size() != columns) {
      std::ostringstream oss;
      oss << "Inconsistent table format. Columns: " << columns << ", "
          << "current row has " << str_vec.size() << " columns.\n"
          << "Row is: " << single_line;
      throw std::runtime_error{oss.str()};
    }

    auto row = std::vector<Value_Type>(columns);

    std::transform(
        str_vec.begin(), str_vec.end(), row.begin(),
        [](std::string s) { return boost::lexical_cast<Value_Type>(s); });

    return row;
  }

  void discard_comments()
  {
    while (ifs and is_comment())
      discard_line();
  }

  void discard_line()
  {
    std::string throw_away{};
    std::getline(ifs, throw_away);
  }

  bool is_comment()
  {
    return std::find(comment_chars.begin(), comment_chars.end(), ifs.peek()) !=
           comment_chars.end();
  }

  std::size_t determine_number_of_columns()
  {
    auto init_pos = ifs.tellg();

    auto one_line = std::string{};
    std::getline(ifs, one_line);

    ifs.seekg(init_pos);
    return count_columns(sanitise_row(one_line));
  }

  std::size_t count_columns(std::string sanitised_str)
  {
    return std::count(sanitised_str.begin(), sanitised_str.end(), ' ') + 1;
  }

  std::string sanitise_row(std::string str)
  {
    boost::trim_left(str);
    boost::trim_right(str);

    // Remove all multiples of spaces
    str.erase(std::unique(str.begin(), str.end(),
                          [](char lhs, char rhs) {
                            return std::isspace(lhs) and std::isspace(rhs);
                          }),
              str.end());

    // Convert all whitespace to space
    std::transform(str.begin(), str.end(), str.begin(),
                   [](char c) { return std::isspace(c) ? ' ' : c; });

    return str;
  }

  return_type allocate_table()
  {
    auto number_of_columns = determine_number_of_columns();

    if (columns.empty())
      return return_type(number_of_columns);

    for (auto col : columns)
      if (col >= number_of_columns) {
        std::ostringstream err_oss;
        err_oss << "In Table_Reader.allocate_result_columns(): "
                << "Cannot read column " << col << ", there are only "
                << number_of_columns << "in the input file.";
        throw std::runtime_error{err_oss.str()};
      }

    return return_type(columns.size());
  }

  void push_back_row(return_type &table, const std::vector<Value_Type> &row)
  {
    if (columns.empty())
      for (auto i = 0; i < table.size(); ++i)
        table[i].push_back(row[i]);
    else
      for (auto i = 0; i < columns.size(); ++i)
        table[i].push_back(row[columns[i]]);
  }

private:
  std::vector<char> comment_chars;
  std::vector<std::size_t> columns;
  std::ifstream ifs;
};

} // namespace io
} // namespace irubataru

#endif /* IRUBATARU_IO_HPP */
