#ifndef CANONICAL_FLOAT_RANDOM_H
#define CANONICAL_FLOAT_RANDOM_H

#if __cplusplus < 201402L
#error "C++14 compatible compiler required"
#else

#include <limits>
#include <cstdint>
#include <cmath>
#include <type_traits>

namespace canonical_float_random_detail {
  struct substitution_failure {};

  template <uintmax_t M>
  struct unsigned_integer_with_max {
    using type = substitution_failure;
    static constexpr bool exist = false;
  };

  template <>
  struct unsigned_integer_with_max<std::numeric_limits<uint32_t>::max()> {
    using type = uint32_t;
    static constexpr bool exist = true;
  };

  template <>
  struct unsigned_integer_with_max<std::numeric_limits<uint64_t>::max()> {
    using type = uint64_t;
    static constexpr bool exist = true;
  };

  constexpr double exp2_int (int exp) {
    double x {1};
    if (exp > 0) for (int i = 0; i < exp; ++i) x *= 2;
    else if (exp < 0) for (int i = 0; i > exp; --i) x /= 2.;
    return x;
  }
}

template <typename float_t, typename URBG, uint_fast8_t cache_max_bits = 15u>
class canonical_float_random {
  static_assert (std::is_floating_point<float_t>::value, "float_t must be a floating-point type");
  static_assert (std::is_integral<typename URBG::result_type>::value && std::is_unsigned<typename 
URBG::result_type>::value, "Uniform random bit generator must produce unsigned integers");
  static_assert (URBG::min() == 0u, "Uniform random bit generator\'s min must be zero");
  static_assert (canonical_float_random_detail::unsigned_integer_with_max<URBG::max()>::exist,
                 "Uniform random bit generator\'s max must be either 2^32-1 or 2^64-1");
  
  using uint_t = typename canonical_float_random_detail::unsigned_integer_with_max<URBG::max()>::type;
  
public:
  // digits counts the implicit bit as well, hence the -1
  static constexpr auto num_float_bits = std::numeric_limits<float_t>::digits - 1u;
  static constexpr auto num_uint_bits = std::numeric_limits<uint_t>::digits;
  
  static constexpr uint_fast8_t min_urbg_calls_needed = num_float_bits/num_uint_bits + 1u;
  // number of extra bits that will be used to kickstart geometric variate generation
  static constexpr uint_fast8_t num_extra_bits = min_urbg_calls_needed*num_uint_bits - num_float_bits;
  // mask for num_extra_bits least significant bits of the random integer
  static constexpr uint_t exponent_mask = (1u << num_extra_bits) - 1u;
  
  // each time all the extra bits are zero, a new random integer is needed and num_extra_bits should 
  // be subtracted from the exponent, or we can multiply the result by 2^-num_extra_bits
  static constexpr auto exp2_num_extra_bits = canonical_float_random_detail::exp2_int (-num_extra_bits);
  // if all the bits in the new number is zero as well, multiply the result by 2^-num_uint_bits
  static constexpr auto urbg_inverse_range = canonical_float_random_detail::exp2_int (-num_uint_bits);
  // the distance between two consecutive floating-point numbers in [0.5, 1) is half epsilon
  static constexpr float_t half_epsilon = std::numeric_limits<float_t>::epsilon()/2;
  
  // too big cache doesn't fit on the stack
  static constexpr uint_fast8_t num_cache_bits = std::min (cache_max_bits, num_extra_bits);
  // if all the bits that will be looked up in the cache table are zero (but the rest is not),
  // multiply the result by 2^-num_cache_bits
  static constexpr auto exp2_num_cache_bits = canonical_float_random_detail::exp2_int (-num_cache_bits);
  static constexpr size_t cache_size = 1u << num_cache_bits;
  static constexpr uint_t cache_mask = cache_size - 1u;
  float_t data[cache_size];
  
public:
  explicit constexpr canonical_float_random () noexcept : data{} {
    // we precompute and store 2^-g values, where g is the geometric variate corresponding to
    // num_cache_bits least significant bits of r
    // when r = 1, 3, 5, 7, 9, ... then 2^-g = 1.
    // when r = 2, 6, 10, 14, 18, ... then 2^-g = 0.5
    // when r = 4, 12, 20, 28, 36, ... then 2^-g = 0.25
    // when r = 8, 24, 40, 56, 72, ... then 2^-g = 0.125
    // and so on
    for (auto half_stride=1u; half_stride<cache_size; half_stride*=2)
      for (auto r=half_stride; r<cache_size; r+=2*half_stride) data[r]=1./half_stride;
  }
  
  float_t operator() (URBG &gen) const {
    float_t number = 0.5, multiplier = 0.5*urbg_inverse_range;
    for (auto i=1u; i<min_urbg_calls_needed; ++i, multiplier *= urbg_inverse_range)
      number += gen()*multiplier;
    uint_t r = gen();
    number += (r >> num_extra_bits)*half_epsilon;   // generate the random fraction
    // if the first non-zero bit is one of the cache bits, use cache
    if (r & cache_mask) return number*data[r & cache_mask];
    else {
      r &= exponent_mask;     // make sure the fraction part of r is masked out
      r >>= num_cache_bits;   // throw away the cache bits since they're zero
      // if the rest of r has non-zero bit, add num_cache_bits (that was thrown away) to g,
      // or multiply the result by 2^-num_cache_bits
      if (r) number *= exp2_num_cache_bits;
      else {
        // all of the extra bits have been zero, so new number should be generated and
        // num_extra_bits should be added to g, or the result should be multiplied by 2^-num_extra_bits
        number *= exp2_num_extra_bits;
        r = gen();
        while (!r) {
          // new numbers have num_uint_bits bits so the result should be multiplied by 2^-num_uint_bits
          // which is equal to urbg_inverse_range
          number *= urbg_inverse_range;
          r = gen();
        }
      }
      // find the position of the first non-zero bit, p, and multiply the result by 2^-p
      while (!(r % 2)) {
        number *= .5;
        r >>= 1;
      }
    }
    return number;
  }
};

#endif
#endif
