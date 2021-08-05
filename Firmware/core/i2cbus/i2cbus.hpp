#pragma once
#include <cstdio>

void I2C_Init();
bool I2C_Read_Reg(uint8_t addr, uint8_t reg, uint8_t *value);
bool I2C_Write_Reg(uint8_t addr, uint8_t reg, uint8_t *value);
