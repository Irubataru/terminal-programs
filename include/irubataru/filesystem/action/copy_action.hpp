
/*
 * Created: 08-01-2018
 * Modified: Wed 10 Jan 2018 13:16:19 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef FILESYSTEM_COPY_ACTION_HPP
#define FILESYSTEM_COPY_ACTION_HPP

#include "action.hpp"

namespace irubataru {
namespace filesystem {

class Copy_Action : public From_To_Action
{
public:
  std::string name() const override
  {
    static const std::string name_{"cp"};
    return name_;
  }

  void apply(boost_fs::path const &file_path, std::regex const &regex,
             std::string const &replace_str) const override
  {
    auto new_file = replace_filename(file_path, regex, replace_str);

    if (new_file.string() == file_path.string())
      return;

    if (boost_fs::exists(new_file)) {
      if (boost_fs::is_directory(new_file)) {
        std::ostringstream oss;
        oss << "The target " << new_file << " exists and is a directory.\n";

        throw std::runtime_error{oss.str()};
      }

      boost_fs::remove(new_file);
    }

    check_and_create_target_dir(new_file);
    boost_fs::copy(file_path, new_file);
  }

  virtual ~Copy_Action(){};
};

} // namespace filesystem
} // namespace irubataru

#endif /* FILESYSTEM_COPY_ACTION_HPP */