#include "timing.hpp"

#include <stm32f0xx.h>

#include "errors.hpp"

volatile uint32_t g_TickCount;

/* Prevent Missing Declaration warnings */
extern "C" void SysTick_Handler();
extern "C" void SysTick_Handler() {
  g_TickCount++;
}

void timing_init() {
  g_TickCount = 0;
  SysTick_Config(SystemCoreClock / 1000);
}

void timing_sleep_us(uint32_t us) {
  volatile uint32_t i;
  for (i = ((SystemCoreClock / 48000000) * us); i != 0; i--) {}
}

void timing_sleep_ms(uint32_t delay) {
  uint32_t now = g_TickCount;

  while ((g_TickCount - now) < delay)
    continue;
}

void timing_timeout_init(timeout_t *to_s, uint32_t timeout_ms) {
  __disable_irq();
  to_s->start_tick = g_TickCount;
  to_s->timeout = timeout_ms;
  __enable_irq();
}
