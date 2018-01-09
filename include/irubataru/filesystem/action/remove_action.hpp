
/*
 * Created: 10-01-2018
 * Modified: Wed 10 Jan 2018 13:25:02 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef FILESYSTEM_REMOVE_ACTION_HPP
#define FILESYSTEM_REMOVE_ACTION_HPP

#include "action.hpp"
#include <iostream>

namespace irubataru {
namespace filesystem {

class Remove_Action : public Action
{
public:
  std::string name() const override
  {
    static const std::string name_{"rm"};
    return name_;
  }

  void apply(boost_fs::path const &file_path, std::regex const &regex,
             std::string const &replace_str) const override
  {
    boost_fs::remove(file_path);
  }

  virtual ~Remove_Action(){};
};

} // namespace filesystem
} // namespace irubataru

#endif /* FILESYSTEM_REMOVE_ACTION_HPP */
