#pragma once
#include <cstdio>
#include "i2cbus.hpp"

#define CAPSENSE_I2C_ADDRESS 0x37

#define BTN_LONG_PRESS_MS 2000
#define BTN_SHORT_PRESS_MS 100
#define BTN_POLL_MS 10
#define BTN_LONG_PRESS_TICKS (BTN_LONG_PRESS_MS / BTN_POLL_MS)
#define BTN_SHORT_PRESS_TICKS (BTN_SHORT_PRESS_MS / BTN_POLL_MS)

/* Generated: 02.03.2021 14:37:00 +02:00 */
const unsigned char CY8CMBR3102_SX1I_configuration[128] = {
  0x01u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
  0x03u, 0x00u, 0x00u, 0x00u, 0x7Fu, 0x7Fu, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x03u, 0x00u, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x80u,
  0x05u, 0x00u, 0x00u, 0x02u, 0x00u, 0x02u, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Eu, 0x00u, 0x00u,
  0x00u, 0x1Eu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
  0x00u, 0xFFu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x02u, 0x03u, 0x01u, 0x59u,
  0x00u, 0x37u, 0x01u, 0x00u, 0x00u, 0x0Au, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x01u, 0xD9u
};

#define CY8CMBR3xxx_CONFIG_DATA_LENGTH            (126)
#define CY8CMBR3xxx_CRC_BIT_WIDTH                 (sizeof(uint16_t) * 8)
#define CY8CMBR3xxx_CRC_BIT4_MASK                 (0x0F)
#define CY8CMBR3xxx_CRC_BIT4_SHIFT                (0x04)
#define CY8CMBR3xxx_CCITT16_DEFAULT_SEED          ((uint16_t) 0xffff)
#define CY8CMBR3xxx_CCITT16_POLYNOM               ((uint16_t) 0x1021)

static uint16_t CY8CMBR3xxx_Calc4BitsCRC(uint8_t value, uint16_t remainder) {
  uint8_t tableIndex;

  /* Divide the value by polynomial, via the CRC polynomial */
  tableIndex = (value & CY8CMBR3xxx_CRC_BIT4_MASK) ^
               ((remainder) >> (CY8CMBR3xxx_CRC_BIT_WIDTH - CY8CMBR3xxx_CRC_BIT4_SHIFT));
  remainder = (CY8CMBR3xxx_CCITT16_POLYNOM * tableIndex) ^ (remainder << CY8CMBR3xxx_CRC_BIT4_SHIFT);
  return remainder;
}

static uint16_t CY8CMBR3xxx_CalculateCrc(uint8_t *configuration) {
  uint32_t messageIndex;
  uint8_t byteValue;
  uint16_t seed = CY8CMBR3xxx_CCITT16_DEFAULT_SEED;

  /* don't make count down cycle! CRC will be different! */
  for(messageIndex = 0; messageIndex < CY8CMBR3xxx_CONFIG_DATA_LENGTH; messageIndex++)
  {
    byteValue = configuration[messageIndex];
    seed = CY8CMBR3xxx_Calc4BitsCRC(byteValue >> CY8CMBR3xxx_CRC_BIT4_SHIFT, seed);
    seed = CY8CMBR3xxx_Calc4BitsCRC(byteValue, seed);
  }
  return seed;
}

static bool verify_flash(uint8_t *config) {
  uint8_t val[0x7E]{}; // Config without CRC fields
  I2C_Read_Reg(CAPSENSE_I2C_ADDRESS, 0x00, val);
  for (auto i = 0u; i < sizeof(val); i++) {
    if(config[i] != val[i]) {
      return false;
    }
  }
  return true;
}
