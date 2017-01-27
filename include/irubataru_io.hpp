
/*
 * Created: 27-01-2017
 * Modified: Fri 27 Jan 2017 11:29:06 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef IRUBATARU_IO_HPP
#define IRUBATARU_IO_HPP

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <vector>

namespace irubataru
{
namespace io
{

template <typename Value_Type>
class TableReader
{
public:
  using return_type = std::vector<std::vector<Value_Type>>;

  explicit TableReader() : TableReader({'#'}) {}

  explicit TableReader(std::vector<char> comments)
      : comment_chars{std::move(comments)}
  {}

  return_type Read(std::string filename, std::size_t skip_lines = 0,
                   std::size_t read_every = 0)
  {
    ifs.open(filename);
    if (!ifs)
      throw std::runtime_error{"Cannot open file \"" + filename +
                               "\" for reading."};

    read_preamble(skip_lines);
    if (!ifs)
      return {};

    return read_body(read_every);
  }

private:
  void read_preamble(std::size_t skip_lines)
  {
    auto lines_read = std::size_t{0ul};

    while (ifs and lines_read < skip_lines)
    {
      if (!is_comment())
        ++lines_read;

      discard_line();
    }

    discard_comments();
  }

  return_type read_body(std::size_t read_every)
  {
    auto result = return_type(determine_columns());

    auto read_lines = std::size_t{0ul};
    while (ifs)
    {
      if (is_comment())
      {
        discard_line();
        continue;
      }

      ++read_lines;
      if ((read_lines - 1) % read_every != 0)
      {
        discard_line();
        continue;
      }

      auto row = read_row(result.size());

      if (row.empty())
        continue;

      for (auto i = 0; i < result.size(); ++i)
        result[i].push_back(row[i]);
    }

    return result;
  }

  std::vector<Value_Type> read_row(std::size_t columns)
  {
    std::string single_line{};
    std::getline(ifs, single_line);

    if (single_line.empty())
      return {};

    auto str_vec = std::vector<std::string>{};
    boost::split(str_vec, single_line, boost::is_any_of("\t "));

    if (str_vec.size() != columns)
    {
      std::ostringstream oss;
      oss << "Inconsistent table format. Columns: " << columns << ", "
          << "current row has " << str_vec.size() << " columns.\n"
          << "Row is: " << single_line;
      throw std::runtime_error{oss.str()};
    }

    auto row = std::vector<Value_Type>(columns);

    std::transform(
        str_vec.begin(), str_vec.end(), row.begin(),
        [](std::string s) { return boost::lexical_cast<double>(s); });

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

  std::size_t determine_columns()
  {
    auto init_pos = ifs.tellg();

    auto one_line = std::string{};
    std::getline(ifs, one_line);

    ifs.seekg(init_pos);
    return count_columns(one_line);
  }

  std::size_t count_columns(std::string str)
  {
    boost::trim_left(str);
    boost::trim_right(str);
    return std::count(str.begin(), str.end(), ' ') + 1;
  }

private:
  std::vector<char> comment_chars;
  std::ifstream ifs;
};

} // namespace io
} // namespace irubataru

#endif /* IRUBATARU_IO_HPP */
