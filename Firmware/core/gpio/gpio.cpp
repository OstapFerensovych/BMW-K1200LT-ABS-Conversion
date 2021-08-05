#include "platform.hpp"

#include "gpio.hpp"

namespace gpio {

#pragma GCC push_options
#pragma GCC optimize("-Os")

__attribute__((flatten)) void init() {
  uint32_t rcc_mask = 0;
  priv::static_for<pins_config_size>([&](auto i) {
    constexpr auto port = pins_config[i].second.port;
    rcc_mask |= priv::get_rcc_mask<port>();
  });

  RCC->AHBENR |= rcc_mask;

  priv::static_for<static_cast<size_t>(port::_max_ports)>([&](auto current_port_idx) {
    constexpr auto current_port = static_cast<enum port>(current_port_idx());
    uint32_t moder = 0;
    uint32_t otyper = 0;
    uint32_t ospeedr = 0;
    uint32_t pupdr = 0;
    uint32_t afrl = 0;
    uint32_t afrh = 0;
    uint32_t odr = 0;

    if (current_port == port::a) {
      moder = 0x28000000;
      ospeedr = 0x0C000000;
      pupdr = 0x24000000;
    }

    priv::static_for<pins_config_size>([&](auto i) {
      constexpr auto pin_cfg = pins_config[i].second;
      if constexpr (pin_cfg.port != current_port) return;

      static_assert(priv::check_config(pin_cfg), "Pin configuration invalid!");

      constexpr auto pin_idx = pin_cfg.index;
      moder |= static_cast<uint32_t>(pin_cfg.mode) << (2 * pin_idx);
      if (pin_cfg.drive != drive::input) otyper |= static_cast<uint32_t>(pin_cfg.drive) << pin_idx;
      if (pin_cfg.speed != speed::input) ospeedr |= static_cast<uint32_t>(pin_cfg.speed) << (2 * pin_idx);
      pupdr |= static_cast<uint32_t>(pin_cfg.pull) << (2 * pin_idx);
      odr |= pin_cfg.value << pin_idx;

      if constexpr (pin_cfg.alt_func != alt_func::none) {
        if constexpr (pin_idx < 8)
          afrl |= static_cast<uint32_t>(pin_cfg.alt_func) << (4 * pin_idx);
        else
          afrh |= static_cast<uint32_t>(pin_cfg.alt_func) << (4 * (pin_idx - 8));
      }
    });

    GPIO_TypeDef *p = priv::get_gpio_ptr<current_port>();
    if (odr) p->ODR = odr;
    if (moder) p->MODER = moder;
    if (otyper) p->OTYPER = otyper;
    if (ospeedr) p->OSPEEDR = ospeedr;
    if (pupdr) p->PUPDR = pupdr;
    if (afrl) p->AFR[0] = afrl;
    if (afrh) p->AFR[1] = afrh;
  });
}

#pragma GCC pop_options

}  // namespace gpio
