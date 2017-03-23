
/*
 * Created: 27-01-2017
 * Modified: Thu 23 Mar 2017 14:08:28 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef IRUBATARU_MATH_STAT_DISTRIBUTION_PROPERTIES_HPP
#define IRUBATARU_MATH_STAT_DISTRIBUTION_PROPERTIES_HPP

#include <algorithm>
#include <iterator>

namespace irubataru {
namespace math {
namespace statistics {

template <typename Iterator>
inline typename std::iterator_traits<Iterator>::value_type mean(Iterator begin,
                                                                Iterator end)
{
  using T = typename std::iterator_traits<Iterator>::value_type;

  auto size = std::distance(begin, end);

  if (size == 0)
    return T{0};

  return std::accumulate(begin, end, T{0.}) / static_cast<T>(size);
}

template <typename Iterator, typename Unary_Function>
inline typename std::iterator_traits<Iterator>::value_type
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
inline typename std::iterator_traits<Iterator>::value_type
standard_deviation(Iterator begin, Iterator end)
{
  using std::sqrt;
  using T = typename std::iterator_traits<Iterator>::value_type;

  auto size = std::distance(begin, end);

  if (size <= 1)
    return T{0};

  auto av1 = mean(begin, end);
  auto av2 = mean_f(begin, end, [](T val) { return val * val; });

  return sqrt((av2 - av1 * av1) / static_cast<T>(size - 1ul));
}

template <typename Iterator, typename Unary_Function>
inline typename std::iterator_traits<Iterator>::value_type
jackknife_average(Iterator begin, Iterator end, Unary_Function unary)
{
  using T = typename std::iterator_traits<Iterator>::value_type;

  auto size = std::distance(begin, end);

  if (size == 0)
    return T{0};
  else if (size == 1u)
    return *begin;

  auto total = std::accumulate(
      begin, end, T{0.},
      [unary](const auto &init, auto val) { return init + unary(val); });

  auto jack_mean = std::accumulate(begin, end, T{0.},
                                   [unary, total](const auto &init, auto val) {
                                     return init + (total - unary(val));
                                   }) /
                   T{size - 1u};

  return total - T{size - 1u} * (jack_mean / T{size});
}

template <typename Iterator>
inline typename std::iterator_traits<Iterator>::value_type
jackknife_average(Iterator begin, Iterator end)
{
  return jackknife_average(begin, end, [](auto v) { return v; });
}

template <typename Iterator, typename Unary_Function>
inline typename std::iterator_traits<Iterator>::value_type
jackknife_error(Iterator begin, Iterator end, Unary_Function unary)
{
  using std::sqrt;
  using T = typename std::iterator_traits<Iterator>::value_type;

  auto size = std::distance(begin, end);

  if (size < 2u)
    return T{0};

  auto total = std::accumulate(
      begin, end, T{0.},
      [unary](const auto &init, auto val) { return init + unary(val); });

  auto jack_mean = std::accumulate(begin, end, T{0.},
                                   [unary, total](const auto &init, auto val) {
                                     return init + (total - unary(val));
                                   }) /
                   T{size - 1u};

  return total - T{size - 1u} * (jack_mean / T{size});
}

template <typename Iterator>
inline typename std::iterator_traits<Iterator>::value_type
jackknife_error(Iterator begin, Iterator end)
{
  return jackknife_error(begin, end, [](auto v) { return v; });
}

} // namespace statistics
} // namespace math
} // namespace irubataru

#endif /* IRUBATARU_MATH_STAT_DISTRIBUTION_PROPERTIES_HPP */
