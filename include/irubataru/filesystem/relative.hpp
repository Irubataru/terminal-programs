
/*
 * Created: 10-01-2018
 * Modified: Thu 11 Jan 2018 11:33:34 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef FILESYSTEM_RELATIVE_HPP
#define FILESYSTEM_RELATIVE_HPP

#include <boost/filesystem.hpp>

namespace boost_fs = ::boost::filesystem;

namespace irubataru {
namespace filesystem {

inline boost_fs::path relative(boost_fs::path p, boost_fs::path base)
{
  // First sanitize the paths
  p = boost_fs::canonical(p);
  base = boost_fs::canonical(base);

  auto p_iter = p.begin();
  auto base_iter = base.begin();

  // Loop through both while they are the same to find nearest common directory
  while (base_iter != base.end() && p_iter != p.end() &&
         (*p_iter) == (*base_iter)) {
    ++p_iter;
    ++base_iter;
  }

  // Replace base path segments with '..' (base => nearest common directory)
  boost_fs::path final_path;
  while (base_iter != base.end()) {
    final_path /= "..";
    ++base_iter;
  }

  // Append the remainder of the p path (nearest common directory => p)
  while (p_iter != p.end()) {
    final_path /= *p_iter;
    ++p_iter;
  }

  if (final_path.empty())
    final_path /= ".";

  return final_path;
}

inline boost_fs::path relative(boost_fs::path const &p)
{
  return relative(p, boost_fs::current_path());
}

} // namespace filesystem
} // namespace irubataru

#endif /* FILESYSTEM_RELATIVE_HPP */
