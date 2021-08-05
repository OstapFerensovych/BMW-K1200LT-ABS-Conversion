#include "errors.hpp"
#include "gpio.hpp"
#include "rcc.h"
#include "stm32f0xx.h"
#include "timing.hpp"
#include "can.hpp"

[[noreturn]] int main();
[[noreturn]] int main() {
  rcc_init();
  errors_init();
  timing_init();
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGCOMPEN);
  READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGCOMPEN);
  SET_BIT(SYSCFG->CFGR1, SYSCFG_CFGR1_PA11_PA12_RMP);
  gpio::init();
  timing_sleep_ms(100);

  CAN_Init();

  for (;;){
    gpio::set<gpio::line::speedo>();
    timing_sleep_ms(10);
    gpio::clear<gpio::line::speedo>();
    timing_sleep_ms(10);
//    continue;
  }
}
