#pragma once
#include <cstdio>

enum class Color {
  Green = 0,
  Blue,
  Size
};

enum class Status {
  Off = 0,
  On,
  Blink,
  Size
};

void LED_init();
void LED_on(Color color);
void LED_off(Color color);
void LED_blink(Color color, uint8_t period);
