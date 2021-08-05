#pragma once

#include <stm32f0xx.h>

#include <cstddef>
#include <cstdint>
#include <type_traits>

#define flat __attribute__((flatten)) constexpr
#define flat_inline __attribute__((flatten, always_inline)) constexpr

#ifdef __clang__
#define consteval constexpr
#endif

namespace util {
#pragma GCC optimize("Os")
extern "C" void delay_cycles(uint32_t cycles);

template <size_t first, size_t last, typename lambda>
flat_inline void static_for(lambda const &func) {
  if constexpr (first <= last) {
    func(std::integral_constant<size_t, first>{});
    static_for<first + 1, last>(func);
  }
}

template <size_t count, typename lambda>
flat_inline void static_for(lambda const &func) {
  static_for<0, count - 1>(func);
}
#pragma GCC reset_options
}  // namespace util
