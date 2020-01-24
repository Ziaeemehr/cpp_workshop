#ifndef ZEST_HPP
#define ZEST_HPP

#if __cplusplus < 201402L
#error "C++14 compatible compiler required"
#else

#include <cmath>
#include <limits>
#include <cassert>
#include <stdexcept>
#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/distributions.hpp>
#include "canonical_float_random.hpp"

template <class Distribution, class URBG, uint_fast16_t N = 1024, typename float_type = double>
class Ziggurat;

namespace zest
{

namespace math
{

static const double pi = 4 * std::atan(1);
static const double half_pi = 2 * std::atan(1);
static const double half_pi_sqrt = std::sqrt(half_pi);
static double sqrt_minus_2_log(double x) { return std::sqrt(-2 * std::log(x)); }

template <typename T>
constexpr T pow(T x, uint_fast8_t n) { return n ? x * pow(x, n - 1) : 1; }

template <typename float_type>
constexpr float_type exp2_int(int exp)
{
  float_type x{1};
  if (exp > 0)
    for (int i = 0; i < exp; ++i)
      x *= 2;
  else if (exp < 0)
    for (int i = 0; i > exp; --i)
      x /= 2.;
  return x;
}

static double ibeta_derivative(double df1, double df2, double x)
{
  // not checking for special cases of df1==0 || df2==0 || x==0 || x==1
  // the first two are disallowed in the FisherF constructor
  // the last two are always satisfied by the arguments used in calls from fisher_f_pdf
  return std::pow(x, df1 - 1) * std::pow(1 - x, df2 - 1);
}

static double fisher_f_pdf(double df1, double df2, double x)
{
  // modified version of boost pdf
  // not using the Lanczos approximation however because we don't want a normalized pdf
  // and the precision is already satisfactory
  // and because the Lanczos approximation is time consuming
  if (x == 0)
  {
    if (df1 > 2)
      return 0;
    else if (df1 == 2)
      return 2 / df2; // beta (1, df2/2) = 2/df2
    else
      std::numeric_limits<double>::infinity();
  }
  double v1x = df1 * x;
  double result;
  if (v1x > df2)
  {
    result = (df2 * df1) / ((df2 + v1x) * (df2 + v1x));
    result *= ibeta_derivative(df2 / 2, df1 / 2, df2 / (df2 + v1x));
  }
  else
  {
    result = df2 + df1 * x;
    result = (result * df1 - x * df1 * df1) / (result * result);
    result *= ibeta_derivative(df1 / 2, df2 / 2, v1x / (df2 + v1x));
  }
  return result;
}

static double fisher_f_cdf(double df1, double df2, double x)
{
  // modified version of boost cdf

  if (x < 0 || std::isinf(x))
    throw std::logic_error("Domain error in fisher_f_cdf");

  double v1x = df1 * x;
  //
  // There are two equivalent formulas used here, the aim is
  // to prevent the final argument to the incomplete beta
  // from being too close to 1: for some values of df1 and df2
  // the rate of change can be arbitrarily large in this area,
  // whilst the value we're passing will have lost information
  // content as a result of being 0.999999something.  Better
  // to switch things around so we're passing 1-z instead.
  //
  return v1x > df2
             ? boost::math::betac(df2 / 2, df1 / 2, df2 / (df2 + v1x))
             : boost::math::beta(df1 / 2, df2 / 2, v1x / (df2 + v1x));
}
static double fisher_f_ccdf(double df1, double df2, double x)
{
  // modified version of boost cdf

  if (x < 0 || std::isinf(x))
    throw std::logic_error("Domain error in fisher_f_ccdf");

  double v1x = df1 * x;
  //
  // There are two equivalent formulas used here, the aim is
  // to prevent the final argument to the incomplete beta
  // from being too close to 1: for some values of df1 and df2
  // the rate of change can be arbitrarily large in this area,
  // whilst the value we're passing will have lost information
  // content as a result of being 0.999999something.  Better
  // to switch things around so we're passing 1-z instead.
  //
  return v1x > df2
             ? boost::math::beta(df2 / 2, df1 / 2, df2 / (df2 + v1x))
             : boost::math::betac(df1 / 2, df2 / 2, v1x / (df2 + v1x));
}

} // namespace math

enum class DistCategory : uint_fast8_t
{
  STRICTLY_DECREASING,
  STRICTLY_INCREASING,
  SYMMETRIC,
  ASYMMETRIC
};
enum class TailCategory : uint_fast8_t
{
  FINITE,
  MAP,
  MAP_REJECT
};

class Normal
{
  const double stddev;

public:
  const double mode;
  static constexpr bool is_mode_unbounded = false;
  static constexpr auto dist_category = DistCategory::SYMMETRIC;
  static constexpr auto tail_category = TailCategory::MAP_REJECT;
  constexpr Normal(double mode = 0.0, double stddev = 1.0) : mode{mode}, stddev{stddev}
  {
    if (stddev <= 0)
      throw std::logic_error("stddev must be positive");
  }
  double pdf(double x) const { return std::exp(-(x - mode) * (x - mode) / (2. * stddev * stddev)); }
  double ccdf(double x) const { return math::half_pi_sqrt * stddev * std::erfc((x - mode) / stddev * std::sqrt(.5)); }
  double strip_area(double x) const { return (x - mode) * pdf(x) + ccdf(x); }
  double tail_value_rel_mode(double tail_start_rel_mode, double u) const
  {
    return std::sqrt(tail_start_rel_mode * tail_start_rel_mode - 2 * stddev * stddev * std::log(u));
  }
  double tail_accept_probability(double tail_start_rel_mode, double x_rel_mode) const
  {
    return tail_start_rel_mode / x_rel_mode;
  }
};

class StandardNormal
{
public:
  static constexpr double mode = 0.0;
  static constexpr bool is_mode_unbounded = false;
  static constexpr auto dist_category = DistCategory::SYMMETRIC;
  static constexpr auto tail_category = TailCategory::MAP_REJECT;
  static double pdf(double x) { return std::exp(-0.5 * x * x); }
  static double ccdf(double x) { return math::half_pi_sqrt * std::erfc(x * std::sqrt(.5)); }
  static double strip_area(double x) { return x * pdf(x) + ccdf(x); }
  static double tail_value_rel_mode(double tail_start_rel_mode, double u)
  {
    return std::sqrt(tail_start_rel_mode * tail_start_rel_mode - 2 * std::log(u));
  }
  static double tail_accept_probability(double tail_start_rel_mode, double x_rel_mode)
  {
    return tail_start_rel_mode / x_rel_mode;
  }
};

class StudentT
{
public:
  const double dof, normalizing_const_inverse;
  const boost::math::students_t_distribution<double> boost_math_dist;
  static constexpr double mode = 0.0;
  static constexpr bool is_mode_unbounded = false;
  static constexpr auto dist_category = DistCategory::SYMMETRIC;
  static constexpr auto tail_category = TailCategory::MAP_REJECT;
  StudentT(double dof) : dof{dof}, normalizing_const_inverse{std::exp(std::lgamma(dof / 2) - std::lgamma((dof + 1) / 2) + 0.5 * std::log(zest::math::pi * dof))}, boost_math_dist{dof}
  {
    if (dof <= 0)
      throw std::logic_error("dof must be positive");
  }
  double pdf(double x) const { return std::pow(1. + x * x / dof, -(dof + 1) / 2); }
  double ccdf(double x) const
  {
    return normalizing_const_inverse * (1 - boost::math::cdf(boost_math_dist, x));
  }
  double strip_area(double x) const { return x * pdf(x) + ccdf(x); }
  double tail_value_rel_mode(double tail_start, double u) const
  {
    return std::sqrt(std::pow(u, -2. / dof) * (dof + tail_start * tail_start) - dof);
  }
  double tail_accept_probability(double tail_start, double x) const
  {
    return std::sqrt((1. + dof / (x * x)) / (1. + dof / (tail_start * tail_start)));
  }
};

class Exponential
{
  const double inverse_scale;

public:
  const double mode;
  static constexpr bool is_mode_unbounded = false;
  static constexpr auto dist_category = DistCategory::STRICTLY_DECREASING;
  static constexpr auto tail_category = TailCategory::MAP;
  constexpr Exponential(double mode = 0.0, double inverse_scale = 1.0) : mode{mode}, inverse_scale{inverse_scale}
  {
    if (inverse_scale <= 0)
      throw std::logic_error("inverse_scale must be positive");
  }
  double pdf(double x) const { return std::exp(-(x - mode) * inverse_scale); }
  double ccdf(double x) const { return std::exp(-(x - mode) * inverse_scale) / inverse_scale; }
  double strip_area(double x) const { return (x - mode) * pdf(x) + ccdf(x); }
  double tail_value_rel_mode(double tail_start_rel_mode, double u) const
  {
    return tail_start_rel_mode - std::log(u) / inverse_scale;
  }
};

class StandardExponential
{
public:
  static constexpr double mode = 0;
  static constexpr bool is_mode_unbounded = false;
  static constexpr auto dist_category = DistCategory::STRICTLY_DECREASING;
  static constexpr auto tail_category = TailCategory::MAP;
  static double pdf(double x) { return std::exp(-x); }
  static double ccdf(double x) { return std::exp(-x); }
  static double strip_area(double x) { return x * pdf(x) + ccdf(x); }
  static double tail_value_rel_mode(double tail_start, double u) { return tail_start - std::log(u); }
};

class Laplace
{
public:
  const double mode, inverse_scale;
  static constexpr bool is_mode_unbounded = false;
  static constexpr auto dist_category = DistCategory::SYMMETRIC;
  static constexpr auto tail_category = TailCategory::MAP;
  constexpr Laplace(double mode = 0.0, double inverse_scale = 1.0) : mode{mode}, inverse_scale{inverse_scale}
  {
    if (inverse_scale <= 0)
      throw std::logic_error("inverse_scale must be positive");
  }
  double pdf(double x) const { return std::exp(-std::abs(x - mode) * inverse_scale); }
  double ccdf(double x) const { return std::exp(-(x - mode) * inverse_scale) / inverse_scale; }
  double strip_area(double x) const { return (x - mode) * pdf(x) + ccdf(x); }
  double tail_value_rel_mode(double tail_start_rel_mode, double u) const
  {
    return tail_start_rel_mode - std::log(u) / inverse_scale;
  }
};

class StandardLaplace
{
public:
  static constexpr double mode = 0;
  static constexpr bool is_mode_unbounded = false;
  static constexpr auto dist_category = DistCategory::SYMMETRIC;
  static constexpr auto tail_category = TailCategory::MAP;
  static double pdf(double x) { return std::exp(-std::abs(x)); }
  static double ccdf(double x) { return std::exp(-std::abs(x)); }
  static double strip_area(double x) { return x * pdf(x) + ccdf(x); }
  static double tail_value_rel_mode(double tail_start, double u) { return tail_start - std::log(u); }
};

class Cauchy
{
  const double scale;

public:
  const double mode;
  static constexpr bool is_mode_unbounded = false;
  static constexpr auto dist_category = DistCategory::SYMMETRIC;
  static constexpr auto tail_category = TailCategory::MAP;
  constexpr Cauchy(double mode = 0.0, double scale = 1.0) : mode{mode}, scale{scale}
  {
    if (scale <= 0)
      throw std::logic_error("scale must be positive");
  }
  constexpr double pdf(double x) const { return 1. / (1. + ((x - mode) / scale) * ((x - mode) / scale)); }
  double ccdf(double x) const { return (math::half_pi - std::atan((x - mode) / scale)) * scale; }
  double strip_area(double x) const { return (x - mode) * pdf(x) + ccdf(x); }
  double ccdf_inv(double y) const { return scale * std::tan(math::half_pi - y / scale) + mode; }
  double tail_value_rel_mode(double tail_start_rel_mode, double u) const
  {
    return ccdf_inv(u * ccdf(tail_start_rel_mode));
  }
};

class StandardCauchy
{
public:
  static constexpr double mode = 0;
  static constexpr bool is_mode_unbounded = false;
  static constexpr auto dist_category = DistCategory::SYMMETRIC;
  static constexpr auto tail_category = TailCategory::MAP;
  static constexpr double pdf(double x) { return 1. / (1. + x * x); }
  static double ccdf(double x) { return math::half_pi - std::atan(x); }
  static double strip_area(double x) { return x * pdf(x) + ccdf(x); }
  static double ccdf_inv(double y) { return std::tan(math::half_pi - y); }
  static double tail_value_rel_mode(double tail_start, double u) { return ccdf_inv(u * ccdf(tail_start)); }
};

class SemiCircle
{
public:
  const double mode, radius;
  const double support;
  static constexpr bool is_mode_unbounded = false;
  static constexpr auto dist_category = DistCategory::SYMMETRIC;
  static constexpr auto tail_category = TailCategory::FINITE;
  constexpr SemiCircle(double mode = 0.0, double radius = 1.0) : mode{mode}, radius{radius}, support{mode + radius}
  {
    if (radius < 0)
      throw std::logic_error("negative radius");
  }
  double pdf(double x) const { return std::sqrt(radius * radius - (x - mode) * (x - mode)); }
  double ccdf(double x) const
  {
    return math::pi * radius * radius / 4 - (x - mode) * pdf(x) / 2 - radius * radius * std::asin((x - mode) / radius) / 2;
  }
  double strip_area(double x) const { return (x - mode) * pdf(x) + ccdf(x); }
};

class Triangular
{
public:
  const double mode, start, end;
  static constexpr bool is_mode_unbounded = false;
  static constexpr auto dist_category = DistCategory::ASYMMETRIC;
  const class Right
  {
  public:
    const double mode, support;
    static constexpr bool is_mode_unbounded = false;
    static constexpr auto dist_category = DistCategory::STRICTLY_DECREASING;
    static constexpr auto tail_category = TailCategory::FINITE;
    constexpr Right(double mode, double end) : mode{mode}, support{end} {}
    constexpr double pdf(double x) const { return (support - x) / (support - mode); }
    constexpr double strip_area(double x) const { return (x + support - 2 * mode) * pdf(x) / 2; }
  } right;
  const class Left
  {
  public:
    const double mode, support;
    static constexpr bool is_mode_unbounded = false;
    static constexpr auto dist_category = DistCategory::STRICTLY_INCREASING;
    static constexpr auto tail_category = TailCategory::FINITE;
    constexpr Left(double mode, double start) : mode{mode}, support{start} {}
    constexpr double pdf(double x) const { return (x - support) / (mode - support); }
    constexpr double strip_area(double x) const { return (2 * mode - x - support) * pdf(x) / 2; }
  } left;
  constexpr Triangular(double start = -1.0, double mode = 0.0, double end = 1.0) : mode{mode}, start{start}, end{end}, right{mode, end}, left{mode, start} {}
};

template <uint_fast8_t dof>
class ChiSquared
{
  static constexpr bool dof_is_even = dof % 2 == 0;

public:
  static constexpr double mode = dof - 2;
  static constexpr bool is_mode_unbounded = false;
  static constexpr auto dist_category = DistCategory::ASYMMETRIC;
  static double pdf(double x)
  {
    return dof_is_even ? std::pow(x, dof / 2 - 1) * std::exp(-x / 2) : std::sqrt(std::pow(x, dof - 2)) * std::exp(-x / 2);
  }
  static double cdf(double x) { return std::pow(2, dof / 2.) * boost::math::tgamma_lower(dof / 2., x / 2.); }
  static double ccdf(double x) { return std::pow(2, dof / 2.) * boost::math::tgamma(dof / 2., x / 2.); }
  class Right
  {
  public:
    static constexpr double mode = ChiSquared::mode;
    static constexpr bool is_mode_unbounded = false;
    static constexpr auto dist_category = DistCategory::STRICTLY_DECREASING;
    static constexpr auto tail_category = TailCategory::MAP_REJECT;
    static double pdf(double x) { return ChiSquared::pdf(x); }
    static double strip_area(double x) { return (x - mode) * pdf(x) + ccdf(x); }
    static double tail_value_rel_mode(double tail_start_rel_mode, double u)
    {
      return tail_start_rel_mode - 2 * std::log(u) * (1 + mode / tail_start_rel_mode);
    }
    static double tail_accept_probability(double tail_start_rel_mode, double x_rel_mode)
    {
      return std::exp((1 - (mode + x_rel_mode) / (mode + tail_start_rel_mode)) * mode / 2) * std::pow((mode + x_rel_mode) / (mode + tail_start_rel_mode), mode / 2);
    }
  };
  class Left
  {
  public:
    static constexpr double mode = ChiSquared::mode;
    static constexpr bool is_mode_unbounded = false;
    static constexpr double support = 0;
    static constexpr auto dist_category = DistCategory::STRICTLY_INCREASING;
    static constexpr auto tail_category = TailCategory::FINITE;
    static double pdf(double x) { return ChiSquared::pdf(x); }
    static double strip_area(double x) { return (mode - x) * pdf(x) + cdf(x); }
  };
};

template <>
class ChiSquared<2>
{
public:
  static constexpr double mode = 0;
  static constexpr bool is_mode_unbounded = false;
  static constexpr auto dist_category = DistCategory::STRICTLY_DECREASING;
  static constexpr auto tail_category = TailCategory::MAP;
  static double pdf(double x) { return std::exp(-x / 2); }
  static double ccdf(double x) { return 2 * std::exp(-x / 2); }
  static double strip_area(double x) { return x * pdf(x) + ccdf(x); }
  static double tail_value_rel_mode(double tail_start, double u) { return tail_start - 2 * std::log(u); }
};

template <>
class ChiSquared<1>
{
public:
  static constexpr double mode = 0;
  static constexpr bool is_mode_unbounded = true;
  static constexpr auto dist_category = DistCategory::STRICTLY_DECREASING;
  static constexpr auto tail_category = TailCategory::MAP_REJECT;
  static double pdf(double x)
  {
    return (x ? (std::exp(-x / 2) / std::sqrt(x)) : std::numeric_limits<double>::infinity());
  }
  static double ccdf(double x) { return std::sqrt(2 * math::pi) * std::erfc(std::sqrt(x / 2)); }
  static double strip_area(double x) { return x ? x * pdf(x) + ccdf(x) : std::sqrt(2 * math::pi); }
  static constexpr double peak_value_rel_mode(double peak_start_rel_mode, double u)
  {
    return u * u * peak_start_rel_mode;
  }
  static double peak_accept_probability(double peak_start, double x)
  {
    return std::exp(-x / 2) - std::exp(-peak_start / 2) * std::sqrt(x / peak_start);
  }
  static double tail_value_rel_mode(double tail_start_rel_mode, double u)
  {
    return tail_start_rel_mode - 2 * std::log(u);
  }
  static double tail_accept_probability(double tail_start_rel_mode, double x_rel_mode)
  {
    return std::sqrt((mode + tail_start_rel_mode) / (mode + x_rel_mode));
  }
};

class Gamma
{
public:
  const double mode, shape, scale;
  const bool is_mode_unbounded;
  static constexpr auto dist_category = DistCategory::ASYMMETRIC;
  const class Right
  {
  public:
    const double mode, shape, scale;
    const bool is_mode_unbounded;
    const double mode_beta, mode_mapping_exponent, peak_accept_probability_const;
    static constexpr auto dist_category = DistCategory::STRICTLY_DECREASING;
    static constexpr auto tail_category = TailCategory::MAP_REJECT;
    Right(double shape, double scale)
        : shape{shape}, scale{scale}, is_mode_unbounded{shape < 1}, mode{shape > 1 ? (shape - 1) * scale : 0},
          mode_beta{(shape * (2 - shape)) / 2.},
          mode_mapping_exponent{1. / mode_beta},
          peak_accept_probability_const{std::pow(1 + (1 - shape) * (1 - shape), 0.5 * ((1 - shape) + 1. / (1 - shape))) / (2 * (1 - shape) * std::pow(shape, shape * shape / (1 - shape)))} {}
    double pdf(double x) const { return std::pow(x, shape - 1) * std::exp(-x / scale); }
    double ccdf(double x) const { return std::pow(scale, shape) * boost::math::tgamma(shape, x / scale); }
    double strip_area(double x) const { return shape < 1 && x == 0 ? std::pow(scale, shape) * std::tgamma(shape) : (x - mode) * pdf(x) + ccdf(x); }
    double peak_value_rel_mode(double peak_start_rel_mode, double u) const
    {
      if (shape >= 1)
        throw std::logic_error("unexpected call");
      return std::pow(u, mode_mapping_exponent) * peak_start_rel_mode;
    }
    double peak_accept_probability(double peak_start_rel_mode, double x) const
    {
      if (shape >= 1)
        throw std::logic_error("unexpected call");
      auto h_b = std::exp(-peak_start_rel_mode / scale);
      auto peak_accept_probability_corrected_const = peak_accept_probability_const / (peak_accept_probability_const + h_b * (1 - peak_accept_probability_const));
      return peak_accept_probability_corrected_const * std::pow(x / peak_start_rel_mode, shape * shape / 2) * (std::exp(-x / scale) - h_b * std::pow(x / peak_start_rel_mode, 1 - shape));
    }
    double tail_value_rel_mode(double tail_start_rel_mode, double u) const
    {
      return shape > 1 ? tail_start_rel_mode - scale * (1 + mode / tail_start_rel_mode) * std::log(u) : tail_start_rel_mode - scale * std::log(u);
    }
    double tail_accept_probability(double tail_start_rel_mode, double x_rel_mode) const
    {
      return shape > 1 ? std::pow((mode + x_rel_mode) / (mode + tail_start_rel_mode), shape - 1) * std::exp(-(x_rel_mode - tail_start_rel_mode) * (shape - 1) / (mode + tail_start_rel_mode)) : std::pow((mode + x_rel_mode) / (mode + tail_start_rel_mode), shape - 1);
    }
  } right;
  const class Left
  {
  public:
    const double mode, shape, scale;
    static constexpr double support = 0;
    static constexpr bool is_mode_unbounded = false;
    static constexpr auto dist_category = DistCategory::STRICTLY_INCREASING;
    static constexpr auto tail_category = TailCategory::FINITE;
    constexpr Left(double shape, double scale) : shape{shape}, scale{scale}, mode{shape > 1 ? (shape - 1) * scale : 0} {}
    double pdf(double x) const { return shape < 1 ? 0 : std::pow(x, shape - 1) * std::exp(-x / scale); }
    double cdf(double x) const { return std::pow(scale, shape) * boost::math::tgamma_lower(shape, x / scale); }
    double strip_area(double x) const { return shape > 1 ? (mode - x) * pdf(x) + cdf(x) : 0; }
  } left;
  Gamma(double shape = 1.0, double scale = 1.0)
      : shape{shape}, scale{scale}, is_mode_unbounded{shape < 1}, mode{shape > 1 ? (shape - 1) * scale : 0}, right{shape, scale}, left{shape, scale} {}
};

class Weibull
{
public:
  const double mode, shape, scale;
  static constexpr auto dist_category = DistCategory::ASYMMETRIC;
  const class Right
  {
  public:
    const double mode, shape, scale;
    const bool is_mode_unbounded;
    const double mode_mapping_exponent, peak_accept_probability_const;
    static constexpr auto dist_category = DistCategory::STRICTLY_DECREASING;
    static constexpr auto tail_category = TailCategory::MAP;
    constexpr Right(double shape, double scale)
        : shape{shape}, scale{scale}, is_mode_unbounded{shape < 1},
          mode{shape > 1 ? scale * std::pow((shape - 1) / shape, 1 / shape) : 0},
          mode_mapping_exponent{2. / (shape * (2 - shape))},
          peak_accept_probability_const{std::pow(1 + (1 - shape) * (1 - shape), 0.5 * ((1 - shape) + 1. / (1 - shape))) / (2 * (1 - shape) * std::pow(shape, shape * shape / (1 - shape)))} {}
    double pdf(double x) const
    {
      return (x == 0 && shape < 1) ? std::numeric_limits<double>::infinity() : shape / scale * std::pow(x / scale, shape - 1) * std::exp(-std::pow(x / scale, shape));
    }
    double ccdf(double x) const { return std::exp(-std::pow(x / scale, shape)); }
    double strip_area(double x) const { return shape < 1 && x == 0 ? 1 : (x - mode) * pdf(x) + ccdf(x); }
    double peak_value_rel_mode(double peak_start_rel_mode, double u) const
    {
      if (shape >= 1)
        throw std::logic_error("unexpected call");
      return std::pow(u, mode_mapping_exponent) * peak_start_rel_mode;
    }
    double peak_accept_probability(double peak_start, double x) const
    {
      if (shape >= 1)
        throw std::logic_error("unexpected call");
      auto h_b = std::exp(-std::pow(peak_start / scale, shape));
      auto peak_accept_probability_corrected_const = peak_accept_probability_const / (peak_accept_probability_const + h_b * (1 - peak_accept_probability_const));
      return peak_accept_probability_corrected_const * std::pow(x / peak_start, shape * shape / 2) * (std::exp(-std::pow(x / scale, shape)) - h_b * std::pow(x / peak_start, 1 - shape));
    }
    double tail_value_rel_mode(double tail_start_rel_mode, double u) const
    {
      return scale * std::pow(std::pow((mode + tail_start_rel_mode) / scale, shape) - std::log(u), 1 / shape) - mode;
    }
  } right;
  const class Left
  {
  public:
    const double mode, shape, scale;
    static constexpr double support = 0;
    static constexpr bool is_mode_unbounded = false;
    static constexpr auto dist_category = DistCategory::STRICTLY_INCREASING;
    static constexpr auto tail_category = TailCategory::FINITE;
    constexpr Left(double shape, double scale)
        : shape{shape}, scale{scale}, mode{shape > 1 ? scale * std::pow((shape - 1) / shape, 1 / shape) : 0} {}
    double pdf(double x) const
    {
      return shape < 1 ? 0 : shape / scale * std::pow(x / scale, shape - 1) * std::exp(-std::pow(x / scale, shape));
    }
    double cdf(double x) const { return 1 - std::exp(-std::pow(x / scale, shape)); }
    double strip_area(double x) const { return shape > 1 ? (mode - x) * pdf(x) + cdf(x) : 0; }
  } left;
  constexpr Weibull(double shape = 1.0, double scale = 1.0)
      : shape{shape}, scale{scale}, mode{shape > 1 ? scale * std::pow((shape - 1) / shape, 1 / shape) : 0}, right{shape, scale}, left{shape, scale} {}
};

class LogNormal
{
public:
  const double mode, normal_mean, normal_std_dev;
  static constexpr bool is_mode_unbounded = false;
  static constexpr auto dist_category = DistCategory::ASYMMETRIC;
  const class Right
  {
  public:
    const double mode, normal_mean, normal_std_dev;
    static constexpr bool is_mode_unbounded = false;
    static constexpr auto dist_category = DistCategory::STRICTLY_DECREASING;
    static constexpr auto tail_category = TailCategory::MAP_REJECT;
    Right(double normal_mean, double normal_std_dev)
        : normal_mean{normal_mean}, normal_std_dev{normal_std_dev}, mode{std::exp(normal_mean - normal_std_dev * normal_std_dev)} {}
    double pdf(double x) const
    {
      return x ? std::exp(-(std::log(x) - normal_mean) * (std::log(x) - normal_mean) / 2 / normal_std_dev / normal_std_dev) / x : 0;
    }
    double ccdf(double x) const
    {
      return x ? math::half_pi_sqrt * normal_std_dev * std::erfc((std::log(x) - normal_mean) * std::sqrt(0.5) / normal_std_dev) : math::half_pi_sqrt * normal_std_dev;
    }
    double strip_area(double x) const { return (x - mode) * pdf(x) + ccdf(x); }
    double tail_value_rel_mode(double tail_start_rel_mode, double u) const
    {
      auto tail_start = mode + tail_start_rel_mode;
      auto alpha = normal_std_dev * normal_std_dev / (std::log(tail_start) - normal_mean);
      return tail_start / std::pow(u, alpha) - mode;
    }
    double tail_accept_probability(double tail_start_rel_mode, double x_rel_mode) const
    {
      auto tail_start = mode + tail_start_rel_mode;
      auto alpha = normal_std_dev * normal_std_dev / (std::log(tail_start) - normal_mean);
      return std::pow((mode + x_rel_mode) / tail_start, 1 + 1 / alpha) * pdf(mode + x_rel_mode) / pdf(tail_start);
    }
  } right;
  const class Left
  {
  public:
    const double mode, normal_mean, normal_std_dev;
    static constexpr double support = 0;
    static constexpr bool is_mode_unbounded = false;
    static constexpr auto dist_category = DistCategory::STRICTLY_INCREASING;
    static constexpr auto tail_category = TailCategory::FINITE;
    Left(double normal_mean, double normal_std_dev)
        : normal_mean{normal_mean}, normal_std_dev{normal_std_dev}, mode{std::exp(normal_mean - normal_std_dev * normal_std_dev)} {}
    double pdf(double x) const
    {
      return x ? std::exp(-(std::log(x) - normal_mean) * (std::log(x) - normal_mean) / 2 / normal_std_dev / normal_std_dev) / x : 0;
    }
    double cdf(double x) const
    {
      return x ? math::half_pi_sqrt * normal_std_dev * (1 + std::erf((std::log(x) - normal_mean) * std::sqrt(0.5) / normal_std_dev)) : 0;
    }
    double strip_area(double x) const { return (mode - x) * pdf(x) + cdf(x); }
  } left;
  LogNormal(double normal_mean = 0.0, double normal_std_dev = 1.0)
      : normal_mean{normal_mean}, normal_std_dev{normal_std_dev}, mode{std::exp(normal_mean - normal_std_dev * normal_std_dev)}, right{normal_mean, normal_std_dev}, left{normal_mean, normal_std_dev} {}
};

class FisherF
{
public:
  const double mode, d1, d2;
  const bool is_mode_unbounded;
  static constexpr auto dist_category = DistCategory::ASYMMETRIC;
  const class Right
  {
  public:
    const double mode, d1, d2;
    const double mode_mapping_exponent, peak_accept_probability_const;
    const bool is_mode_unbounded;
    static constexpr auto dist_category = DistCategory::STRICTLY_DECREASING;
    static constexpr auto tail_category = TailCategory::MAP_REJECT;
    Right(double d1, double d2)
        : d1{d1}, d2{d2}, is_mode_unbounded{d1 < 2}, mode{d1 > 2 ? d2 * (d1 - 2) / d1 / (d2 + 2) : 0},
          mode_mapping_exponent{2 / d1 / (1 - d1 / 4)},
          peak_accept_probability_const{std::pow(d1 / 2, -d1 * d1 / (4 - 2 * d1)) * std::pow(2 - d1 + d1 * d1 / 4, (2 - d1 + d1 * d1 / 4) / (2 - d1)) / (2 - d1)} {}
    double pdf(double x) const { return math::fisher_f_pdf(d1, d2, x); }
    double ccdf(double x) const { return math::fisher_f_ccdf(d1, d2, x); }
    double strip_area(double x) const
    {
      return d1 < 2 && x == 0 ? boost::math::beta(d1 / 2, d2 / 2) : (x - mode) * pdf(x) + ccdf(x);
    }
    double peak_value_rel_mode(double peak_start_rel_mode, double u) const
    {
      if (d1 >= 2)
        throw std::logic_error("unexpected call");
      return std::pow(u, mode_mapping_exponent) * peak_start_rel_mode;
    }
    double peak_accept_probability(double peak_start_rel_mode, double x) const
    {
      if (d1 >= 2)
        throw std::logic_error("unexpected call");
      auto h_b = std::pow(1 + d1 / d2 * peak_start_rel_mode, -(d1 + d2) / 2);
      auto peak_accept_probability_corrected_const = peak_accept_probability_const / (peak_accept_probability_const + h_b * (1 - peak_accept_probability_const));
      return peak_accept_probability_corrected_const * std::pow(x / peak_start_rel_mode, d1 * d1 / 8) * (std::pow(1 + x * d1 / d2, -(d1 + d2) / 2) - h_b * std::pow(x / peak_start_rel_mode, 1 - d1 / 2));
    }
    double tail_value_rel_mode(double tail_start_rel_mode, double u) const
    {
      auto tail_start = mode + tail_start_rel_mode;
      auto sigma_minus_tail_start = d1 == 2 ? d2 / 2 : d1 < 2 ? d2 * (d1 + d2) / d1 / (d2 + 2) : tail_start * d2 * (d1 + d2) / (tail_start * d1 * (d2 + 2) - d2 * (d1 - 2));
      return (tail_start + sigma_minus_tail_start) * std::pow(u, -2 / d2) - sigma_minus_tail_start - mode;
    }
    double tail_accept_probability(double tail_start_rel_mode, double x_rel_mode) const
    {
      auto tail_start = mode + tail_start_rel_mode, x = mode + x_rel_mode;
      auto sigma = d1 == 2 ? d2 / 2 + tail_start : d1 < 2 ? d2 * (d1 + d2) / d1 / (d2 + 2) + tail_start : tail_start * d2 * (d1 + d2) / (tail_start * d1 * (d2 + 2) - d2 * (d1 - 2)) + tail_start;
      return std::pow(x / tail_start, d1 / 2 - 1) * std::pow((1 + tail_start * d1 / d2) / (1 + x * d1 / d2), (d1 + d2) / 2) * std::pow(1 + (x_rel_mode - tail_start_rel_mode) / sigma, d2 / 2 + 1);
    }
  } right;
  const class Left
  {
  public:
    const double mode, d1, d2;
    static constexpr double support = 0;
    static constexpr bool is_mode_unbounded = false;
    static constexpr auto dist_category = DistCategory::STRICTLY_INCREASING;
    static constexpr auto tail_category = TailCategory::FINITE;
    Left(double d1, double d2) : d1{d1}, d2{d2}, mode{d1 > 2 ? d2 * (d1 - 2) / d1 / (d2 + 2) : 0} {}
    double pdf(double x) const { return math::fisher_f_pdf(d1, d2, x); }
    double cdf(double x) const { return math::fisher_f_cdf(d1, d2, x); }
    double strip_area(double x) const
    {
      return d1 > 2 ? (mode - x) * pdf(x) + cdf(x) : 0;
    }
  } left;
  FisherF(double d1 = 1.0, double d2 = 1.0)
      : d1{d1}, d2{d2}, is_mode_unbounded{d1 < 2}, mode{d1 > 2 ? d2 * (d1 - 2) / d1 / (d2 + 2) : 0}, right{d1, d2}, left{d1, d2} {}
};

namespace detail
{

template <class Dist, typename float_type>
float_type area_residue(float_type y, float_type target_area)
{
  return Dist::strip_area(y) - target_area;
}

template <class Dist, typename float_type>
float_type binary_search_for_strip_coord(float_type old_y, float_type new_y, float_type target_area)
{
  return area_residue<Dist, float_type>(old_y, target_area) == 0 ? old_y : area_residue<Dist, float_type>(new_y, target_area) == 0 ? new_y : (old_y == new_y) ? new_y : ((old_y + new_y) / 2 == old_y || (old_y + new_y) / 2 == new_y) ? (std::abs(area_residue<Dist, float_type>(old_y, target_area)) < std::abs(area_residue<Dist, float_type>(new_y, target_area)) ? old_y : new_y) : area_residue<Dist, float_type>(new_y, target_area) * area_residue<Dist, float_type>((old_y + new_y) / 2, target_area) < 0 ? binary_search_for_strip_coord<Dist, float_type>(new_y, (old_y + new_y) / 2, target_area) : binary_search_for_strip_coord<Dist, float_type>(old_y, (old_y + new_y) / 2, target_area);
}

template <class Dist, typename float_type>
float_type multiplicative_binary_search(float_type old_y, float_type new_y, float_type target_area)
{
  return area_residue<Dist, float_type>(old_y, target_area) == 0 ? old_y : area_residue<Dist, float_type>(new_y, target_area) == 0 ? new_y : area_residue<Dist, float_type>(old_y, target_area) * area_residue<Dist, float_type>(new_y, target_area) < 0 ? binary_search_for_strip_coord<Dist, float_type>(old_y, new_y, target_area) : multiplicative_binary_search<Dist, float_type>(new_y, new_y + 2 * (new_y - old_y), target_area);
}

template <class Dist, typename float_type>
float_type area_residue(float_type y, float_type target_area, const Dist &dist)
{
  return dist.strip_area(y) - target_area;
}

template <class Dist, typename float_type>
float_type binary_search_for_strip_coord(float_type old_y, float_type new_y, float_type target_area, const Dist &dist)
{
  return area_residue<Dist, float_type>(old_y, target_area, dist) == 0 ? old_y : area_residue<Dist, float_type>(new_y, target_area, dist) == 0 ? new_y : (old_y == new_y) ? new_y : ((old_y + new_y) / 2 == old_y || (old_y + new_y) / 2 == new_y) ? (std::abs(area_residue<Dist, float_type>(old_y, target_area, dist)) < std::abs(area_residue<Dist, float_type>(new_y, target_area, dist)) ? old_y : new_y) : area_residue<Dist, float_type>(new_y, target_area, dist) * area_residue<Dist, float_type>((old_y + new_y) / 2, target_area, dist) < 0 ? binary_search_for_strip_coord<Dist, float_type>(new_y, (old_y + new_y) / 2, target_area, dist) : binary_search_for_strip_coord<Dist, float_type>(old_y, (old_y + new_y) / 2, target_area, dist);
}

template <class Dist, typename float_type>
float_type multiplicative_binary_search(float_type old_y, float_type new_y, float_type target_area, const Dist &dist)
{
  return area_residue<Dist, float_type>(old_y, target_area, dist) == 0 ? old_y : area_residue<Dist, float_type>(new_y, target_area, dist) == 0 ? new_y : area_residue<Dist, float_type>(old_y, target_area, dist) * area_residue<Dist, float_type>(new_y, target_area, dist) < 0 ? binary_search_for_strip_coord<Dist, float_type>(old_y, new_y, target_area, dist) : multiplicative_binary_search<Dist, float_type>(new_y, new_y + 2 * (new_y - old_y), target_area, dist);
}

namespace type_utils
{

struct substitution_failure
{
};

template <uintmax_t M>
struct unsigned_integer_with_max
{
  using type = substitution_failure;
  static constexpr bool exist = false;
};

template <>
struct unsigned_integer_with_max<std::numeric_limits<uint32_t>::max()>
{
  using type = uint32_t;
  static constexpr bool exist = true;
};

template <>
struct unsigned_integer_with_max<std::numeric_limits<uint64_t>::max()>
{
  using type = uint64_t;
  static constexpr bool exist = true;
};

template <uintmax_t M>
using unsigned_integer_with_max_t = typename unsigned_integer_with_max<M>::type;

template <class C>
class has_dist_category
{
  template <class X>
  static auto check(const X &) -> decltype(&X::dist_category);
  static substitution_failure check(...);

public:
  static constexpr bool value = std::is_same<decltype(check(std::declval<C>())), const DistCategory *>::value;
};

template <class C>
class has_tail_category
{
  template <class X>
  static auto check(const X &) -> decltype(&X::tail_category);
  static substitution_failure check(...);

public:
  static constexpr bool value = std::is_same<decltype(check(std::declval<C>())), const TailCategory *>::value;
};

template <class C>
class has_static_is_mode_unbounded
{
  template <class X>
  static auto check(const X &) -> decltype(&X::is_mode_unbounded);
  static substitution_failure check(...);

public:
  static constexpr bool value = std::is_same<decltype(check(std::declval<C>())), const bool *>::value;
};

template <class C, bool has_static_sigularity_flag>
struct need_peak_member_functions_base
{
  static constexpr bool value = true;
};

template <class C>
struct need_peak_member_functions_base<C, true>
{
  static constexpr bool value = C::is_mode_unbounded;
};

template <class C>
struct need_peak_member_functions : public need_peak_member_functions_base<C, has_static_is_mode_unbounded<C>::value>
{
};

class is_valid_pdf_base_check
{
protected:
  template <class X>
  static auto check_mode(const X &x) -> decltype(x.mode);
  static substitution_failure check_mode(...);
  template <class X>
  static auto check_is_mode_unbounded(const X &x) -> decltype(x.is_mode_unbounded);
  static substitution_failure check_is_mode_unbounded(...);
  template <class X>
  static auto check_pdf(const X &x) -> decltype(x.pdf(double{}));
  static substitution_failure check_pdf(...);
  template <class X>
  static auto check_peak_value_rel_mode(const X &x) -> decltype(x.peak_value_rel_mode(double{}, double{}));
  static substitution_failure check_peak_value_rel_mode(...);
  template <class X>
  static auto check_peak_accept_probability(const X &x) -> decltype(x.peak_accept_probability(double{}, double{}));
  static substitution_failure check_peak_accept_probability(...);
  template <class X>
  static auto check_tail_value_rel_mode(const X &x) -> decltype(x.tail_value_rel_mode(double{}, double{}));
  static substitution_failure check_tail_value_rel_mode(...);
  template <class X>
  static auto check_tail_accept_probability(const X &x) -> decltype(x.tail_accept_probability(double{}, double{}));
  static substitution_failure check_tail_accept_probability(...);
  template <class X>
  static auto check_right(const X &) -> typename X::Right;
  static substitution_failure check_right(...);
  template <class X>
  static auto check_left(const X &) -> typename X::Left;
  static substitution_failure check_left(...);
};

template <class C>
struct has_or_dont_need_peak_member_functions : private is_valid_pdf_base_check
{
  static constexpr bool value = (!need_peak_member_functions<C>::value) || (std::is_floating_point<decltype(check_peak_value_rel_mode(std::declval<C>()))>::value && std::is_floating_point<decltype(check_peak_accept_probability(std::declval<C>()))>::value);
};

template <class C, TailCategory tail_category>
struct is_valid_simple_pdf_base : private is_valid_pdf_base_check
{
  static constexpr bool value = false;
};

template <class C>
struct is_valid_simple_pdf_base<C, TailCategory::FINITE> : private is_valid_pdf_base_check
{
  static constexpr bool value = std::is_floating_point<decltype(check_mode(std::declval<C>()))>::value && std::is_same<decltype(check_is_mode_unbounded(std::declval<C>())), bool>::value && std::is_floating_point<decltype(check_pdf(std::declval<C>()))>::value && has_or_dont_need_peak_member_functions<C>::value;
};

template <class C>
struct is_valid_simple_pdf_base<C, TailCategory::MAP> : private is_valid_pdf_base_check
{
  static constexpr bool value = std::is_floating_point<decltype(check_mode(std::declval<C>()))>::value && std::is_same<decltype(check_is_mode_unbounded(std::declval<C>())), bool>::value && std::is_floating_point<decltype(check_pdf(std::declval<C>()))>::value && std::is_floating_point<decltype(check_tail_value_rel_mode(std::declval<C>()))>::value && has_or_dont_need_peak_member_functions<C>::value;
};

template <class C>
struct is_valid_simple_pdf_base<C, TailCategory::MAP_REJECT> : private is_valid_pdf_base_check
{
  static constexpr bool value = std::is_floating_point<decltype(check_mode(std::declval<C>()))>::value && std::is_same<decltype(check_is_mode_unbounded(std::declval<C>())), bool>::value && std::is_floating_point<decltype(check_pdf(std::declval<C>()))>::value && std::is_floating_point<decltype(check_tail_value_rel_mode(std::declval<C>()))>::value && std::is_floating_point<decltype(check_tail_accept_probability(std::declval<C>()))>::value && has_or_dont_need_peak_member_functions<C>::value;
};

template <class C>
struct is_valid_asymmetric_pdf_base : private is_valid_pdf_base_check
{
  static_assert(!std::is_same<substitution_failure, decltype(check_right(std::declval<C>()))>::value && !std::is_same<substitution_failure, decltype(check_left(std::declval<C>()))>::value, "Asymmetric distributions must have Right and Left subclasses");
  static constexpr bool value = is_valid_simple_pdf_base<typename C::Right, C::Right::tail_category>::value && is_valid_simple_pdf_base<typename C::Left, C::Left::tail_category>::value;
};

template <class C, DistCategory dist_category>
struct is_valid_pdf_base
{
  static constexpr bool value = is_valid_simple_pdf_base<C, C::tail_category>::value;
};

template <class C>
struct is_valid_pdf_base<C, DistCategory::ASYMMETRIC>
{
  static constexpr bool value = is_valid_asymmetric_pdf_base<C>::value;
};

class is_static_pdf_base_check
{
protected:
  template <class X>
  static auto check_mode(const X &) -> decltype(&X::mode);
  static substitution_failure check_mode(...);
  template <class X>
  static auto check_is_mode_unbounded(const X &) -> decltype(&X::is_mode_unbounded);
  static substitution_failure check_is_mode_unbounded(...);
  template <class X>
  static auto check_pdf(const X &) -> decltype(X::pdf(double{}));
  static substitution_failure check_pdf(...);
  template <class X>
  static auto check_peak_value_rel_mode(const X &) -> decltype(X::peak_value_rel_mode(double{}, double{}));
  static substitution_failure check_peak_value_rel_mode(...);
  template <class X>
  static auto check_peak_accept_probability(const X &) -> decltype(X::peak_accept_probability(double{}, double{}));
  static substitution_failure check_peak_accept_probability(...);
  template <class X>
  static auto check_tail_value_rel_mode(const X &) -> decltype(X::tail_value_rel_mode(double{}, double{}));
  static substitution_failure check_tail_value_rel_mode(...);
  template <class X>
  static auto check_tail_accept_probability(const X &) -> decltype(X::tail_accept_probability(double{}, double{}));
  static substitution_failure check_tail_accept_probability(...);
};

template <class C>
struct has_static_or_dont_need_peak_member_functions : private is_static_pdf_base_check
{
  static constexpr bool value = (!need_peak_member_functions<C>::value) || (std::is_floating_point<decltype(check_peak_value_rel_mode(std::declval<C>()))>::value && std::is_floating_point<decltype(check_peak_accept_probability(std::declval<C>()))>::value);
};

template <class C, TailCategory tail_category>
struct is_static_simple_pdf_base : private is_static_pdf_base_check
{
  static constexpr bool value = false;
};

template <class C>
struct is_static_simple_pdf_base<C, TailCategory::FINITE> : private is_static_pdf_base_check
{
  static constexpr bool value = std::is_floating_point<std::remove_pointer_t<decltype(check_mode(std::declval<C>()))>>::value && std::is_same<decltype(check_is_mode_unbounded(std::declval<C>())), const bool *>::value && std::is_floating_point<decltype(check_pdf(std::declval<C>()))>::value && has_static_or_dont_need_peak_member_functions<C>::value;
};

template <class C>
struct is_static_simple_pdf_base<C, TailCategory::MAP> : private is_static_pdf_base_check
{
  static constexpr bool value = std::is_floating_point<std::remove_pointer_t<decltype(check_mode(std::declval<C>()))>>::value && std::is_same<decltype(check_is_mode_unbounded(std::declval<C>())), const bool *>::value && std::is_floating_point<decltype(check_pdf(std::declval<C>()))>::value && std::is_floating_point<decltype(check_tail_value_rel_mode(std::declval<C>()))>::value && has_static_or_dont_need_peak_member_functions<C>::value;
};

template <class C>
struct is_static_simple_pdf_base<C, TailCategory::MAP_REJECT> : private is_static_pdf_base_check
{
  static constexpr bool value = std::is_floating_point<std::remove_pointer_t<decltype(check_mode(std::declval<C>()))>>::value && std::is_same<decltype(check_is_mode_unbounded(std::declval<C>())), const bool *>::value && std::is_floating_point<decltype(check_pdf(std::declval<C>()))>::value && std::is_floating_point<decltype(check_tail_value_rel_mode(std::declval<C>()))>::value && std::is_floating_point<decltype(check_tail_accept_probability(std::declval<C>()))>::value && has_static_or_dont_need_peak_member_functions<C>::value;
};

template <class C, DistCategory dist_category>
struct is_static_pdf_base
{
  static constexpr bool value = is_static_simple_pdf_base<C, C::tail_category>::value;
};

template <class C>
struct is_static_pdf_base<C, DistCategory::ASYMMETRIC>
{
  static constexpr bool value = is_static_simple_pdf_base<typename C::Right, C::Right::tail_category>::value && is_static_simple_pdf_base<typename C::Left, C::Left::tail_category>::value;
};

} // namespace type_utils

template <class C>
class is_valid_pdf : public type_utils::is_valid_pdf_base<C, C::dist_category>
{
};

template <class C>
class is_static_pdf : public type_utils::is_static_pdf_base<C, C::dist_category>
{
};

constexpr auto region_efficiency_error_threshold = 1e-10;
constexpr auto region_efficiency_warning_threshold = 1e-2;
constexpr auto overall_efficiency_error_threshold = 1e-5;
constexpr auto overall_efficiency_warning_threshold = 1e-1;

static void verify_region_rejection_efficiency(double region_efficiency, uint_fast16_t region_idx)
{
  if (region_efficiency < region_efficiency_error_threshold)
  {
    std::cerr << "ERROR: region #" << region_idx << " has extremely low rejection efficiency: " << region_efficiency << std::endl;
    throw std::logic_error("extremely low rejection efficiency");
  }
  else if (region_efficiency < region_efficiency_warning_threshold)
    std::cerr << "WARNING: region #" << region_idx << " has low rejection efficiency: " << region_efficiency << std::endl;
}

static void verify_overall_rejection_efficiency(double overall_efficiency)
{
  if (overall_efficiency < overall_efficiency_error_threshold)
  {
    std::cerr << "ERROR: extremely low overall rejection efficiency: " << overall_efficiency << std::endl;
    throw std::logic_error("extremely low overall rejection efficiency");
  }
  else if (overall_efficiency < overall_efficiency_warning_threshold)
    std::cerr << "WARNING: overall rejection efficiency is low: " << overall_efficiency << std::endl;
}

template <typename float_type>
void verify_efficiency(const float_type *const x_corner_rel_mode, const float_type *const y_corner, uint_fast16_t first_idx, uint_fast16_t num_finite_regions, double region_area)
{
  auto overall_rejection_efficiency = 0.;
  for (int i = first_idx; i < first_idx + num_finite_regions; ++i)
  {
    auto efficiency = region_area / std::abs(x_corner_rel_mode[i]) / (y_corner[i + 1] - y_corner[i]);
    verify_region_rejection_efficiency(efficiency, i);
    overall_rejection_efficiency += efficiency;
  }
  overall_rejection_efficiency /= num_finite_regions;
  verify_overall_rejection_efficiency(overall_rejection_efficiency);
}

namespace symmetric /*distributions*/
{

namespace nonsingular /*mode*/
{

namespace finite /*tail*/
{

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class static_impl
{
  static_assert(is_static_pdf<Dist>::value && Dist::dist_category == DistCategory::SYMMETRIC && Dist::tail_category == TailCategory::FINITE);

public:
  using result_type = float_type;

  static_impl();

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  using signed_integer_type = std::make_signed_t<unsigned_integer_type>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr float_type gen_signed_range_inv = math::exp2_int<float_type>(-std::numeric_limits<signed_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_signed_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  signed_integer_type x_corner_ratio[N];
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
static_impl<Dist, URBG, N, float_type>::static_impl()
    : y_max{Dist::pdf(Dist::mode)}, total_area{Dist::strip_area(Dist::mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_signed_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}
{
  if (Dist::mode == Dist::support)
    return;
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");
  if (Dist::pdf(Dist::support) != 0)
    throw std::logic_error("PDF should be zero at the support boundary");
  if (Dist::strip_area(Dist::support) != 0)
    throw std::logic_error("strip_area must return zero for a strip of zero height");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = binary_search_for_strip_coord<Dist, float_type>(Dist::support, Dist::mode, total_area * i / N) - Dist::mode;
    y_corner[i] = Dist::pdf(Dist::mode + x_corner_rel_mode[i]);
    strip_width_w_signed_gen_range[i] = x_corner_rel_mode[i] * gen_signed_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }

  verify_efficiency(x_corner_rel_mode, y_corner, 0, N, total_area / N);

  x_corner_rel_mode[0] = Dist::support - Dist::mode;
  strip_width_w_signed_gen_range[0] = x_corner_rel_mode[0] * gen_signed_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<signed_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type static_impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  signed_integer_type x = static_cast<signed_integer_type>(r & not_index_mask);
  while (true)
  {
    if (std::abs(x) < x_corner_ratio[index] || (gen() * strip_height_w_unsigned_gen_range[index + 1] < (Dist::pdf(Dist::mode + x * strip_width_w_signed_gen_range[index]) - y_corner[index])))
      return Dist::mode + x * strip_width_w_signed_gen_range[index];
    x = static_cast<signed_integer_type>(gen() & not_index_mask);
  }
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class impl
{
  static_assert(!is_static_pdf<Dist>::value && Dist::dist_category == DistCategory::SYMMETRIC && Dist::tail_category == TailCategory::FINITE);

public:
  using result_type = float_type;

  impl(const Dist &);

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  using signed_integer_type = std::make_signed_t<unsigned_integer_type>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr float_type gen_signed_range_inv = math::exp2_int<float_type>(-std::numeric_limits<signed_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  const Dist &dist;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_signed_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  signed_integer_type x_corner_ratio[N];
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
impl<Dist, URBG, N, float_type>::impl(const Dist &dist)
    : dist{dist}, y_max{dist.pdf(dist.mode)}, total_area{dist.strip_area(dist.mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_signed_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}
{
  if (dist.mode == dist.support)
    return;
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");
  if (dist.pdf(dist.support) != 0)
    throw std::logic_error("PDF should be zero at the support boundary");
  if (dist.strip_area(dist.support) != 0)
    throw std::logic_error("strip_area must return zero for a strip of zero height");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = binary_search_for_strip_coord<Dist, float_type>(dist.support, dist.mode, total_area * i / N, dist) - dist.mode;
    y_corner[i] = dist.pdf(dist.mode + x_corner_rel_mode[i]);
    strip_width_w_signed_gen_range[i] = x_corner_rel_mode[i] * gen_signed_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }

  verify_efficiency(x_corner_rel_mode, y_corner, 0, N, total_area / N);

  x_corner_rel_mode[0] = dist.support - dist.mode;
  strip_width_w_signed_gen_range[0] = x_corner_rel_mode[0] * gen_signed_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<signed_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  signed_integer_type x = static_cast<signed_integer_type>(r & not_index_mask);
  while (true)
  {
    if (std::abs(x) < x_corner_ratio[index] || (gen() * strip_height_w_unsigned_gen_range[index + 1] < (dist.pdf(dist.mode + x * strip_width_w_signed_gen_range[index]) - y_corner[index])))
      return dist.mode + x * strip_width_w_signed_gen_range[index];
    x = static_cast<signed_integer_type>(gen() & not_index_mask);
  }
}

} // namespace finite

namespace map /* infinite tail generate by mapping */
{

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class static_impl
{
  static_assert(is_static_pdf<Dist>::value && Dist::dist_category == DistCategory::SYMMETRIC && Dist::tail_category == TailCategory::MAP);

public:
  using result_type = float_type;

  static_impl();

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  using signed_integer_type = std::make_signed_t<unsigned_integer_type>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr float_type gen_signed_range_inv = math::exp2_int<float_type>(-std::numeric_limits<signed_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr unsigned_integer_type sign_mask = 1;
  static constexpr unsigned_integer_type not_sign_mask = URBG::max() ^ sign_mask;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_signed_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type tail_start_rel_mode;
  signed_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
static_impl<Dist, URBG, N, float_type>::static_impl()
    : y_max{Dist::pdf(Dist::mode)}, total_area{Dist::strip_area(Dist::mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_signed_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = multiplicative_binary_search<Dist, float_type>(Dist::mode, Dist::mode + 1, total_area * i / N) - Dist::mode;
    y_corner[i] = Dist::pdf(Dist::mode + x_corner_rel_mode[i]);
    strip_width_w_signed_gen_range[i] = x_corner_rel_mode[i] * gen_signed_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  tail_start_rel_mode = x_corner_rel_mode[1];

  verify_efficiency(x_corner_rel_mode, y_corner, 1, N - 1, total_area / N);

  x_corner_rel_mode[0] = total_area / N / y_corner[1];
  strip_width_w_signed_gen_range[0] = x_corner_rel_mode[0] * gen_signed_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<signed_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type static_impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  signed_integer_type x = static_cast<signed_integer_type>(r & not_index_mask);
  while (true)
  {
    if (std::abs(x) < x_corner_ratio[index] || (index && (gen() * strip_height_w_unsigned_gen_range[index + 1] < (Dist::pdf(Dist::mode + x * strip_width_w_signed_gen_range[index]) - y_corner[index]))))
      return Dist::mode + x * strip_width_w_signed_gen_range[index];
    else if (index == 0)
    {
      float_type x_rel_mode = Dist::tail_value_rel_mode(tail_start_rel_mode, canonical_dist(gen));
      switch ((r / N) & sign_mask)
      {
      case sign_mask:
        return Dist::mode + x_rel_mode;
      case 0:
        return Dist::mode - x_rel_mode;
      }
    }
    x = static_cast<signed_integer_type>(gen() & not_index_mask);
  }
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class impl
{
  static_assert(!is_static_pdf<Dist>::value && Dist::dist_category == DistCategory::SYMMETRIC && Dist::tail_category == TailCategory::MAP);

public:
  using result_type = float_type;

  impl(const Dist &);

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  using signed_integer_type = std::make_signed_t<unsigned_integer_type>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr float_type gen_signed_range_inv = math::exp2_int<float_type>(-std::numeric_limits<signed_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr unsigned_integer_type sign_mask = 1;
  static constexpr unsigned_integer_type not_sign_mask = URBG::max() ^ sign_mask;
  const Dist &dist;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_signed_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type tail_start_rel_mode;
  signed_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
impl<Dist, URBG, N, float_type>::impl(const Dist &dist)
    : dist{dist}, y_max{dist.pdf(dist.mode)}, total_area{dist.strip_area(dist.mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_signed_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = multiplicative_binary_search<Dist, float_type>(dist.mode, dist.mode + 1, total_area * i / N, dist) - dist.mode;
    y_corner[i] = dist.pdf(dist.mode + x_corner_rel_mode[i]);
    strip_width_w_signed_gen_range[i] = x_corner_rel_mode[i] * gen_signed_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  tail_start_rel_mode = x_corner_rel_mode[1];

  verify_efficiency(x_corner_rel_mode, y_corner, 1, N - 1, total_area / N);

  x_corner_rel_mode[0] = total_area / N / y_corner[1];
  strip_width_w_signed_gen_range[0] = x_corner_rel_mode[0] * gen_signed_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<signed_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  signed_integer_type x = static_cast<signed_integer_type>(r & not_index_mask);
  while (true)
  {
    if (std::abs(x) < x_corner_ratio[index] || (index && (gen() * strip_height_w_unsigned_gen_range[index + 1] < (dist.pdf(dist.mode + x * strip_width_w_signed_gen_range[index]) - y_corner[index]))))
      return dist.mode + x * strip_width_w_signed_gen_range[index];
    else if (index == 0)
    {
      float_type x_rel_mode = dist.tail_value_rel_mode(tail_start_rel_mode, canonical_dist(gen));
      switch ((r / N) & sign_mask)
      {
      case sign_mask:
        return dist.mode + x_rel_mode;
      case 0:
        return dist.mode - x_rel_mode;
      }
    }
    x = static_cast<signed_integer_type>(gen() & not_index_mask);
  }
}

} // namespace map

namespace map_reject /* infinite tail generate by mapping+rejection sampling */
{

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class static_impl
{
  static_assert(is_static_pdf<Dist>::value && Dist::dist_category == DistCategory::SYMMETRIC && Dist::tail_category == TailCategory::MAP_REJECT);

public:
  using result_type = float_type;

  static_impl();

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  using signed_integer_type = std::make_signed_t<unsigned_integer_type>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr float_type gen_signed_range_inv = math::exp2_int<float_type>(-std::numeric_limits<signed_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr unsigned_integer_type sign_mask = 1;
  static constexpr unsigned_integer_type not_sign_mask = URBG::max() ^ sign_mask;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_signed_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type tail_start_rel_mode;
  signed_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
static_impl<Dist, URBG, N, float_type>::static_impl()
    : y_max{Dist::pdf(Dist::mode)}, total_area{Dist::strip_area(Dist::mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_signed_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = multiplicative_binary_search<Dist, float_type>(Dist::mode, Dist::mode + 1, total_area * i / N) - Dist::mode;
    y_corner[i] = Dist::pdf(Dist::mode + x_corner_rel_mode[i]);
    strip_width_w_signed_gen_range[i] = x_corner_rel_mode[i] * gen_signed_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  tail_start_rel_mode = x_corner_rel_mode[1];

  verify_efficiency(x_corner_rel_mode, y_corner, 1, N - 1, total_area / N);

  x_corner_rel_mode[0] = total_area / N / y_corner[1];
  strip_width_w_signed_gen_range[0] = x_corner_rel_mode[0] * gen_signed_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(std::round(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<signed_integer_type>::max()));

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type static_impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  signed_integer_type x = static_cast<signed_integer_type>(r & not_index_mask);
  while (true)
  {
    if (std::abs(x) < x_corner_ratio[index] || (index && (gen() * strip_height_w_unsigned_gen_range[index + 1] < (Dist::pdf(Dist::mode + x * strip_width_w_signed_gen_range[index]) - y_corner[index]))))
      return Dist::mode + x * strip_width_w_signed_gen_range[index];
    else if (index == 0)
      while (true)
      {
        unsigned_integer_type r1 = gen();
        float_type x_rel_mode = Dist::tail_value_rel_mode(tail_start_rel_mode, canonical_dist(gen));
        if ((r1 & not_sign_mask) * gen_unsigned_range_inv < Dist::tail_accept_probability(tail_start_rel_mode, x_rel_mode))
          switch (r1 & sign_mask)
          {
          case sign_mask:
            return Dist::mode + x_rel_mode;
          case 0:
            return Dist::mode - x_rel_mode;
          }
      }
    x = static_cast<signed_integer_type>(gen() & not_index_mask);
  }
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class impl
{
  static_assert(!is_static_pdf<Dist>::value && Dist::dist_category == DistCategory::SYMMETRIC && Dist::tail_category == TailCategory::MAP_REJECT);

public:
  using result_type = float_type;

  impl(const Dist &);

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  using signed_integer_type = std::make_signed_t<unsigned_integer_type>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr float_type gen_signed_range_inv = math::exp2_int<float_type>(-std::numeric_limits<signed_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr unsigned_integer_type sign_mask = 1;
  static constexpr unsigned_integer_type not_sign_mask = URBG::max() ^ sign_mask;
  const Dist &dist;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_signed_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type tail_start_rel_mode;
  signed_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
impl<Dist, URBG, N, float_type>::impl(const Dist &dist)
    : dist{dist}, y_max{dist.pdf(dist.mode)}, total_area{dist.strip_area(dist.mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_signed_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}
{
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = multiplicative_binary_search<Dist, float_type>(dist.mode, dist.mode + 1, total_area * i / N, dist) - dist.mode;
    y_corner[i] = dist.pdf(dist.mode + x_corner_rel_mode[i]);
    strip_width_w_signed_gen_range[i] = x_corner_rel_mode[i] * gen_signed_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  tail_start_rel_mode = x_corner_rel_mode[1];

  verify_efficiency(x_corner_rel_mode, y_corner, 1, N - 1, total_area / N);

  x_corner_rel_mode[0] = total_area / N / y_corner[1];
  strip_width_w_signed_gen_range[0] = x_corner_rel_mode[0] * gen_signed_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<signed_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  signed_integer_type x = static_cast<signed_integer_type>(r & not_index_mask);
  while (true)
  {
    if (std::abs(x) < x_corner_ratio[index] || (index && (gen() * strip_height_w_unsigned_gen_range[index + 1] < (dist.pdf(dist.mode + x * strip_width_w_signed_gen_range[index]) - y_corner[index]))))
      return dist.mode + x * strip_width_w_signed_gen_range[index];
    else if (index == 0)
      while (true)
      {
        unsigned_integer_type r1 = gen();
        float_type x_rel_mode = dist.tail_value_rel_mode(tail_start_rel_mode, canonical_dist(gen));
        if ((r1 & not_sign_mask) * gen_unsigned_range_inv < dist.tail_accept_probability(tail_start_rel_mode, x_rel_mode))
          switch (r1 & sign_mask)
          {
          case sign_mask:
            return dist.mode + x_rel_mode;
          case 0:
            return dist.mode - x_rel_mode;
          }
      }
    x = static_cast<signed_integer_type>(gen() & not_index_mask);
  }
}

} // namespace map_reject

} // namespace nonsingular

namespace singular /*mode*/
{

namespace finite /*tail*/
{

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class static_impl
{
  static_assert(is_static_pdf<Dist>::value && Dist::dist_category == DistCategory::SYMMETRIC && Dist::tail_category == TailCategory::FINITE);

public:
  using result_type = float_type;

  static_impl();

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  using signed_integer_type = std::make_signed_t<unsigned_integer_type>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr float_type gen_signed_range_inv = math::exp2_int<float_type>(-std::numeric_limits<signed_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr unsigned_integer_type sign_mask = 1;
  static constexpr unsigned_integer_type not_sign_mask = URBG::max() ^ sign_mask;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_signed_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type peak_start_rel_mode;
  signed_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
static_impl<Dist, URBG, N, float_type>::static_impl()
    : y_max{Dist::pdf(Dist::mode)}, total_area{Dist::strip_area(Dist::mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_signed_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (Dist::mode == Dist::support)
    return;
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");
  if (Dist::pdf(Dist::support) != 0)
    throw std::logic_error("PDF should be zero at the support boundary");
  if (Dist::strip_area(Dist::support) != 0)
    throw std::logic_error("strip_area must return zero for a strip of zero height");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = binary_search_for_strip_coord<Dist, float_type>(Dist::support, Dist::mode, total_area * i / N) - Dist::mode;
    y_corner[i] = Dist::pdf(Dist::mode + x_corner_rel_mode[i]);
    strip_width_w_signed_gen_range[i] = x_corner_rel_mode[i] * gen_signed_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  peak_start_rel_mode = x_corner_rel_mode[N - 1];

  verify_efficiency(x_corner_rel_mode, y_corner, 0, N - 1, total_area / N);

  x_corner_rel_mode[0] = Dist::support - Dist::mode;
  strip_width_w_signed_gen_range[0] = x_corner_rel_mode[0] * gen_signed_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<signed_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type static_impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  signed_integer_type x = static_cast<signed_integer_type>(r & not_index_mask);
  while (true)
  {
    if (std::abs(x) < x_corner_ratio[index] || ((index != N - 1 || !Dist::is_mode_unbounded) && gen() * strip_height_w_unsigned_gen_range[index + 1] < (Dist::pdf(Dist::mode + x * strip_width_w_signed_gen_range[index]) - y_corner[index])))
      return Dist::mode + x * strip_width_w_signed_gen_range[index];
    else if (index == N - 1 && Dist::is_mode_unbounded)
      while (true)
      {
        float_type x_rel_mode = Dist::peak_value_rel_mode(peak_start_rel_mode, canonical_dist(gen));
        //                 if (x == Dist::mode) continue;
        unsigned_integer_type r1 = gen();
        if ((r1 & not_sign_mask) * gen_unsigned_range_inv < Dist::peak_accept_probability(peak_start_rel_mode, x_rel_mode))
          switch (r1 & sign_mask)
          {
          case sign_mask:
            return Dist::mode + x_rel_mode;
          case 0:
            return Dist::mode - x_rel_mode;
          }
      }
    x = static_cast<signed_integer_type>(gen() & not_index_mask);
  }
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class impl
{
  static_assert(!is_static_pdf<Dist>::value && Dist::dist_category == DistCategory::SYMMETRIC && Dist::tail_category == TailCategory::FINITE);

public:
  using result_type = float_type;

  impl(const Dist &);

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  using signed_integer_type = std::make_signed_t<unsigned_integer_type>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr float_type gen_signed_range_inv = math::exp2_int<float_type>(-std::numeric_limits<signed_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr unsigned_integer_type sign_mask = 1;
  static constexpr unsigned_integer_type not_sign_mask = URBG::max() ^ sign_mask;
  const Dist &dist;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_signed_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type peak_start_rel_mode;
  signed_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
impl<Dist, URBG, N, float_type>::impl(const Dist &dist)
    : dist{dist}, y_max{dist.pdf(dist.mode)}, total_area{dist.strip_area(dist.mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_signed_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (dist.mode == dist.support)
    return;
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");
  if (dist.pdf(dist.support) != 0)
    throw std::logic_error("PDF should be zero at the support boundary");
  if (dist.strip_area(dist.support) != 0)
    throw std::logic_error("strip_area must return zero for a strip of zero height");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = binary_search_for_strip_coord<Dist, float_type>(dist.support, dist.mode, total_area * i / N, dist) - dist.mode;
    y_corner[i] = dist.pdf(dist.mode + x_corner_rel_mode[i]);
    strip_width_w_signed_gen_range[i] = x_corner_rel_mode[i] * gen_signed_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  peak_start_rel_mode = x_corner_rel_mode[N - 1];

  verify_efficiency(x_corner_rel_mode, y_corner, 0, dist.is_mode_unbounded ? N - 1 : N, total_area / N);

  x_corner_rel_mode[0] = dist.support - dist.mode;
  strip_width_w_signed_gen_range[0] = x_corner_rel_mode[0] * gen_signed_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<signed_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  signed_integer_type x = static_cast<signed_integer_type>(r & not_index_mask);
  while (true)
  {
    if (std::abs(x) < x_corner_ratio[index] || ((index != N - 1 || !dist.is_mode_unbounded) && gen() * strip_height_w_unsigned_gen_range[index + 1] < (dist.pdf(dist.mode + x * strip_width_w_signed_gen_range[index]) - y_corner[index])))
      return dist.mode + x * strip_width_w_signed_gen_range[index];
    else if (index == N - 1 && dist.is_mode_unbounded)
      while (true)
      {
        float_type x_rel_mode = dist.peak_value_rel_mode(peak_start_rel_mode, canonical_dist(gen));
        //                 if (x == dist.mode) continue;
        unsigned_integer_type r1 = gen();
        if ((r1 & not_sign_mask) * gen_unsigned_range_inv < dist.peak_accept_probability(peak_start_rel_mode, x_rel_mode))
          switch (r1 & sign_mask)
          {
          case sign_mask:
            return dist.mode + x_rel_mode;
          case 0:
            return dist.mode - x_rel_mode;
          }
      }
    x = static_cast<signed_integer_type>(gen() & not_index_mask);
  }
}

} // namespace finite

namespace map /* infinite tail generate by mapping */
{

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class static_impl
{
  static_assert(is_static_pdf<Dist>::value && Dist::dist_category == DistCategory::SYMMETRIC && Dist::tail_category == TailCategory::MAP);

public:
  using result_type = float_type;

  static_impl();

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  using signed_integer_type = std::make_signed_t<unsigned_integer_type>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr float_type gen_signed_range_inv = math::exp2_int<float_type>(-std::numeric_limits<signed_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr unsigned_integer_type sign_mask = 1;
  static constexpr unsigned_integer_type not_sign_mask = URBG::max() ^ sign_mask;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_signed_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type tail_start_rel_mode;
  float_type peak_start_rel_mode;
  signed_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
static_impl<Dist, URBG, N, float_type>::static_impl()
    : y_max{Dist::pdf(Dist::mode)}, total_area{Dist::strip_area(Dist::mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_signed_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = multiplicative_binary_search<Dist, float_type>(Dist::mode, Dist::mode + 1, total_area * i / N) - Dist::mode;
    y_corner[i] = Dist::pdf(Dist::mode + x_corner_rel_mode[i]);
    strip_width_w_signed_gen_range[i] = x_corner_rel_mode[i] * gen_signed_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  tail_start_rel_mode = x_corner_rel_mode[1];
  peak_start_rel_mode = x_corner_rel_mode[N - 1];

  verify_efficiency(x_corner_rel_mode, y_corner, 1, N - 2, total_area / N);

  x_corner_rel_mode[0] = total_area / N / y_corner[1];
  strip_width_w_signed_gen_range[0] = x_corner_rel_mode[0] * gen_signed_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<signed_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type static_impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  signed_integer_type x = static_cast<signed_integer_type>(r & not_index_mask);
  while (true)
  {
    if (std::abs(x) < x_corner_ratio[index] || (index && (index != N - 1 || !Dist::is_mode_unbounded) && (gen() * strip_height_w_unsigned_gen_range[index + 1] < (Dist::pdf(Dist::mode + x * strip_width_w_signed_gen_range[index]) - y_corner[index]))))
      return Dist::mode + x * strip_width_w_signed_gen_range[index];
    else if (index == N - 1 && Dist::is_mode_unbounded)
      while (true)
      {
        float_type x_rel_mode = Dist::peak_value_rel_mode(peak_start_rel_mode, canonical_dist(gen));
        //                 if (x == Dist::mode) continue;
        unsigned_integer_type r1 = gen();
        if ((r1 & not_sign_mask) * gen_unsigned_range_inv < Dist::peak_accept_probability(peak_start_rel_mode, x_rel_mode))
          switch (r1 & sign_mask)
          {
          case sign_mask:
            return Dist::mode + x_rel_mode;
          case 0:
            return Dist::mode - x_rel_mode;
          }
      }
    else if (index == 0)
    {
      float_type x_rel_mode = Dist::tail_value_rel_mode(tail_start_rel_mode, canonical_dist(gen));
      switch ((r / N) & sign_mask)
      {
      case sign_mask:
        return Dist::mode + x_rel_mode;
      case 0:
        return Dist::mode - x_rel_mode;
      }
    }
    x = static_cast<signed_integer_type>(gen() & not_index_mask);
  }
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class impl
{
  static_assert(!is_static_pdf<Dist>::value && Dist::dist_category == DistCategory::SYMMETRIC && Dist::tail_category == TailCategory::MAP);

public:
  using result_type = float_type;

  impl(const Dist &);

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  using signed_integer_type = std::make_signed_t<unsigned_integer_type>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr float_type gen_signed_range_inv = math::exp2_int<float_type>(-std::numeric_limits<signed_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr unsigned_integer_type sign_mask = 1;
  static constexpr unsigned_integer_type not_sign_mask = URBG::max() ^ sign_mask;
  const Dist &dist;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_signed_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type tail_start_rel_mode;
  float_type peak_start_rel_mode;
  signed_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
impl<Dist, URBG, N, float_type>::impl(const Dist &dist)
    : dist{dist}, y_max{dist.pdf(dist.mode)}, total_area{dist.strip_area(dist.mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_signed_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = multiplicative_binary_search<Dist, float_type>(dist.mode, dist.mode + 1, total_area * i / N, dist) - dist.mode;
    y_corner[i] = dist.pdf(dist.mode + x_corner_rel_mode[i]);
    strip_width_w_signed_gen_range[i] = x_corner_rel_mode[i] * gen_signed_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  tail_start_rel_mode = x_corner_rel_mode[1];
  peak_start_rel_mode = x_corner_rel_mode[N - 1];

  verify_efficiency(x_corner_rel_mode, y_corner, 1, dist.is_mode_unbounded ? N - 2 : N - 1, total_area / N);

  x_corner_rel_mode[0] = total_area / N / y_corner[1];
  strip_width_w_signed_gen_range[0] = x_corner_rel_mode[0] * gen_signed_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<signed_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  signed_integer_type x = static_cast<signed_integer_type>(r & not_index_mask);
  while (true)
  {
    if (std::abs(x) < x_corner_ratio[index] || (index && (index != N - 1 || !dist.is_mode_unbounded) && (gen() * strip_height_w_unsigned_gen_range[index + 1] < (dist.pdf(dist.mode + x * strip_width_w_signed_gen_range[index]) - y_corner[index]))))
      return dist.mode + x * strip_width_w_signed_gen_range[index];
    else if (index == N - 1 && dist.is_mode_unbounded)
      while (true)
      {
        float_type x_rel_mode = dist.peak_value_rel_mode(peak_start_rel_mode, canonical_dist(gen));
        //                 if (x == dist.mode) continue;
        unsigned_integer_type r1 = gen();
        if ((r1 & not_sign_mask) * gen_unsigned_range_inv < dist.peak_accept_probability(peak_start_rel_mode, x_rel_mode))
          switch (r1 & sign_mask)
          {
          case sign_mask:
            return dist.mode + x_rel_mode;
          case 0:
            return dist.mode - x_rel_mode;
          }
      }
    else if (index == 0)
    {
      float_type x_rel_mode = dist.tail_value_rel_mode(tail_start_rel_mode, canonical_dist(gen));
      switch ((r / N) & sign_mask)
      {
      case sign_mask:
        return dist.mode + x_rel_mode;
      case 0:
        return dist.mode - x_rel_mode;
      }
    }
    x = static_cast<signed_integer_type>(gen() & not_index_mask);
  }
}

} // namespace map

namespace map_reject /* infinite tail generate by mapping+rejection sampling */
{

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class static_impl
{
  static_assert(is_static_pdf<Dist>::value && Dist::dist_category == DistCategory::SYMMETRIC && Dist::tail_category == TailCategory::MAP_REJECT);

public:
  using result_type = float_type;

  static_impl();

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  using signed_integer_type = std::make_signed_t<unsigned_integer_type>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr float_type gen_signed_range_inv = math::exp2_int<float_type>(-std::numeric_limits<signed_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr unsigned_integer_type sign_mask = 1;
  static constexpr unsigned_integer_type not_sign_mask = URBG::max() ^ sign_mask;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_signed_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type tail_start_rel_mode;
  float_type peak_start_rel_mode;
  signed_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
static_impl<Dist, URBG, N, float_type>::static_impl()
    : y_max{Dist::pdf(Dist::mode)}, total_area{Dist::strip_area(Dist::mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_signed_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = multiplicative_binary_search<Dist, float_type>(Dist::mode, Dist::mode + 1, total_area * i / N) - Dist::mode;
    y_corner[i] = Dist::pdf(Dist::mode + x_corner_rel_mode[i]);
    strip_width_w_signed_gen_range[i] = x_corner_rel_mode[i] * gen_signed_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  tail_start_rel_mode = x_corner_rel_mode[1];
  peak_start_rel_mode = x_corner_rel_mode[N - 1];

  verify_efficiency(x_corner_rel_mode, y_corner, 1, N - 2, total_area / N);

  x_corner_rel_mode[0] = total_area / N / y_corner[1];
  strip_width_w_signed_gen_range[0] = x_corner_rel_mode[0] * gen_signed_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(std::round(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<signed_integer_type>::max()));

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type static_impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  signed_integer_type x = static_cast<signed_integer_type>(r & not_index_mask);
  while (true)
  {
    if (std::abs(x) < x_corner_ratio[index] || (index && (index != N - 1 || !Dist::is_mode_unbounded) && (gen() * strip_height_w_unsigned_gen_range[index + 1] < (Dist::pdf(Dist::mode + x * strip_width_w_signed_gen_range[index]) - y_corner[index]))))
      return Dist::mode + x * strip_width_w_signed_gen_range[index];
    else if (index == N - 1 && Dist::is_mode_unbounded)
      while (true)
      {
        float_type x_rel_mode = Dist::peak_value_rel_mode(peak_start_rel_mode, canonical_dist(gen));
        //                 if (x == Dist::mode) continue;
        unsigned_integer_type r1 = gen();
        if ((r1 & not_sign_mask) * gen_unsigned_range_inv < Dist::peak_accept_probability(peak_start_rel_mode, x_rel_mode))
          switch (r1 & sign_mask)
          {
          case sign_mask:
            return Dist::mode + x_rel_mode;
          case 0:
            return Dist::mode - x_rel_mode;
          }
      }
    else if (index == 0)
      while (true)
      {
        unsigned_integer_type r1 = gen();
        float_type x_rel_mode = Dist::tail_value_rel_mode(tail_start_rel_mode, canonical_dist(gen));
        if ((r1 & not_sign_mask) * gen_unsigned_range_inv < Dist::tail_accept_probability(tail_start_rel_mode, x_rel_mode))
          switch (r1 & sign_mask)
          {
          case sign_mask:
            return Dist::mode + x_rel_mode;
          case 0:
            return Dist::mode - x_rel_mode;
          }
      }
    x = static_cast<signed_integer_type>(gen() & not_index_mask);
  }
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class impl
{
  static_assert(!is_static_pdf<Dist>::value && Dist::dist_category == DistCategory::SYMMETRIC && Dist::tail_category == TailCategory::MAP_REJECT);

public:
  using result_type = float_type;

  impl(const Dist &);

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  using signed_integer_type = std::make_signed_t<unsigned_integer_type>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr float_type gen_signed_range_inv = math::exp2_int<float_type>(-std::numeric_limits<signed_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr unsigned_integer_type sign_mask = 1;
  static constexpr unsigned_integer_type not_sign_mask = URBG::max() ^ sign_mask;
  const Dist &dist;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_signed_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type tail_start_rel_mode;
  float_type peak_start_rel_mode;
  signed_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
impl<Dist, URBG, N, float_type>::impl(const Dist &dist)
    : dist{dist}, y_max{dist.pdf(dist.mode)}, total_area{dist.strip_area(dist.mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_signed_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}
{
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = multiplicative_binary_search<Dist, float_type>(dist.mode, dist.mode + 1, total_area * i / N, dist) - dist.mode;
    y_corner[i] = dist.pdf(dist.mode + x_corner_rel_mode[i]);
    strip_width_w_signed_gen_range[i] = x_corner_rel_mode[i] * gen_signed_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  tail_start_rel_mode = x_corner_rel_mode[1];
  peak_start_rel_mode = x_corner_rel_mode[N - 1];

  verify_efficiency(x_corner_rel_mode, y_corner, 1, dist.is_mode_unbounded ? N - 2 : N - 1, total_area / N);

  x_corner_rel_mode[0] = total_area / N / y_corner[1];
  strip_width_w_signed_gen_range[0] = x_corner_rel_mode[0] * gen_signed_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<signed_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  signed_integer_type x = static_cast<signed_integer_type>(r & not_index_mask);
  while (true)
  {
    if (std::abs(x) < x_corner_ratio[index] || (index && (index != N - 1 || !dist.is_mode_unbounded) && (gen() * strip_height_w_unsigned_gen_range[index + 1] < (dist.pdf(dist.mode + x * strip_width_w_signed_gen_range[index]) - y_corner[index]))))
      return dist.mode + x * strip_width_w_signed_gen_range[index];
    else if (index == N - 1 && dist.is_mode_unbounded)
      while (true)
      {
        float_type x_rel_mode = dist.peak_value_rel_mode(peak_start_rel_mode, canonical_dist(gen));
        //                 if (x == dist.mode) continue;
        unsigned_integer_type r1 = gen();
        if ((r1 & not_sign_mask) * gen_unsigned_range_inv < dist.peak_accept_probability(peak_start_rel_mode, x_rel_mode))
          switch (r1 & sign_mask)
          {
          case sign_mask:
            return dist.mode + x_rel_mode;
          case 0:
            return dist.mode - x_rel_mode;
          }
      }
    else if (index == 0)
      while (true)
      {
        unsigned_integer_type r1 = gen();
        float_type x_rel_mode = dist.tail_value_rel_mode(tail_start_rel_mode, canonical_dist(gen));
        if ((r1 & not_sign_mask) * gen_unsigned_range_inv < dist.tail_accept_probability(tail_start_rel_mode, x_rel_mode))
          switch (r1 & sign_mask)
          {
          case sign_mask:
            return dist.mode + x_rel_mode;
          case 0:
            return dist.mode - x_rel_mode;
          }
      }
    x = static_cast<signed_integer_type>(gen() & not_index_mask);
  }
}

} // namespace map_reject

} // namespace singular

} // namespace symmetric

namespace monotonic /*distributions*/
{

namespace nonsingular /*mode*/
{

namespace finite /*tail*/
{

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class static_impl
{
  static_assert(is_static_pdf<Dist>::value && (Dist::dist_category == DistCategory::STRICTLY_DECREASING || Dist::dist_category == DistCategory::STRICTLY_INCREASING) && Dist::tail_category == TailCategory::FINITE);

public:
  using result_type = float_type;

  static_impl();

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr auto sign = Dist::dist_category == DistCategory::STRICTLY_DECREASING ? 1 : -1;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_unsigned_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  unsigned_integer_type x_corner_ratio[N];
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
static_impl<Dist, URBG, N, float_type>::static_impl()
    : y_max{Dist::pdf(Dist::mode)}, total_area{Dist::strip_area(Dist::mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_unsigned_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}
{
  if (Dist::mode == Dist::support)
    return;
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");
  if (Dist::pdf(Dist::support) != 0)
    throw std::logic_error("PDF should be zero at the support boundary");
  if (Dist::strip_area(Dist::support) != 0)
    throw std::logic_error("strip_area must return zero for a strip of zero height");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = binary_search_for_strip_coord<Dist, float_type>(Dist::support, Dist::mode, total_area * i / N) - Dist::mode;
    y_corner[i] = Dist::pdf(Dist::mode + x_corner_rel_mode[i]);
    strip_width_w_unsigned_gen_range[i] = x_corner_rel_mode[i] * gen_unsigned_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }

  x_corner_rel_mode[0] = Dist::support - Dist::mode;
  strip_width_w_unsigned_gen_range[0] = x_corner_rel_mode[0] * gen_unsigned_range_inv;

  verify_efficiency(x_corner_rel_mode, y_corner, 0, N, total_area / N);

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<unsigned_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type static_impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  unsigned_integer_type x = r & not_index_mask;
  while (true)
  {
    if (x < x_corner_ratio[index] || (gen() * strip_height_w_unsigned_gen_range[index + 1] < (Dist::pdf(x * strip_width_w_unsigned_gen_range[index] + Dist::mode) - y_corner[index])))
      return x * strip_width_w_unsigned_gen_range[index] + Dist::mode;
    x = gen() & not_index_mask;
  }
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class impl
{
  static_assert(!is_static_pdf<Dist>::value && (Dist::dist_category == DistCategory::STRICTLY_DECREASING || Dist::dist_category == DistCategory::STRICTLY_INCREASING) && Dist::tail_category == TailCategory::FINITE);

public:
  using result_type = float_type;

  impl(const Dist &);

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr auto sign = Dist::dist_category == DistCategory::STRICTLY_DECREASING ? 1 : -1;
  const Dist &dist;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_unsigned_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  unsigned_integer_type x_corner_ratio[N];
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
impl<Dist, URBG, N, float_type>::impl(const Dist &dist)
    : dist{dist}, y_max{dist.pdf(dist.mode)}, total_area{dist.strip_area(dist.mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_unsigned_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}
{
  if (dist.mode == dist.support)
    return;
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");
  if (dist.pdf(dist.support) != 0)
    throw std::logic_error("PDF should be zero at the support boundary");
  if (dist.strip_area(dist.support) != 0)
    throw std::logic_error("strip_area must return zero for a strip of zero height");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = binary_search_for_strip_coord<Dist, float_type>(dist.support, dist.mode, total_area * i / N, dist) - dist.mode;
    y_corner[i] = dist.pdf(dist.mode + x_corner_rel_mode[i]);
    strip_width_w_unsigned_gen_range[i] = x_corner_rel_mode[i] * gen_unsigned_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }

  x_corner_rel_mode[0] = dist.support - dist.mode;
  strip_width_w_unsigned_gen_range[0] = x_corner_rel_mode[0] * gen_unsigned_range_inv;

  verify_efficiency(x_corner_rel_mode, y_corner, 0, N, total_area / N);

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<unsigned_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  unsigned_integer_type x = r & not_index_mask;
  while (true)
  {
    if (x < x_corner_ratio[index] || (gen() * strip_height_w_unsigned_gen_range[index + 1] < (dist.pdf(x * strip_width_w_unsigned_gen_range[index] + dist.mode) - y_corner[index])))
      return x * strip_width_w_unsigned_gen_range[index] + dist.mode;
    x = gen() & not_index_mask;
  }
}

} // namespace finite

namespace map /* infinite tail generate by mapping */
{

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class static_impl
{
  static_assert(is_static_pdf<Dist>::value && (Dist::dist_category == DistCategory::STRICTLY_DECREASING || Dist::dist_category == DistCategory::STRICTLY_INCREASING) && Dist::tail_category == TailCategory::MAP);

public:
  using result_type = float_type;

  static_impl();

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr auto sign = Dist::dist_category == DistCategory::STRICTLY_DECREASING ? 1 : -1;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_unsigned_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type tail_start_rel_mode;
  unsigned_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
static_impl<Dist, URBG, N, float_type>::static_impl()
    : y_max{Dist::pdf(Dist::mode)}, total_area{Dist::strip_area(Dist::mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_unsigned_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = multiplicative_binary_search<Dist, float_type>(Dist::mode, Dist::mode + sign, total_area * i / N) - Dist::mode;
    y_corner[i] = Dist::pdf(Dist::mode + x_corner_rel_mode[i]);
    strip_width_w_unsigned_gen_range[i] = x_corner_rel_mode[i] * gen_unsigned_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  tail_start_rel_mode = x_corner_rel_mode[1];

  verify_efficiency(x_corner_rel_mode, y_corner, 1, N - 1, total_area / N);

  x_corner_rel_mode[0] = total_area / N / y_corner[1] * sign;
  strip_width_w_unsigned_gen_range[0] = x_corner_rel_mode[0] * gen_unsigned_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<unsigned_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type static_impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  unsigned_integer_type x = r & not_index_mask;
  while (true)
  {
    if (x < x_corner_ratio[index] || (index && (gen() * strip_height_w_unsigned_gen_range[index + 1] < (Dist::pdf(x * strip_width_w_unsigned_gen_range[index] + Dist::mode) - y_corner[index]))))
      return x * strip_width_w_unsigned_gen_range[index] + Dist::mode;
    else if (index == 0)
      return Dist::mode + Dist::tail_value_rel_mode(tail_start_rel_mode, canonical_dist(gen));
    x = gen() & not_index_mask;
  }
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class impl
{
  static_assert(!is_static_pdf<Dist>::value && (Dist::dist_category == DistCategory::STRICTLY_DECREASING || Dist::dist_category == DistCategory::STRICTLY_INCREASING) && Dist::tail_category == TailCategory::MAP);

public:
  using result_type = float_type;

  impl(const Dist &);

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr auto sign = Dist::dist_category == DistCategory::STRICTLY_DECREASING ? 1 : -1;
  const Dist &dist;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_unsigned_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type tail_start_rel_mode;
  unsigned_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
impl<Dist, URBG, N, float_type>::impl(const Dist &dist)
    : dist{dist}, y_max{dist.pdf(dist.mode)}, total_area{dist.strip_area(dist.mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_unsigned_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = multiplicative_binary_search<Dist, float_type>(dist.mode, dist.mode + sign, total_area * i / N, dist) - dist.mode;
    y_corner[i] = dist.pdf(dist.mode + x_corner_rel_mode[i]);
    strip_width_w_unsigned_gen_range[i] = x_corner_rel_mode[i] * gen_unsigned_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  tail_start_rel_mode = x_corner_rel_mode[1];

  verify_efficiency(x_corner_rel_mode, y_corner, 1, N - 1, total_area / N);

  x_corner_rel_mode[0] = total_area / N / y_corner[1] * sign;
  strip_width_w_unsigned_gen_range[0] = x_corner_rel_mode[0] * gen_unsigned_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<unsigned_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  unsigned_integer_type x = r & not_index_mask;
  while (true)
  {
    if (x < x_corner_ratio[index] || (index && (gen() * strip_height_w_unsigned_gen_range[index + 1] < (dist.pdf(x * strip_width_w_unsigned_gen_range[index] + dist.mode) - y_corner[index]))))
      return x * strip_width_w_unsigned_gen_range[index] + dist.mode;
    else if (index == 0)
      return dist.mode + dist.tail_value_rel_mode(tail_start_rel_mode, canonical_dist(gen));
    x = gen() & not_index_mask;
  }
}

} // namespace map

namespace map_reject /* infinite tail generate by mapping+rejection sampling */
{

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class static_impl
{
  static_assert(is_static_pdf<Dist>::value && (Dist::dist_category == DistCategory::STRICTLY_DECREASING || Dist::dist_category == DistCategory::STRICTLY_INCREASING) && Dist::tail_category == TailCategory::MAP_REJECT);

public:
  using result_type = float_type;

  static_impl();

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr auto sign = Dist::dist_category == DistCategory::STRICTLY_DECREASING ? 1 : -1;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_unsigned_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type tail_start_rel_mode;
  unsigned_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
static_impl<Dist, URBG, N, float_type>::static_impl()
    : y_max{Dist::pdf(Dist::mode)}, total_area{Dist::strip_area(Dist::mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_unsigned_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = multiplicative_binary_search<Dist, float_type>(Dist::mode, Dist::mode + sign, total_area * i / N) - Dist::mode;
    y_corner[i] = Dist::pdf(Dist::mode + x_corner_rel_mode[i]);
    strip_width_w_unsigned_gen_range[i] = x_corner_rel_mode[i] * gen_unsigned_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  tail_start_rel_mode = x_corner_rel_mode[1];

  verify_efficiency(x_corner_rel_mode, y_corner, 1, N - 1, total_area / N);

  x_corner_rel_mode[0] = total_area / N / y_corner[1] * sign;
  strip_width_w_unsigned_gen_range[0] = x_corner_rel_mode[0] * gen_unsigned_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<unsigned_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type static_impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  unsigned_integer_type x = r & not_index_mask;
  while (true)
  {
    if (x < x_corner_ratio[index] || (index && (gen() * strip_height_w_unsigned_gen_range[index + 1] < (Dist::pdf(x * strip_width_w_unsigned_gen_range[index] + Dist::mode) - y_corner[index]))))
      return x * strip_width_w_unsigned_gen_range[index] + Dist::mode;
    else if (index == 0)
      while (true)
      {
        float_type x_rel_mode = Dist::tail_value_rel_mode(tail_start_rel_mode, canonical_dist(gen));
        if (gen() * gen_unsigned_range_inv < Dist::tail_accept_probability(tail_start_rel_mode, x_rel_mode))
          return Dist::mode + x_rel_mode;
      }
    x = gen() & not_index_mask;
  }
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class impl
{
  static_assert(!is_static_pdf<Dist>::value && (Dist::dist_category == DistCategory::STRICTLY_DECREASING || Dist::dist_category == DistCategory::STRICTLY_INCREASING) && Dist::tail_category == TailCategory::MAP_REJECT);

public:
  using result_type = float_type;

  impl(const Dist &);

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr auto sign = Dist::dist_category == DistCategory::STRICTLY_DECREASING ? 1 : -1;
  const Dist &dist;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_unsigned_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type tail_start_rel_mode;
  unsigned_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
impl<Dist, URBG, N, float_type>::impl(const Dist &dist)
    : dist{dist}, y_max{dist.pdf(dist.mode)}, total_area{dist.strip_area(dist.mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_unsigned_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = multiplicative_binary_search<Dist, float_type>(dist.mode, dist.mode + sign, total_area * i / N, dist) - dist.mode;
    y_corner[i] = dist.pdf(dist.mode + x_corner_rel_mode[i]);
    strip_width_w_unsigned_gen_range[i] = x_corner_rel_mode[i] * gen_unsigned_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  tail_start_rel_mode = x_corner_rel_mode[1];

  verify_efficiency(x_corner_rel_mode, y_corner, 1, N - 1, total_area / N);

  x_corner_rel_mode[0] = total_area / N / y_corner[1] * sign;
  strip_width_w_unsigned_gen_range[0] = x_corner_rel_mode[0] * gen_unsigned_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<unsigned_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  unsigned_integer_type x = r & not_index_mask;
  while (true)
  {
    if (x < x_corner_ratio[index] || (index && (gen() * strip_height_w_unsigned_gen_range[index + 1] < (dist.pdf(x * strip_width_w_unsigned_gen_range[index] + dist.mode) - y_corner[index]))))
      return x * strip_width_w_unsigned_gen_range[index] + dist.mode;
    else if (index == 0)
      while (true)
      {
        float_type x_rel_mode = dist.tail_value_rel_mode(tail_start_rel_mode, canonical_dist(gen));
        if (gen() * gen_unsigned_range_inv < dist.tail_accept_probability(tail_start_rel_mode, x_rel_mode))
          return dist.mode + x_rel_mode;
      }
    x = gen() & not_index_mask;
  }
}

} // namespace map_reject

} // namespace nonsingular

namespace singular /*mode*/
{

namespace finite /*tail*/
{

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class static_impl
{
  static_assert(is_static_pdf<Dist>::value && (Dist::dist_category == DistCategory::STRICTLY_DECREASING || Dist::dist_category == DistCategory::STRICTLY_INCREASING) && Dist::tail_category == TailCategory::FINITE);

public:
  using result_type = float_type;

  static_impl();

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr auto sign = Dist::dist_category == DistCategory::STRICTLY_DECREASING ? 1 : -1;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_unsigned_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type peak_start_rel_mode;
  unsigned_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
static_impl<Dist, URBG, N, float_type>::static_impl()
    : y_max{Dist::pdf(Dist::mode)}, total_area{Dist::strip_area(Dist::mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_unsigned_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (Dist::mode == Dist::support)
    return;
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");
  if (Dist::pdf(Dist::support) != 0)
    throw std::logic_error("PDF should be zero at the support boundary");
  if (Dist::strip_area(Dist::support) != 0)
    throw std::logic_error("strip_area must return zero for a strip of zero height");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = binary_search_for_strip_coord<Dist, float_type>(Dist::support, Dist::mode, total_area * i / N) - Dist::mode;
    y_corner[i] = Dist::pdf(Dist::mode + x_corner_rel_mode[i]);
    strip_width_w_unsigned_gen_range[i] = x_corner_rel_mode[i] * gen_unsigned_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  peak_start_rel_mode = x_corner_rel_mode[N - 1];

  x_corner_rel_mode[0] = Dist::support - Dist::mode;
  strip_width_w_unsigned_gen_range[0] = x_corner_rel_mode[0] * gen_unsigned_range_inv;

  verify_efficiency(x_corner_rel_mode, y_corner, 0, N - 1, total_area / N);

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<unsigned_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type static_impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  unsigned_integer_type x = r & not_index_mask;
  while (true)
  {
    if (x < x_corner_ratio[index] || ((index != N - 1 || !Dist::is_mode_unbounded) && gen() * strip_height_w_unsigned_gen_range[index + 1] < (Dist::pdf(x * strip_width_w_unsigned_gen_range[index] + Dist::mode) - y_corner[index])))
      return x * strip_width_w_unsigned_gen_range[index] + Dist::mode;
    else if (index == N - 1 && Dist::is_mode_unbounded)
      while (true)
      {
        float_type x_rel_mode = Dist::peak_value_rel_mode(peak_start_rel_mode, canonical_dist(gen));
        //                 if (x == Dist::mode) continue;
        if (gen() * gen_unsigned_range_inv < Dist::peak_accept_probability(peak_start_rel_mode, x_rel_mode))
          return Dist::mode + x_rel_mode;
      }
    x = gen() & not_index_mask;
  }
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class impl
{
  static_assert(!is_static_pdf<Dist>::value && (Dist::dist_category == DistCategory::STRICTLY_DECREASING || Dist::dist_category == DistCategory::STRICTLY_INCREASING) && Dist::tail_category == TailCategory::FINITE);

public:
  using result_type = float_type;

  impl(const Dist &);

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr auto sign = Dist::dist_category == DistCategory::STRICTLY_DECREASING ? 1 : -1;
  const Dist &dist;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_unsigned_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type peak_start_rel_mode;
  unsigned_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
impl<Dist, URBG, N, float_type>::impl(const Dist &dist)
    : dist{dist}, y_max{dist.pdf(dist.mode)}, total_area{dist.strip_area(dist.mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_unsigned_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (dist.mode == dist.support)
    return;
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");
  if (dist.pdf(dist.support) != 0)
    throw std::logic_error("PDF should be zero at the support boundary");
  if (dist.strip_area(dist.support) != 0)
    throw std::logic_error("strip_area must return zero for a strip of zero height");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = binary_search_for_strip_coord<Dist, float_type>(dist.support, dist.mode, total_area * i / N, dist) - dist.mode;
    y_corner[i] = dist.pdf(dist.mode + x_corner_rel_mode[i]);
    strip_width_w_unsigned_gen_range[i] = x_corner_rel_mode[i] * gen_unsigned_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  peak_start_rel_mode = x_corner_rel_mode[N - 1];

  x_corner_rel_mode[0] = dist.support - dist.mode;
  strip_width_w_unsigned_gen_range[0] = x_corner_rel_mode[0] * gen_unsigned_range_inv;

  verify_efficiency(x_corner_rel_mode, y_corner, 0, dist.is_mode_unbounded ? N - 1 : N, total_area / N);

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<unsigned_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  unsigned_integer_type x = r & not_index_mask;
  while (true)
  {
    if (x < x_corner_ratio[index] || ((index != N - 1 || !dist.is_mode_unbounded) && gen() * strip_height_w_unsigned_gen_range[index + 1] < (dist.pdf(x * strip_width_w_unsigned_gen_range[index] + dist.mode) - y_corner[index])))
      return x * strip_width_w_unsigned_gen_range[index] + dist.mode;
    else if (index == N - 1 && dist.is_mode_unbounded)
      while (true)
      {
        float_type x_rel_mode = dist.peak_value_rel_mode(peak_start_rel_mode, canonical_dist(gen));
        //                 if (x == dist.mode) continue;
        if (gen() * gen_unsigned_range_inv < dist.peak_accept_probability(peak_start_rel_mode, x_rel_mode))
          return dist.mode + x_rel_mode;
      }
    x = gen() & not_index_mask;
  }
}

} // namespace finite

namespace map /* infinite tail generate by mapping */
{

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class static_impl
{
  static_assert(is_static_pdf<Dist>::value && (Dist::dist_category == DistCategory::STRICTLY_DECREASING || Dist::dist_category == DistCategory::STRICTLY_INCREASING) && Dist::tail_category == TailCategory::MAP);

public:
  using result_type = float_type;

  static_impl();

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr auto sign = Dist::dist_category == DistCategory::STRICTLY_DECREASING ? 1 : -1;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_unsigned_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type tail_start_rel_mode;
  float_type peak_start_rel_mode;
  unsigned_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
static_impl<Dist, URBG, N, float_type>::static_impl()
    : y_max{Dist::pdf(Dist::mode)}, total_area{Dist::strip_area(Dist::mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_unsigned_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = multiplicative_binary_search<Dist, float_type>(Dist::mode, Dist::mode + sign, total_area * i / N) - Dist::mode;
    y_corner[i] = Dist::pdf(Dist::mode + x_corner_rel_mode[i]);
    strip_width_w_unsigned_gen_range[i] = x_corner_rel_mode[i] * gen_unsigned_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  tail_start_rel_mode = x_corner_rel_mode[1];
  peak_start_rel_mode = x_corner_rel_mode[N - 1];

  verify_efficiency(x_corner_rel_mode, y_corner, 1, N - 2, total_area / N);

  x_corner_rel_mode[0] = total_area / N / y_corner[1] * sign;
  strip_width_w_unsigned_gen_range[0] = x_corner_rel_mode[0] * gen_unsigned_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<unsigned_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type static_impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  unsigned_integer_type x = r & not_index_mask;
  while (true)
  {
    if (x < x_corner_ratio[index] || (index && (index != N - 1 || !Dist::is_mode_unbounded) && (gen() * strip_height_w_unsigned_gen_range[index + 1] < (Dist::pdf(x * strip_width_w_unsigned_gen_range[index] + Dist::mode) - y_corner[index]))))
      return x * strip_width_w_unsigned_gen_range[index] + Dist::mode;
    else if (index == N - 1 && Dist::is_mode_unbounded)
      while (true)
      {
        float_type x_rel_mode = Dist::peak_value_rel_mode(peak_start_rel_mode, canonical_dist(gen));
        //                 if (x == Dist::mode) continue;
        if (gen() * gen_unsigned_range_inv < Dist::peak_accept_probability(peak_start_rel_mode, x_rel_mode))
          return Dist::mode + x_rel_mode;
      }
    else if (index == 0)
      return Dist::mode + Dist::tail_value_rel_mode(tail_start_rel_mode, canonical_dist(gen));
    x = gen() & not_index_mask;
  }
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class impl
{
  static_assert(!is_static_pdf<Dist>::value && (Dist::dist_category == DistCategory::STRICTLY_DECREASING || Dist::dist_category == DistCategory::STRICTLY_INCREASING) && Dist::tail_category == TailCategory::MAP);

public:
  using result_type = float_type;

  impl(const Dist &);

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr auto sign = Dist::dist_category == DistCategory::STRICTLY_DECREASING ? 1 : -1;
  const Dist &dist;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_unsigned_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type tail_start_rel_mode;
  float_type peak_start_rel_mode;
  unsigned_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
impl<Dist, URBG, N, float_type>::impl(const Dist &dist)
    : dist{dist}, y_max{dist.pdf(dist.mode)}, total_area{dist.strip_area(dist.mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_unsigned_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = multiplicative_binary_search<Dist, float_type>(dist.mode, dist.mode + sign, total_area * i / N, dist) - dist.mode;
    y_corner[i] = dist.pdf(dist.mode + x_corner_rel_mode[i]);
    strip_width_w_unsigned_gen_range[i] = x_corner_rel_mode[i] * gen_unsigned_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  tail_start_rel_mode = x_corner_rel_mode[1];
  peak_start_rel_mode = x_corner_rel_mode[N - 1];

  verify_efficiency(x_corner_rel_mode, y_corner, 1, dist.is_mode_unbounded ? N - 2 : N - 1, total_area / N);

  x_corner_rel_mode[0] = total_area / N / y_corner[1] * sign;
  strip_width_w_unsigned_gen_range[0] = x_corner_rel_mode[0] * gen_unsigned_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<unsigned_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  unsigned_integer_type x = r & not_index_mask;
  while (true)
  {
    if (x < x_corner_ratio[index] || (index && (index != N - 1 || !dist.is_mode_unbounded) && (gen() * strip_height_w_unsigned_gen_range[index + 1] < (dist.pdf(x * strip_width_w_unsigned_gen_range[index] + dist.mode) - y_corner[index]))))
      return x * strip_width_w_unsigned_gen_range[index] + dist.mode;
    else if (index == N - 1 && dist.is_mode_unbounded)
      while (true)
      {
        float_type x_rel_mode = dist.peak_value_rel_mode(peak_start_rel_mode, canonical_dist(gen));
        //                 if (x == dist.mode) continue;
        if (gen() * gen_unsigned_range_inv < dist.peak_accept_probability(peak_start_rel_mode, x_rel_mode))
          return dist.mode + x_rel_mode;
      }
    else if (index == 0)
      return dist.mode + dist.tail_value_rel_mode(tail_start_rel_mode, canonical_dist(gen));
    x = gen() & not_index_mask;
  }
}

} // namespace map

namespace map_reject /* infinite tail generate by mapping+rejection sampling */
{

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class static_impl
{
  static_assert(is_static_pdf<Dist>::value && (Dist::dist_category == DistCategory::STRICTLY_DECREASING || Dist::dist_category == DistCategory::STRICTLY_INCREASING) && Dist::tail_category == TailCategory::MAP_REJECT);

public:
  using result_type = float_type;

  static_impl();

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr auto sign = Dist::dist_category == DistCategory::STRICTLY_DECREASING ? 1 : -1;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_unsigned_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type tail_start_rel_mode;
  float_type peak_start_rel_mode;
  unsigned_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
static_impl<Dist, URBG, N, float_type>::static_impl()
    : y_max{Dist::pdf(Dist::mode)}, total_area{Dist::strip_area(Dist::mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_unsigned_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = multiplicative_binary_search<Dist, float_type>(Dist::mode, Dist::mode + sign, total_area * i / N) - Dist::mode;
    y_corner[i] = Dist::pdf(Dist::mode + x_corner_rel_mode[i]);
    strip_width_w_unsigned_gen_range[i] = x_corner_rel_mode[i] * gen_unsigned_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  tail_start_rel_mode = x_corner_rel_mode[1];
  peak_start_rel_mode = x_corner_rel_mode[N - 1];

  verify_efficiency(x_corner_rel_mode, y_corner, 1, Dist::is_mode_unbounded ? N - 2 : N - 1, total_area / N);

  x_corner_rel_mode[0] = total_area / N / y_corner[1] * sign;
  strip_width_w_unsigned_gen_range[0] = x_corner_rel_mode[0] * gen_unsigned_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<unsigned_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type static_impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  unsigned_integer_type x = r & not_index_mask;
  while (true)
  {
    if (x < x_corner_ratio[index] || (index && (index != N - 1 || !Dist::is_mode_unbounded) && (gen() * strip_height_w_unsigned_gen_range[index + 1] < (Dist::pdf(x * strip_width_w_unsigned_gen_range[index] + Dist::mode) - y_corner[index]))))
      return x * strip_width_w_unsigned_gen_range[index] + Dist::mode;
    else if (index == N - 1 && Dist::is_mode_unbounded)
      while (true)
      {
        float_type x_rel_mode = Dist::peak_value_rel_mode(peak_start_rel_mode, canonical_dist(gen));
        //                 if (x == Dist::mode) continue;
        if (gen() * gen_unsigned_range_inv < Dist::peak_accept_probability(peak_start_rel_mode, x_rel_mode))
          return Dist::mode + x_rel_mode;
      }
    else if (index == 0)
      while (true)
      {
        float_type x_rel_mode = Dist::tail_value_rel_mode(tail_start_rel_mode, canonical_dist(gen));
        if (gen() * gen_unsigned_range_inv < Dist::tail_accept_probability(tail_start_rel_mode, x_rel_mode))
          return Dist::mode + x_rel_mode;
      }
    x = gen() & not_index_mask;
  }
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class impl
{
  static_assert(!is_static_pdf<Dist>::value && (Dist::dist_category == DistCategory::STRICTLY_DECREASING || Dist::dist_category == DistCategory::STRICTLY_INCREASING) && Dist::tail_category == TailCategory::MAP_REJECT);

public:
  using result_type = float_type;

  impl(const Dist &);

  float_type operator()(URBG &) const;

private:
  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  static constexpr unsigned_integer_type index_mask = N - 1;
  static constexpr unsigned_integer_type not_index_mask = URBG::max() ^ index_mask;
  static constexpr auto sign = Dist::dist_category == DistCategory::STRICTLY_DECREASING ? 1 : -1;
  const Dist &dist;
  const float_type y_max;
  const float_type total_area;
  float_type x_corner_rel_mode[N + 1], y_corner[N + 1], strip_width_w_unsigned_gen_range[N + 1], strip_height_w_unsigned_gen_range[N + 1];
  float_type tail_start_rel_mode;
  float_type peak_start_rel_mode;
  unsigned_integer_type x_corner_ratio[N];
  const canonical_float_random<float_type, URBG> canonical_dist;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
impl<Dist, URBG, N, float_type>::impl(const Dist &dist)
    : dist{dist}, y_max{dist.pdf(dist.mode)}, total_area{dist.strip_area(dist.mode)}, x_corner_rel_mode{0}, y_corner{0}, strip_width_w_unsigned_gen_range{0}, strip_height_w_unsigned_gen_range{0}, x_corner_ratio{0}, canonical_dist{}
{
  if (y_max <= 0)
    throw std::logic_error("PDF of mode must be positive (and PDF in general must be non-negative)");
  if (total_area <= 0)
    throw std::logic_error("strip_area must return a positive value for all positive arguments");

  for (int i = 1; i <= N; ++i)
  {
    x_corner_rel_mode[i] = multiplicative_binary_search<Dist, float_type>(dist.mode, dist.mode + sign, total_area * i / N, dist) - dist.mode;
    y_corner[i] = dist.pdf(dist.mode + x_corner_rel_mode[i]);
    strip_width_w_unsigned_gen_range[i] = x_corner_rel_mode[i] * gen_unsigned_range_inv;
    strip_height_w_unsigned_gen_range[i] = (y_corner[i] - y_corner[i - 1]) * gen_unsigned_range_inv;
  }
  tail_start_rel_mode = x_corner_rel_mode[1];
  peak_start_rel_mode = x_corner_rel_mode[N - 1];

  verify_efficiency(x_corner_rel_mode, y_corner, 1, dist.is_mode_unbounded ? N - 2 : N - 1, total_area / N);

  x_corner_rel_mode[0] = total_area / N / y_corner[1] * sign;
  strip_width_w_unsigned_gen_range[0] = x_corner_rel_mode[0] * gen_unsigned_range_inv;

  for (int i = 0; i < N; ++i)
    x_corner_ratio[i] = static_cast<unsigned_integer_type>(x_corner_rel_mode[i + 1] / x_corner_rel_mode[i] * std::numeric_limits<unsigned_integer_type>::max());

  if (y_corner[N] != y_max || x_corner_rel_mode[N] != 0)
    throw std::logic_error("unexpected error");
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  unsigned_integer_type r = gen();
  uint_fast16_t index = r & index_mask;
  unsigned_integer_type x = r & not_index_mask;
  while (true)
  {
    if (x < x_corner_ratio[index] || (index && (index != N - 1 || !dist.is_mode_unbounded) && (gen() * strip_height_w_unsigned_gen_range[index + 1] < (dist.pdf(x * strip_width_w_unsigned_gen_range[index] + dist.mode) - y_corner[index]))))
      return x * strip_width_w_unsigned_gen_range[index] + dist.mode;
    else if (index == N - 1 && dist.is_mode_unbounded)
      while (true)
      {
        float_type x_rel_mode = dist.peak_value_rel_mode(peak_start_rel_mode, canonical_dist(gen));
        //                 if (x == dist.mode) continue;
        if (gen() * gen_unsigned_range_inv < dist.peak_accept_probability(peak_start_rel_mode, x_rel_mode))
          return dist.mode + x_rel_mode;
      }
    else if (index == 0)
      while (true)
      {
        float_type x_rel_mode = dist.tail_value_rel_mode(tail_start_rel_mode, canonical_dist(gen));
        if (gen() * gen_unsigned_range_inv < dist.tail_accept_probability(tail_start_rel_mode, x_rel_mode))
          return dist.mode + x_rel_mode;
      }
    x = gen() & not_index_mask;
  }
}

} // namespace map_reject

} // namespace singular

} // namespace monotonic

namespace asymmetric /*distributions*/
{

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class static_impl
{
  static_assert(is_static_pdf<Dist>::value && Dist::dist_category == DistCategory::ASYMMETRIC);

  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  const float_type area_right, area_left, total_area;
  const bool one_sided;
  Ziggurat<typename Dist::Right, URBG, N, float_type> right;
  Ziggurat<typename Dist::Left, URBG, N, float_type> left;
  const canonical_float_random<float_type, URBG> canonical_dist;

public:
  using result_type = float_type;

  static_impl();

  float_type operator()(URBG &) const;
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
static_impl<Dist, URBG, N, float_type>::static_impl()
    : area_right{Dist::Right::strip_area(Dist::Right::mode)}, area_left{Dist::Left::strip_area(Dist::Left::mode)}, total_area{area_right + area_left}, one_sided{area_left == 0 || area_right == 0}, canonical_dist{}, right{}, left{} {}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
float_type static_impl<Dist, URBG, N, float_type>::operator()(URBG &gen) const
{
  if (one_sided)
  {
    if (area_right)
      return right(gen);
    else if (area_left)
      return left(gen);
  }
  else if (gen() * gen_unsigned_range_inv < area_right / total_area)
    return right(gen);
  else
    return left(gen);
}

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
class impl
{
  static_assert(!is_static_pdf<Dist>::value && Dist::dist_category == DistCategory::ASYMMETRIC);

  using unsigned_integer_type = type_utils::unsigned_integer_with_max_t<URBG::max()>;
  static constexpr float_type gen_unsigned_range_inv = math::exp2_int<float_type>(-std::numeric_limits<unsigned_integer_type>::digits);
  const float_type area_right, area_left, total_area;
  const bool one_sided;
  Ziggurat<typename Dist::Right, URBG, N, float_type> right;
  Ziggurat<typename Dist::Left, URBG, N, float_type> left;
  const canonical_float_random<float_type, URBG> canonical_dist;

public:
  using result_type = float_type;

  impl(const Dist &);

  float_type operator()(URBG &gen) const
  {
    if (one_sided)
    {
      if (area_right)
        return right(gen);
      else if (area_left)
        return left(gen);
    }
    else if (canonical_dist(gen) < area_right / total_area)
      return right(gen);
    else
      return left(gen);
  }
};

template <class Dist, class URBG, uint_fast16_t N, typename float_type>
impl<Dist, URBG, N, float_type>::impl(const Dist &dist)
    : area_right{dist.right.strip_area(dist.right.mode)}, area_left{dist.left.strip_area(dist.left.mode)}, total_area{area_right + area_left}, one_sided{area_left == 0 || area_right == 0}, right{dist.right}, left{dist.left} {}

} // namespace asymmetric

namespace type_utils
{

template <bool is_static, bool is_singular, DistCategory dist_category, TailCategory tail_category, class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector
{
  using type = substitution_failure;
};

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<true, false, DistCategory::SYMMETRIC, TailCategory::FINITE, Distribution, URBG, N, float_type>
{
  using type = symmetric::nonsingular::finite::static_impl<Distribution, URBG, N, float_type>;
};

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<false, false, DistCategory::SYMMETRIC, TailCategory::FINITE, Distribution, URBG, N, float_type>
{
  using type = symmetric::nonsingular::finite::impl<Distribution, URBG, N, float_type>;
};

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<true, false, DistCategory::SYMMETRIC, TailCategory::MAP, Distribution, URBG, N, float_type>
{
  using type = symmetric::nonsingular::map::static_impl<Distribution, URBG, N, float_type>;
};

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<false, false, DistCategory::SYMMETRIC, TailCategory::MAP, Distribution, URBG, N, float_type>
{
  using type = symmetric::nonsingular::map::impl<Distribution, URBG, N, float_type>;
};

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<true, false, DistCategory::SYMMETRIC, TailCategory::MAP_REJECT, Distribution, URBG, N, float_type>
{
  using type = symmetric::nonsingular::map_reject::static_impl<Distribution, URBG, N, float_type>;
};

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<false, false, DistCategory::SYMMETRIC, TailCategory::MAP_REJECT, Distribution, URBG, N, float_type>
{
  using type = symmetric::nonsingular::map_reject::impl<Distribution, URBG, N, float_type>;
};

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<true, true, DistCategory::SYMMETRIC, TailCategory::FINITE, Distribution, URBG, N, float_type>
{
  using type = symmetric::singular::finite::static_impl<Distribution, URBG, N, float_type>;
};

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<false, true, DistCategory::SYMMETRIC, TailCategory::FINITE, Distribution, URBG, N, float_type>
{
  using type = symmetric::singular::finite::impl<Distribution, URBG, N, float_type>;
};

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<true, true, DistCategory::SYMMETRIC, TailCategory::MAP, Distribution, URBG, N, float_type>
{
  using type = symmetric::singular::map::static_impl<Distribution, URBG, N, float_type>;
};

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<false, true, DistCategory::SYMMETRIC, TailCategory::MAP, Distribution, URBG, N, float_type>
{
  using type = symmetric::singular::map::impl<Distribution, URBG, N, float_type>;
};

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<true, true, DistCategory::SYMMETRIC, TailCategory::MAP_REJECT, Distribution, URBG, N, float_type>
{
  using type = symmetric::singular::map_reject::static_impl<Distribution, URBG, N, float_type>;
};

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<false, true, DistCategory::SYMMETRIC, TailCategory::MAP_REJECT, Distribution, URBG, N, float_type>
{
  using type = symmetric::singular::map_reject::impl<Distribution, URBG, N, float_type>;
};

template <DistCategory dist_category, class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<true, false, dist_category, TailCategory::FINITE, Distribution, URBG, N, float_type>
{
  using type = monotonic::nonsingular::finite::static_impl<Distribution, URBG, N, float_type>;
};

template <DistCategory dist_category, class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<false, false, dist_category, TailCategory::FINITE, Distribution, URBG, N, float_type>
{
  using type = monotonic::nonsingular::finite::impl<Distribution, URBG, N, float_type>;
};

template <DistCategory dist_category, class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<true, false, dist_category, TailCategory::MAP, Distribution, URBG, N, float_type>
{
  using type = monotonic::nonsingular::map::static_impl<Distribution, URBG, N, float_type>;
};

template <DistCategory dist_category, class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<false, false, dist_category, TailCategory::MAP, Distribution, URBG, N, float_type>
{
  using type = monotonic::nonsingular::map::impl<Distribution, URBG, N, float_type>;
};

template <DistCategory dist_category, class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<true, false, dist_category, TailCategory::MAP_REJECT, Distribution, URBG, N, float_type>
{
  using type = monotonic::nonsingular::map_reject::static_impl<Distribution, URBG, N, float_type>;
};

template <DistCategory dist_category, class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<false, false, dist_category, TailCategory::MAP_REJECT, Distribution, URBG, N, float_type>
{
  using type = monotonic::nonsingular::map_reject::impl<Distribution, URBG, N, float_type>;
};

template <DistCategory dist_category, class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<true, true, dist_category, TailCategory::FINITE, Distribution, URBG, N, float_type>
{
  using type = monotonic::singular::finite::static_impl<Distribution, URBG, N, float_type>;
};

template <DistCategory dist_category, class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<false, true, dist_category, TailCategory::FINITE, Distribution, URBG, N, float_type>
{
  using type = monotonic::singular::finite::impl<Distribution, URBG, N, float_type>;
};

template <DistCategory dist_category, class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<true, true, dist_category, TailCategory::MAP, Distribution, URBG, N, float_type>
{
  using type = monotonic::singular::map::static_impl<Distribution, URBG, N, float_type>;
};

template <DistCategory dist_category, class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<false, true, dist_category, TailCategory::MAP, Distribution, URBG, N, float_type>
{
  using type = monotonic::singular::map::impl<Distribution, URBG, N, float_type>;
};

template <DistCategory dist_category, class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<true, true, dist_category, TailCategory::MAP_REJECT, Distribution, URBG, N, float_type>
{
  using type = monotonic::singular::map_reject::static_impl<Distribution, URBG, N, float_type>;
};

template <DistCategory dist_category, class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct sellector<false, true, dist_category, TailCategory::MAP_REJECT, Distribution, URBG, N, float_type>
{
  using type = monotonic::singular::map_reject::impl<Distribution, URBG, N, float_type>;
};

template <bool is_static, bool is_singular, DistCategory dist_category, TailCategory tail_category, class Distribution, class URBG, uint_fast16_t N, typename float_type>
using sellector_t = typename sellector<is_static, is_singular, dist_category, tail_category, Distribution, URBG, N, float_type>::type;

template <bool has_static_sigularity_flag, class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct singularity_sellector
{
  using type = substitution_failure;
};

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct singularity_sellector<true, Distribution, URBG, N, float_type>
{
  using type = sellector_t<is_static_pdf<Distribution>::value, Distribution::is_mode_unbounded, Distribution::dist_category, Distribution::tail_category, Distribution, URBG, N, float_type>;
};

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct singularity_sellector<false, Distribution, URBG, N, float_type>
{
  using type = sellector_t<is_static_pdf<Distribution>::value, true, Distribution::dist_category, Distribution::tail_category, Distribution, URBG, N, float_type>;
};

template <bool has_static_sigularity_flag, class Distribution, class URBG, uint_fast16_t N, typename float_type>
using singularity_sellector_t = typename singularity_sellector<has_static_sigularity_flag, Distribution, URBG, N, float_type>::type;

template <bool is_static, DistCategory dist_category, class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct distribution_sellector
{
  using type = singularity_sellector_t<has_static_is_mode_unbounded<Distribution>::value, Distribution, URBG, N, float_type>;
};

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct distribution_sellector<true, DistCategory::ASYMMETRIC, Distribution, URBG, N, float_type>
{
  using type = asymmetric::static_impl<Distribution, URBG, N, float_type>;
};

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct distribution_sellector<false, DistCategory::ASYMMETRIC, Distribution, URBG, N, float_type>
{
  using type = asymmetric::impl<Distribution, URBG, N, float_type>;
};

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
struct verifier_sellector
{
  static_assert(has_dist_category<Distribution>::value && is_valid_pdf<Distribution>::value, "Invalid PDF class");
  static_assert(N > 0 && ((N & (N - 1)) == 0), "N must be an integer power of two");
  static_assert(std::is_integral<typename URBG::result_type>::value && std::is_unsigned<typename URBG::result_type>::value, "Uniform random number generator must produce unsigned integers");
  static_assert(URBG::min() == 0u, "Uniform random number generator's min must be zero");
  static_assert(unsigned_integer_with_max<URBG::max()>::exist, "Uniform random number generator's max must be either 2^32-1 or 2^64-1");
  static_assert(std::is_floating_point<float_type>::value, "float_type must be a floating point type");
  static constexpr auto need_sign = Distribution::dist_category == DistCategory::SYMMETRIC;
  static constexpr auto needed_range = N * (1 + need_sign);
  static_assert(URBG::max() >= needed_range - 1 && URBG::max() % needed_range == needed_range - 1, "Uniform random number generator range must be greater than and divisible by the needed range");

  using type = typename distribution_sellector<is_static_pdf<Distribution>::value, Distribution::dist_category, Distribution, URBG, N, float_type>::type;
};

} // namespace type_utils
} // namespace detail
} // namespace zest

template <class Distribution, class URBG, uint_fast16_t N, typename float_type>
class Ziggurat : public zest::detail::type_utils::verifier_sellector<Distribution, URBG, N, float_type>::type
{
public:
  Ziggurat() : zest::detail::type_utils::verifier_sellector<Distribution, URBG, N, float_type>::type{} {}

  Ziggurat(const Distribution &dist) : zest::detail::type_utils::verifier_sellector<Distribution, URBG, N, float_type>::type{dist} {}
};

#endif
#endif
