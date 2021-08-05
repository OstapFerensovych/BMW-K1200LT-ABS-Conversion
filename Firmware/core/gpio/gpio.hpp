#pragma once

#include <stm32f0xx.h>

#include <cstdint>
#include <tuple>

#include "platform.hpp"

namespace gpio {
enum class port {
  a = 0,
  b = 1,
  c = 2,
  f = 3,
  _max_ports,
};

enum class mode : uint8_t {
  input  = 0,
  gp_out = 1,
  af_io  = 2,
  analog = 3,
};

enum class drive : uint8_t {
  push_pull  = 0,
  open_drain = 1,
  input      = UINT8_MAX,
};

enum class speed : uint8_t {
  low    = 0,
  medium = 1,
  high   = 3,
  input  = UINT8_MAX,
};

enum class pull : uint8_t {
  none = 0,
  up   = 1,
  down = 2,
};

enum class alt_func : uint8_t {
  af0  = 0,
  af1  = 1,
  af2  = 2,
  af3  = 3,
  af4  = 4,
  af5  = 5,
  af6  = 6,
  af7  = 7,
  none = UINT8_MAX,
};

using pin_config = struct {
  enum port port;
  uint8_t index;
  uint8_t value;
  enum mode mode;
  enum drive drive;
  enum speed speed;
  enum pull pull;
  enum alt_func alt_func;
};

#include "gpio_conf.hpp"

constexpr size_t pins_config_size = sizeof(pins_config) / sizeof(pins_config[0]);
// clang-format on

namespace priv {
flat bool check_config(const pin_config &p) {
  /* Alternate function enabled and pin is not af_out or input */
  if (p.alt_func != alt_func::none && (p.mode == mode::analog || p.mode == mode::gp_out)) return false;

  /* PullUp of PullDown on strong driven output pin */
  if (p.pull != pull::none && p.drive != drive::open_drain && (p.mode == mode::gp_out || p.mode == mode::af_io))
    return false;

  /* OpenDrain or PushPull on input pin */
  if ((p.mode == mode::analog || p.mode == mode::input) && p.drive != drive::input) return false;

  /* PullUp of PullDown on analog input pin */
  if (p.mode == mode::analog && p.pull != pull::none) return false;

  return true;
}

template <port p>
flat_inline GPIO_TypeDef *get_gpio_ptr() {
  if constexpr (p == port::a) return GPIOA;
  if constexpr (p == port::b) return GPIOB;
  if constexpr (p == port::c) return GPIOC;
  if constexpr (p == port::f) return GPIOF;
}

template <port p>
flat_inline uint32_t get_gpio_offset() {
  if constexpr (p == port::a) return GPIOA_BASE - GPIOA_BASE;
  if constexpr (p == port::b) return GPIOB_BASE - GPIOA_BASE;
  if constexpr (p == port::c) return GPIOC_BASE - GPIOA_BASE;
  if constexpr (p == port::f) return GPIOF_BASE - GPIOA_BASE;
}

template <port p>
flat_inline uint32_t get_rcc_mask() {
  if constexpr (p == port::a) return RCC_AHBENR_GPIOAEN;
  if constexpr (p == port::b) return RCC_AHBENR_GPIOBEN;
  if constexpr (p == port::c) return RCC_AHBENR_GPIOCEN;
  if constexpr (p == port::f) return RCC_AHBENR_GPIOFEN;
}

template <size_t count, typename Lambda>
flat_inline void static_for(Lambda const &f) {
  if constexpr (count > 0) {
    f(std::integral_constant<size_t, count - 1>{});
    static_for<count - 1>(f);
  }
}

template <line line, size_t i = pins_config_size - 1, typename Lambda>
flat_inline bool foreach_line(Lambda const &f) {
  static_assert(pins_config_size > 0, "No lines defined???");

  if constexpr (line != pins_config[i].first) {
    static_assert(i > 0, "line not found");
    if constexpr (i > 0) return foreach_line<line, i - 1, Lambda>(f);
  }

  constexpr auto index = pins_config[i].second.index;
  auto port            = get_gpio_ptr<pins_config[i].second.port>();
  return f(port, index);
}

template <line... lines>
flat_inline bool check_line_group() {
  constexpr line sig[] = {lines...};
  port p               = port::_max_ports;

  for (auto &s : sig) {
    port new_p = port::_max_ports;
    for (auto &pc : pins_config)
      if (pc.first == s) new_p = pc.second.port;

    if (new_p == port::_max_ports) return false;
    if (p == port::_max_ports) p = new_p;
    if (p != new_p) return false;
  }

  return true;
}

template <bool set, line... s>
flat_inline void set_clear_group() {
  uint32_t set_mask = 0;
  GPIO_TypeDef *p   = nullptr;

  constexpr bool config_ok = check_line_group<s...>();
  static_assert(config_ok, "Invalid line group");
  if (!config_ok) return;

  static_for<sizeof...(s)>([&](auto j) {
    constexpr line lines[]   = {s...};
    constexpr line crnt_line = lines[j];
    static_for<pins_config_size>([&](auto i) {
      if constexpr (pins_config[i].first != crnt_line) return;
      p = get_gpio_ptr<pins_config[i].second.port>();
      set_mask |= 1u << pins_config[i].second.index;
    });
  });

  p->BSRR = set ? set_mask : set_mask << 16u;
}
}  // namespace priv

template <line... s>
flat void set_group() {
  priv::set_clear_group<true, s...>();
}

template <line... s>
flat void clear_group() {
  priv::set_clear_group<false, s...>();
}

template <line line>
flat_inline void set() {
  priv::foreach_line<line>([&](GPIO_TypeDef *port, uint8_t index) { return port->BSRR = 1u << index; });
}

template <line line>
flat_inline void clear() {
  priv::foreach_line<line>([&](GPIO_TypeDef *port, uint8_t index) { return port->BRR = 1u << index; });
}

template <line line>
flat_inline bool get() {
  return priv::foreach_line<line>([&](GPIO_TypeDef *port, uint8_t index) { return port->IDR & (1u << index); });
}

using pin_group = std::pair<uint16_t, uint16_t>;

static inline void set(pin_group g) {
  auto ptr  = (GPIO_TypeDef *)(GPIOA_BASE + g.first);
  ptr->BSRR = g.second;
}

static inline void clear(pin_group g) {
  auto ptr = (GPIO_TypeDef *)(GPIOA_BASE + g.first);
  ptr->BRR = g.second;
}

template <line... s>
flat pin_group mask() {
  uint16_t set_mask = 0;
  uint32_t addr     = 0;

  constexpr bool config_ok = priv::check_line_group<s...>();
  static_assert(config_ok, "Invalid line group");
  if (!config_ok) return {};

  priv::static_for<sizeof...(s)>([&](auto j) {
    constexpr line lines[]   = {s...};
    constexpr line crnt_line = lines[j];
    priv::static_for<pins_config_size>([&](auto i) {
      if constexpr (pins_config[i].first != crnt_line) return;
      addr = priv::get_gpio_offset<pins_config[i].second.port>();
      set_mask |= 1u << pins_config[i].second.index;
    });
  });

  return {addr, set_mask};
}

void init();

}  // namespace gpio
