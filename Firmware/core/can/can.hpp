#pragma once
#include <stdio.h>
#include <cstdint>

enum CanCmd_e {
  ABS_MSG_ABS_STATUS_1 = 0,
  ABS_MSG_ABS_STATUS_2 = 2,
};

#define CAN_ID_MASK  ((1U << 9) | 0x1FF)

int CAN_Init();
bool CAN_front_lever();
bool CAN_rear_lever();
bool CAN_front_sensor_ok();
bool CAN_rear_sensor_ok();
bool CAN_abs_on();
float CAN_SpeedFrontWheel();
float CAN_SpeedRearWheel();
