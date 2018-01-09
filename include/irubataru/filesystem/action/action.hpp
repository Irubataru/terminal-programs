
/*
 * Created: 08-01-2018
 * Modified: Tue 09 Jan 2018 11:58:46 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef FILESYSTEM_ACTION_INTERFACE_HPP
#define FILESYSTEM_ACTION_INTERFACE_HPP

#include <string>
#include <sstream>
#include <regex>
#include <boost/filesystem.hpp>

namespace irubataru {
namespace filesystem {

namespace boost_fs = ::boost::filesystem;

class Action
{
public:
  virtual std::string name() const
  {
    static const std::string name_{"action"};
    return name_;
  }

  virtual bool is_from_to_action() const
  {
    return false;
  }

  boost_fs::path replace_filename(boost_fs::path const &file_path,
                                  std::regex const &regex,
                                  std::string const &replace_str) const
  {
    std::string replace_path;

    try {
      replace_path = std::regex_replace(file_path.string(), regex, replace_str);
    } catch (std::regex_error &err) {
      std::ostringstream oss;
      oss << "Invalid regex + replace string combo passed to " << name() << "\n"
          << "regex_error: \n"
          << err.what();

      throw std::runtime_error{oss.str()};
    }

    return {replace_path};
  }

  void check_and_create_target_dir(boost_fs::path const &target) const
  {
    // Create the parent dir if it doesn't exist
    // TODO: How do I couple this to a verbose variant of the program?
    if (!boost_fs::exists(target.parent_path())) {
      try {
        boost_fs::create_directories(target.parent_path());
      } catch (boost_fs::filesystem_error &err) {
        std::ostringstream oss;
        oss << "Unable to create root directory for the new file " << target
            << "\n"
            << "filesystem_error:\n"
            << err.what();

        throw std::runtime_error{oss.str()};
      }
    } else if (!boost_fs::is_directory(target.parent_path())) {
      std::ostringstream oss;
      oss << "Unable to copy file to directory " << target.parent_path() << "\n"
          << "File already exist and is not a directory";

      throw std::runtime_error{oss.str()};
    }
  }

  virtual void apply(boost_fs::path const &file_path, std::regex const &regex,
                     std::string const &replace_str = "") const = 0;

  virtual ~Action(){};
};

class From_To_Action : public Action
{
public:
  bool is_from_to_action() const override
  {
    return true;
  }

  virtual ~From_To_Action(){};
};

} // namespace filesystem
} // namespace irubataru

#endif /* FILESYSTEM_ACTION_INTERFACE_HPP */
