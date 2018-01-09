
/*
 * Created: 09-01-2018
 * Modified: Wed 10 Jan 2018 12:11:46 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef FIND_FILE_HPP
#define FIND_FILE_HPP

#include "action/action.hpp"

namespace irubataru {
namespace filesystem {

enum class Match_Type
{
  Match,
  Search
};

struct Find_And_Apply_Options
{
  bool follow_symlinks;
  bool use_absolute_path;
  bool use_hidden_files;
  Match_Type match_type;
};

struct Regex_Args
{
  std::regex find_regex;
  std::regex match_regex;
  std::string replace_str;
};

inline bool is_hidden(boost_fs::path const &file_path)
{
  auto filename = file_path.filename().string();
  return (filename != "." and filename != ".." and filename[0] == '.');
}

inline bool matching_file(boost_fs::path const &file_path,
                          std::regex const &find_regex,
                          Find_And_Apply_Options const &opt)
{
  try {
    switch (opt.match_type) {
    case Match_Type::Match:
      return std::regex_match(file_path.string(), find_regex);
    case Match_Type::Search:
      return std::regex_search(file_path.string(), find_regex);
    default:
      throw std::runtime_error{"matching_file: Invalid Match_Type"};
    }
  } catch (std::regex_error &err) {
    std::ostringstream oss;
    oss << "Invalid regex in the search filter \n"
        << "regex_error: \n"
        << err.what();

    throw std::runtime_error{oss.str()};
  }
}

inline void find_and_apply(boost_fs::path const &root_path,
                           Regex_Args const &reg_args, Action const &action,
                           Find_And_Apply_Options opt)
{
  if (!opt.use_hidden_files and is_hidden(root_path))
    return;

  if (boost_fs::is_directory(root_path)) {
    for (auto const &dir_paths : boost_fs::directory_iterator{root_path})
      find_and_apply(dir_paths, reg_args, action, opt);

  } else if (boost_fs::is_symlink(root_path) and opt.follow_symlinks) {
    throw std::runtime_error{"Symlink following not yet implemented"};

  } else if (boost_fs::is_regular_file(root_path)) {
    auto match_path =
        opt.use_absolute_path ? boost_fs::absolute(root_path) : root_path;

    if (matching_file(match_path, reg_args.find_regex, opt)) {
      action.apply(match_path, reg_args.match_regex, reg_args.replace_str);
    }
  }
}

} // namespace filesystem
} // namespace irubataru

#endif /* FIND_FILE_HPP */
