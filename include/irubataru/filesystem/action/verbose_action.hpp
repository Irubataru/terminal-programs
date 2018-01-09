
/*
 * Created: 09-01-2018
 * Modified: Wed 10 Jan 2018 13:19:00 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef FILESYSTEM_VERBOSE_ACTION_HPP
#define FILESYSTEM_VERBOSE_ACTION_HPP

#include "action.hpp"
#include <iostream>

namespace irubataru {
namespace filesystem {

class Verbose_Action : public Action
{
public:
  Verbose_Action(std::unique_ptr<Action> acting_action)
      : acting_action_ptr_{std::move(acting_action)} {};

  Verbose_Action(Action *&&acting_action)
      : acting_action_ptr_{acting_action} {};

  std::string name() const override
  {
    static const std::string name_{""};
    return name_;
  }

  void apply(boost_fs::path const &file_path, std::regex const &regex,
             std::string const &replace_str) const override
  {
    std::cout << "(" << acting_action_ptr_->name() << ") " << file_path;

    if (acting_action_ptr_->is_from_to_action()) {
      auto new_file = acting_action_ptr_->replace_filename(file_path, regex, replace_str);

      if (new_file.string() == file_path.string())
        return;

      std::cout << " -> " << new_file << "\n";
    }

    acting_action_ptr_->apply(file_path, regex, replace_str);
  }

  virtual ~Verbose_Action(){};

private:
  std::unique_ptr<Action> acting_action_ptr_;
};

} // namespace filesystem
} // namespace irubataru

#endif /* FILESYSTEM_VERBOSE_ACTION_HPP */
