
/*
 * Created: 23-03-2017
 * Modified: Thu 23 Mar 2017 14:42:34 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef IRUBATARU_MATH_STATISTICS_DISTRIBUTIONS_HPP
#define IRUBATARU_MATH_STATISTICS_DISTRIBUTIONS_HPP

#include <stdexcept>
#include <string>

namespace irubataru {
namespace math {
namespace statistics {

class Function_Not_Implemented_Error : std::runtime_error
{
public:
  Function_Not_Implemented_Error(std::string distribution_name,
                                 std::string function_name)
      : std::runtime_error{"Function " + function_name +
                           " not implemented in " + distribution_name}
  {
  }
};

class Continuous_Distribution
{
public:
  using value_type = double;

  // Return the probability distribution function
  virtual value_type pdf(value_type x)
  {
    function_not_implemented("pdf");
    return 0.;
  }

  // Return the cumulative distribution function
  virtual value_type cdf(value_type x)
  {
    function_not_implemented("cdf");
    return 0.;
  }

  // Return the complementary cumulative distribution function
  virtual value_type comp_cdf(value_type x)
  {
    function_not_implemented("comp_cdf");
    return 0.;
  }

  // Return the inverse of the cumulative distribution function
  virtual value_type inv_cdf(value_type x)
  {
    function_not_implemented("inv_cdf");
    return 0.;
  }

  // Return the inverse of the complementary cumulative distribution function
  virtual value_type inv_comp_cdf(value_type x)
  {
    function_not_implemented("inv_comp_cdf");
    return 0.;
  }

  // Return the name of the current distribition function
  virtual std::string name() const
  {
    static const auto name = std::string{"Continuous_Distribution"};
    return name;
  }

private:

  // Throw a Function_Not_Implemented_Error error, default action for all the 
  // various distribution functions in case they aren't (or can't be) defined
  void function_not_implemented(std::string function_name)
  {
    throw Function_Not_Implemented_Error{name(), function_name};
  }
};

} // namespace statistics
} // namespace math
} // namespace irubataru

#endif /* IRUBATARU_MATH_STATISTICS_DISTRIBUTIONS_HPP */
