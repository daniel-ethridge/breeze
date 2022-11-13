#ifndef _BREEZE_MATH_H_
#define _BREEZE_MATH_H_

#include <vector>
#include <cmath>

namespace Breeze
{

/**
 * @brief Create a polynomial with order n.
 * 
 * Create a functor polynomial of the form a_0 + a_1*x + a_2*x^2 + ... a_n*x^n
 * 
 * @tparam T The primitive type to use for the polynomial (int, double, etc)
 */
template<typename T> 
class Polynomial
{
public:
  /**
   * @brief Construct a new Polynomial object
   * 
   * @param coefficients The coefficients of the polynomial. The first coefficient is for the nth variable. All
   * coefficients, includings 0s, must be passed. For example, to create the polynomial 3.2*x^3 + 2.0*x + 1.2, write
   * Polynomial<double>({3.2, 0.0, 2.0, 1.2});
   */
  Polynomial(std::vector<T> coefficients) :
    m_coefficients(coefficients),
    m_order(static_cast<unsigned int>(coefficients.size() - 1))
  {}

  /**
   * @brief Evaluate a set of inputs with the polynomial
   * 
   * @param inputVec The values to evaluate. 
   * @return std::vector<T> The output values.
   */
  std::vector<T> operator()(const std::vector<T>& inputVec)
  {
    std::vector<T> out;
    out.reserve(inputVec.size());
    for (auto element : inputVec)
      out.emplace_back(function(element));
  }

  /**
   * @brief Evaluate an input with the polynomial
   * 
   * @param inputVal The value to evaluate
   * @return T The output value
   */
  T operator()(T inputVal) { return function(inputVal); }

private:
  std::vector<T> m_coefficients;  ///< Polynomial coefficients
  int m_order;  ///< The order of the polynomial

  /**
   * @brief The polynomial evaluation fuction
   * 
   * @param input The input
   * @return T The output
   */
  T function (T input)
  {
#define SC static_cast

    T sum = SC<T>(0.0);
    for (int i = 0; i <= m_order; i++)
      sum += m_coefficients[i] * SC<T>(std::pow(SC<double>(input), m_order - i));

    return sum;

#undef SC
  }
};

}

#endif  // _BREEZE_MATH_H_
