#include "led.hpp"
#include "gpio.hpp"
#include "can.hpp"
#include <array>

//static constexpr std::array led_on = {
//  gpio::set<gpio::line::led_green>,
//  gpio::set<gpio::line::led_blue>,
//};

//static constexpr std::array led_off = {
//  gpio::clear<gpio::line::led_green>,
//  gpio::clear<gpio::line::led_blue>,
//};

void LED_init(){

}

void LED_on(Color color) {
//  led_on[static_cast<uint8_t>(color)]();
}

void LED_off(Color color) {
//  led_off[static_cast<uint8_t>(color)]();
}

void LED_blink(Color color, uint8_t period) {
  //TODO Blinking...
  (void)color;
  (void)period;
}
