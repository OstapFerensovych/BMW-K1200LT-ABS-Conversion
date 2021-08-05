#pragma once
#include <cstdio>

enum class ButtonStatus {
  kNotPressed = 0, ///< Used for when the button is not pressed
  kPressing, ///< Used for when the button is being pressed and held down
  kShortPress, ///< Used for when the button is just released after a short press
  kLongPress, ///< Used for when the button is just released after a long press
  kLongPressTriggered, ///< Used for when the button is pressed for longer than the threshold and is still pressed
  kSize ///< Max size of the enum
};

void capsense_init();
bool capsense_read(uint8_t cs_num);
bool capsense_last();
