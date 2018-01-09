
/*
 * Created: 10-01-2018
 * Modified: Wed 10 Jan 2018 13:22:54 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef FILESYSTEM_LIST_ACTION_HPP
#define FILESYSTEM_LIST_ACTION_HPP

#include "action.hpp"
#include <iostream>

namespace irubataru {
namespace filesystem {

class List_Action : public Action
{
public:
  std::string name() const override
  {
    static const std::string name_{"ls"};
    return name_;
  }

  void apply(boost_fs::path const &file_path, std::regex const &regex,
             std::string const &replace_str) const override
  {
    std::cout << file_path.string() << "\n";
  }

  virtual ~List_Action(){};
};

} // namespace filesystem
} // namespace irubataru

#endif /* FILESYSTEM_LIST_ACTION_HPP */
