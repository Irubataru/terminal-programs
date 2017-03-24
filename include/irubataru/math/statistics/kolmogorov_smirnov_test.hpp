
/*
 * Created: 23-03-2017
 * Modified: Thu 23 Mar 2017 17:35:56 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef IRUBATARU_MATH_STATISTICS_KOLMOGOROV_SMIRNOV_TEST_HPP
#define IRUBATARU_MATH_STATISTICS_KOLMOGOROV_SMIRNOV_TEST_HPP

#include "kolmogorov_smirnov_distribution.hpp"
#include <algorithm>
#include <vector>

namespace irubataru {
namespace math {
namespace statistics {

struct Kolmogorov_Smirnov_Return_Type
{
  double d_value, p_value;
};

auto Kolmogorov_Smirnov_Test(std::vector<double> set1, std::vector<double> set2)
{
  using std::abs;
  using std::sqrt;
  using value_type = Kolmogorov_Smirnov_Distribution::value_type;

  Kolmogorov_Smirnov_Return_Type result{0., 0.};

  std::sort(set1.begin(), set1.end());
  std::sort(set2.begin(), set2.end());

  auto dn1 = static_cast<value_type>(set1.size()),
       dn2 = static_cast<value_type>(set2.size());

  value_type fn1{0.}, fn2{0.};
  std::size_t i1{0ul}, i2{0ul};

  while (i1 < set1.size() and i2 < set2.size()) {
    auto d1 = set1[i1], d2 = set2[i2];

    if (d1 <= d2) {
      do {
        ++i1;
        fn1 = i1 / dn1;
      } while (i1 < set1.size() and set1[i1] == d1);
    }

    if (d2 <= d1) {
      do {
        ++i2;
        fn2 = i2 / dn2;
      } while (i2 < set2.size() and set2[i2] == d2);
    }

    if (abs(fn2 - fn1) > result.d_value)
      result.d_value = abs(fn2 - fn1);
  }

  Kolmogorov_Smirnov_Distribution dist;

  auto effective_size = sqrt(dn1 * dn2 / (dn1 + dn2));

  result.p_value = dist.comp_cdf(
      result.d_value * (effective_size + 0.12 + 0.11 / effective_size));
  return result;
}

} // namespace statistics
} // namespace math
} // namespace irubataru

#endif /* IRUBATARU_MATH_STATISTICS_KOLMOGOROV_SMIRNOV_TEST_HPP */
