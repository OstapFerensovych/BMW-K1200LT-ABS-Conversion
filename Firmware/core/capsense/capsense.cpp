#include "capsense.hpp"
#include "capsense_helper.h"
#include "platform.hpp"
#include <cstring>
#include "i2cbus.hpp"
#include "CY8CMBR3xxx_Registers.h"
#include "timing.hpp"
#include "can.hpp"

//uint8_t val[128]{};
//uint8_t err[1]{};
uint8_t count = 0;

struct capsense {
  ButtonStatus tp_state = ButtonStatus::kNotPressed;
  bool touch = false;
  bool released = true;
  uint32_t tp_prs_tick = 0;
};

capsense cs;

static bool verify_flash(uint8_t *config);

static void timer_init() {
  SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);
  TIM2->PSC |= 48000 - 1;
  TIM2->CR2 |= TIM_CR1_ARPE;
  TIM2->DIER |= TIM_DIER_CC1IE;
  TIM2->CCMR1 |= TIM_CCMR1_OC1CE;
  TIM2->ARR |= 1000000 - 1;
  TIM2->CCR1 = 470000 - 1; // ~10ms interrupt
  TIM2->CR1 |= TIM_CR1_CEN;
  NVIC_EnableIRQ(TIM2_IRQn);
  NVIC_SetPriority(TIM2_IRQn, 0);
}

void capsense_init() {
  uint8_t config[128];
  memcpy(&config, &CY8CMBR3102_SX1I_configuration, sizeof(CY8CMBR3102_SX1I_configuration));
  if(!verify_flash(config)) {
    /* printf("Flash needs to be updated\n"); */

    /* Recalculate CRC */
    unsigned int crc = CY8CMBR3xxx_CalculateCrc(config);
    config[0x7F] = (uint8_t)(crc >> 8);
    config[0x7E] = (uint8_t)(crc & 0xFF);

    /* Write config to CapSense */
    I2C_Write_Reg(CAPSENSE_I2C_ADDRESS, 0x00, config);
    uint8_t data[1] = {CY8CMBR3xxx_CMD_SAVE_CHECK_CRC};
    I2C_Write_Reg(CAPSENSE_I2C_ADDRESS, CY8CMBR3xxx_CTRL_CMD, data);
    timing_sleep_ms(300);

    data[0] = {CY8CMBR3xxx_CMD_SOFTWARE_RESET};
    I2C_Write_Reg(CAPSENSE_I2C_ADDRESS, CY8CMBR3xxx_CTRL_CMD, data);
    timing_sleep_ms(100);
  } /*else printf("Flash is up to date\n");*/
  timer_init();
}

bool capsense_read(uint8_t cs_num) {
  uint8_t data[1]{};
  I2C_Read_Reg(CAPSENSE_I2C_ADDRESS, CY8CMBR3xxx_BUTTON_STAT, data);
  return (data[0] >> cs_num) & 0x1;
}

extern "C" void TIM2_IRQHandler();
extern "C" void TIM2_IRQHandler() {
  __disable_irq();
  cs.touch = capsense_read(0);

  if(!cs.touch) {
    cs.released = true;
  }

  if(cs.touch && cs.released) {
    cs.tp_prs_tick++;
    if(cs.tp_prs_tick > BTN_LONG_PRESS_TICKS) {
//      printf("Long Press\r\n");
      cs.tp_state = ButtonStatus::kLongPressTriggered;
      CAN_send_button_state(cs.tp_state);
      cs.tp_prs_tick = 0;
      cs.released = false;

    }
  } else if ((cs.tp_prs_tick > BTN_SHORT_PRESS_TICKS)) {
//    printf("Short press\r\n");
    cs.tp_state = ButtonStatus::kShortPress;
    CAN_send_button_state(cs.tp_state);
    cs.released = false;
    cs.tp_prs_tick = 0;
  } else {
    cs.tp_prs_tick = 0;
  }

  __enable_irq();
  TIM2->SR &= ~TIM_SR_CC1IF;
  TIM2->CNT = 0;

  count++;
  if(count >= 5) {
    count = 0;
    if(cs.touch) {
      CAN_hearbeat(ButtonStatus::kPressing);
    } else {
      CAN_hearbeat(ButtonStatus::kNotPressed);
    }
  }
}
