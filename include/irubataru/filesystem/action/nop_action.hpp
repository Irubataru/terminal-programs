
/*
 * Created: 08-01-2018
 * Modified: Tue 09 Jan 2018 11:57:20 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef FILESYSTEM_NOP_ACTION_HPP
#define FILESYSTEM_NOP_ACTION_HPP

#include "action.hpp"

namespace irubataru {
namespace filesystem {

class NOP_Action : public Action
{
public:
  NOP_Action(std::unique_ptr<Action> imitated_action)
      : imitated_action_ptr_{std::move(imitated_action)} {};

  NOP_Action(Action *&&imitated_action)
      : imitated_action_ptr_{imitated_action} {};

  std::string name() const override { return imitated_action_ptr_->name(); }

  bool is_from_to_action() const override
  {
    return imitated_action_ptr_->is_from_to_action();
  }

  void apply(boost_fs::path const &file_path, std::regex const &regex,
             std::string const &replace_str) const override{};

  virtual ~NOP_Action(){};

private:
  std::unique_ptr<Action> imitated_action_ptr_;
};

} // namespace filesystem
} // namespace irubataru

#endif /* FILESYSTEM_NOP_ACTION_HPP */
