#include "i2cbus.hpp"

#include <stm32f0xx.h>

#include <cstdio>
#include <optional>
#include <type_traits>

#include "CY8CMBR3xxx_Registers.h"
#include "timing.hpp"

typedef enum {
  i2c_dir_read,
  i2c_dir_write,
} i2c_direction_e;

const char *err_str;

static bool i2c_has_error() {
  bool has_error = I2C1->ISR & (I2C_ISR_TIMEOUT | I2C_ISR_ARLO | I2C_ISR_BERR | I2C_ISR_NACKF);
  if (has_error) {
    I2C1->CR1 = 0;
    I2C1->CR1 = I2C_CR1_PE;
  }

  return has_error;
}

static bool i2c_start(uint32_t s_addr, i2c_direction_e direction, uint32_t datasize) {
  err_str = __func__;
  I2C1->CR2 = (s_addr << 1) | (datasize << I2C_CR2_NBYTES_Pos);

  if (direction == i2c_dir_read) I2C1->CR2 |= I2C_CR2_RD_WRN;

  I2C1->CR2 |= I2C_CR2_START;

  bool err = false;
  while ((I2C1->CR2 & I2C_CR2_START) && !(err = i2c_has_error())) continue;

  return !err;
}

static bool i2c_stop() {
  err_str = __func__;
  bool err = false;
  while ((I2C1->ISR & I2C_ISR_TC) == 0 && !(err = i2c_has_error())) continue;
  if (err) return false;

  I2C1->CR2 |= I2C_CR2_STOP;
  while ((I2C1->CR2 & I2C_CR2_STOP) && !(err = i2c_has_error())) continue;
  if (err) return false;

  while ((I2C1->CR2 & I2C_ISR_BUSY) && !(err = i2c_has_error())) continue;
  if (err) return false;

  return true;
}

static bool i2c_write8(std::byte byte) {
  err_str = __func__;
  bool err = false;

  while ((I2C1->ISR & I2C_ISR_TXE) == 0 && !(err = i2c_has_error())) continue;
  if (err) return false;

  I2C1->TXDR = (uint8_t)byte;
  return true;
}

static std::optional<std::byte> i2c_read8() {
  err_str = __func__;
  bool err = false;

  while ((I2C1->ISR & I2C_ISR_RXNE) == 0 && !(err = i2c_has_error())) continue;
  if (err) return {};

  return (std::byte)I2C1->RXDR;
}

template <typename T>
bool read_reg(uint8_t addr, uint8_t reg, T &out_val) {
  size_t retry_cnt = 32;
  while (!i2c_start(addr, i2c_dir_write, 1) && retry_cnt--) continue;
  if (!retry_cnt || !i2c_write8((std::byte)reg) || !i2c_stop()) return false;

  while (!i2c_start(addr, i2c_dir_read, sizeof(T)) && retry_cnt--) continue;
  if (!retry_cnt) return false;

  std::byte *p_data = reinterpret_cast<std::byte *>(out_val);
  for (auto i = 0u; i < sizeof(T); i++) {
    auto val = i2c_read8();
    if (!val) return false;
    *p_data++ = val.value();
  }

  if (!i2c_stop()) return false;
  return true;
}

template <typename T>
bool write_reg(uint8_t addr, uint8_t reg, T &value) {
  size_t retry_cnt = 32;
  while (!i2c_start(addr, i2c_dir_write, 1) && retry_cnt--) continue;
  if (!retry_cnt || !i2c_write8((std::byte)reg)) return false;

  while (!i2c_start(addr, i2c_dir_write, sizeof(T) + 1) && retry_cnt--) continue;
  if (!retry_cnt || !i2c_write8((std::byte)reg)) return false;

  std::byte *p_data = reinterpret_cast<std::byte *>(value);
  for (auto i = 0u; i < sizeof(T); i++) {
    auto val = i2c_write8(*p_data++);
    if (!val) return false;
    //    *value >>= 8;
  }

  if (!i2c_stop()) return false;
  return true;
}

bool I2C_Write_Reg(uint8_t addr, uint8_t reg, uint8_t *value) {
  return write_reg(addr, reg, value);
}

bool I2C_Read_Reg(uint8_t addr, uint8_t reg, uint8_t *value) {
  return read_reg(addr, reg, value);
}

void I2C_Init() {
  /* Reset the I2C master */
  RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
  RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;

  /* Enable I2C1 Clock */
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

  /* Use SysClk as I2C Clock */
  RCC->CFGR3 |= RCC_CFGR3_I2C1SW_SYSCLK;

  /* Set I2C Timings (100kHz Standard mode, Fi2cclk = 8 MHz) */
  I2C1->TIMINGR = (0x05 << I2C_TIMINGR_PRESC_Pos) | (0x09 << I2C_TIMINGR_SCLL_Pos) | (0x05 << I2C_TIMINGR_SCLH_Pos) |
                  (0x03 << I2C_TIMINGR_SDADEL_Pos) | (0x03 << I2C_TIMINGR_SCLDEL_Pos);

  /* Enable peripherial */
  I2C1->CR1 = I2C_CR1_PE;
}
