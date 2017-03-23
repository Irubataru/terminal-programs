
/*
 * Created: 23-03-2017
 * Modified: Thu 23 Mar 2017 16:11:25 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef IRUBATARU_MATH_STATISTICS_KOLMOGOROV_SMIRNOV_DISTRIBUTION_HPP
#define IRUBATARU_MATH_STATISTICS_KOLMOGOROV_SMIRNOV_DISTRIBUTION_HPP

#include "continuous_distributions.hpp"
#include <cmath>

namespace irubataru {
namespace math {
namespace statistics {

// The Kolmogorov Smirnov distribution function, necessary for the distribution
// test by the same name, is a fairly straight forward implementation of the
// numerical recipes code (chapter 6.14.12 in the third edition)
//
// TODO: Actually make this readable
class Kolmogorov_Smirnov_Distribution : public Continuous_Distribution
{
public:
  // Return the cumulative distribution function
  virtual value_type cdf(value_type x) override
  {
    using std::sqrt;
    using std::pow;
    using std::exp;
    using std::log;

    if (x < 0.)
      throw std::runtime_error{"In " + name() +
                               ", cdf(x):\n\tx must be larger than 0"};
    else if (x == 0.)
      return 1.;

    // Use four terms of the series:
    // \frac{\sqrt{2pi}}{x} \sum_{j=1}^{\inf} \exp(-\frac{2j-1}^2 \pi^2}{8 x^2})
    if (x < 1.18) {
      auto y = exp(-M_PI * M_PI / (8. * x * x));
      return (sqrt(2. * M_PI) / x) *
             (y + pow(y, 9.) + pow(y, 25.) + pow(y, 49));

      // Use three terms of the alternate series:
      // 1 - 2 \sum_{j=1}^{\inf} (-1)^{j-1} \exp( -2j^2 x^2 )
    } else {
      auto y = exp(-2. * x * x);
      return 1. - 2. * (y - pow(y, 4.) + pow(y, 9.));
    }
  }

  // Return the complementary cumulative distribution function
  virtual value_type comp_cdf(value_type x) override
  {
    using std::exp;
    using std::pow;

    if (x < 0.)
      throw std::runtime_error{"In " + name() +
                               ", comp_cdf(x):\n\tx must be larger than 0"};
    else if (x == 0.)
      return 0.;

    if (x < 1.18)
      return 1. - cdf(x);

    auto y = exp(-2. * x * x);
    return 2. * (y - pow(y, 4.) + pow(y, 9.));
  }

  // Return the inverse of the cumulative distribution function
  virtual value_type inv_cdf(value_type p) override
  {
    return inv_comp_cdf(1. - p);
  }

  // Return the inverse of the complementary cumulative distribution function
  virtual value_type inv_comp_cdf(value_type q) override
  {
    using std::abs;
    using std::sqrt;
    using std::log;
    using std::pow;
    using std::max;

    if (q <= 0. or q > 1.)
      throw std::runtime_error{
          "In " + name() +
          ", inv_comp_cdf(q): q must be in the interval (0., 1.]"};

    if (q == 1.)
      return 0.;

    // Solve
    // Q = 1 - \sqrt{2\pi}/x \sum_{j=1}^{\inf} \exp(-([2j-1]^2 \pi^2)/(8 x^2))
    //
    // by solving
    // y log(y) = -pi (1-Q)^2 / 8 (1 + y^4 + y^12 + ...)^-1
    // x(Q) = pi/2 / sqrt(-log(y))
    //
    // This equation is solved by a variant of Halley's method
    if (q > 0.3) {
      auto f = -M_PI * square(1. - q) / 8.;
      auto sol = invxlogx(f);

      value_type newton{0.};

      do {
        auto log_sol = log(sol);
        auto ff = f / square(1. + pow(sol, 4.) + pow(sol, 12.));
        auto u = (sol * log_sol - ff) / (1. + log_sol);
        newton = u / max(0.5, 1. - 0.5 * u / (sol * (1. + log_sol)));
        sol = sol - newton;
      } while (abs(newton / sol) > 1e-15);

      return M_PI / (2. * sqrt(-log(sol)));

      // Inverse of:
      // Q = 2 \sum_{j=1}^{\inf} (-1)^{j-1} \exp( -2j^2 x^2 )
      //
      // Computed iteratively using the iteration:
      // y0 = 0.0
      // y_{i+1} = Q/2 + y_i^4 - y_i^9 + y_i^16 - y_i^25 + ...
      // x(Q) = sqrt(-0.5*log(y_inf))
    } else {
      value_type sol{0.03}, sol_prev{0.};

      do {
        sol_prev = sol;
        sol = 0.5 * q + pow(sol, 4.) - pow(sol, 9.);
        if (sol > 0.06)
          sol += pow(sol, 16.) - pow(sol, 25.);

      } while (abs((sol - sol_prev) / sol) > 1e-15);

      return sqrt(-0.5 * log(sol));
    }
  }

  virtual std::string name() const override
  {
    static const auto name = std::string{"Kolmogorov_Smirnov_Distribution"};
    return name;
  }

private:
  // Finds the solution to the equation x = y * log(y), necessary for the
  // inverse cdf functions, from numerical recipes (3rd) section 6.11
  value_type invxlogx(value_type x)
  {
    using std::exp;
    using std::log;
    using std::sqrt;
    using std::abs;

    static const auto one_over_e = exp(-1.);

    if (x >= 0. || x <= -one_over_e)
      throw std::runtime_error{"In " + name() +
                               ", invxlogx(x): no inverse value"};

    value_type sol{0.};

    // First approximation by inverse Taylor series
    if (x < -0.2)
      sol = log(one_over_e - sqrt(2. * one_over_e * (x + one_over_e)));
    else
      sol = -10.;

    value_type newton{0.}, newton_prev{0.};

    do {
      newton = (log(x / sol) - sol) * (sol / (1. + sol));
      sol += newton;

      if (newton < 1e-8 and abs(newton + newton_prev) < 0.01 * abs(newton))
        break;

      newton_prev = newton;

    } while (abs(newton / sol) > 1e-15);

    return exp(sol);
  }

  value_type square(value_type x) { return x * x; }
};

} // namespace statistics
} // namespace math
} // namespace irubataru

#endif /* IRUBATARU_MATH_STATISTICS_KOLMOGOROV_SMIRNOV_DISTRIBUTION_HPP */
