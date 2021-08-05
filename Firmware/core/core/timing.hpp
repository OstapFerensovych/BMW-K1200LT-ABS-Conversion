#ifndef TIMING_H
#define TIMING_H

#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstring>

typedef struct {
  uint32_t start_tick;
  uint32_t timeout;
} timeout_t;

extern volatile uint32_t g_TickCount;

void timing_init();
void timing_sleep_ms(uint32_t delay);
void timing_sleep_us(uint32_t us);
void timing_timeout_init(timeout_t *to_s, uint32_t timeout_ms);

inline __attribute__((always_inline)) bool timeout_expired(timeout_t *to_s) {
  return (g_TickCount - to_s->start_tick) >= to_s->timeout;
}

#endif  // TIMING_H
