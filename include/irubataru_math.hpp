
/*
 * Created: 27-01-2017
 * Modified: Fri 27 Jan 2017 11:25:31 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef IRUBATARU_MATH_HPP
#define IRUBATARU_MATH_HPP

#include <algorithm>
#include <iterator>

namespace irubataru
{
namespace math
{
namespace statistics
{

template <typename Iterator>
typename std::iterator_traits<Iterator>::value_type mean(Iterator begin,
                                                         Iterator end)
{
  using T = typename std::iterator_traits<Iterator>::value_type;

  auto size = std::distance(begin, end);

  if (size == 0)
    return T{0};

  return std::accumulate(begin, end, T{0.}) / static_cast<T>(size);
}

template <typename Iterator, typename Unary_Function>
typename std::iterator_traits<Iterator>::value_type
mean_f(Iterator begin, Iterator end, Unary_Function unary)
{
  using T = typename std::iterator_traits<Iterator>::value_type;

  auto size = std::distance(begin, end);

  if (size == 0)
    return T{0};

  return std::accumulate(begin, end, T{0.},
                         [unary = std::move(unary)](T init, T val) {
                           return init + unary(val);
                         }) /
         static_cast<T>(size);
}

template <typename Iterator>
typename std::iterator_traits<Iterator>::value_type
standard_deviation(Iterator begin, Iterator end)
{
  using std::sqrt;
  using T = typename std::iterator_traits<Iterator>::value_type;

  auto av1 = mean(begin, end);
  auto av2 = mean_f(begin, end, [](T val) { return val * val; });

  return sqrt(av2 - av1 * av1);
}

} // namespace statistics
} // namespace math
} // namespace irubataru

#endif /* IRUBATARU_MATH_HPP */
