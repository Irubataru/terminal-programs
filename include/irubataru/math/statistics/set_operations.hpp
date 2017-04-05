
/*
 * Created: 05-04-2017
 * Modified: Wed 05 Apr 2017 12:28:20 BST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef IRUBATARU_MATH_STAT_SET_OPERATIONS_HPP
#define IRUBATARU_MATH_STAT_SET_OPERATIONS_HPP

#include "distribution_properties.hpp"
#include <vector>

namespace irubataru {
namespace math {
namespace statistics {

template <typename Type>
std::vector<Type> bin_data(const std::vector<Type> &data, std::size_t bin)
{
  if (bin == 0)
    throw std::runtime_error{"bin_data(vector, size): bin size cannot be zero"};
  else if (bin == 1)
    return data;

  auto result = std::vector<Type>{};

  for (auto it = data.begin(); (it+bin) <= data.end(); it += bin)
    result.push_back(mean(it,it+bin));
  
  return result;
};

} // namespace statistics
} // namespace math
} // namespace irubataru

#endif /* IRUBATARU_MATH_STAT_SET_OPERATIONS_HPP */
