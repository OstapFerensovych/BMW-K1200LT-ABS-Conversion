#pragma once
#include <stdio.h>
#include "capsense.hpp"
/*
 *  CAN Identification
 *  0-4 bits Command ID. Max. 0x1F
 *  5-8 bits Device ID. Max. 0x0F
 *  9-10 bits Device enumerator. Max 0x03
*/

enum CanCmd_e {
  CS_MSG_SET_CAPSENSE_LED_STATUS,
  CS_MSG_SET_CONFIG,
  CS_HEARTBEAT,
  CS_MSG_EVT_BUTTON,
};

enum side_e {
  SIDE_LEFT,
  SIDE_RIGHT,
};

const uint8_t SIDE = SIDE_RIGHT;

#define CAN_ID_MASK  ((1U << 9) | 0x1FF)

#define CAN_DEVID_SENSCAP 0x2

#define CAN_CMD_Pos      0
#define CAN_CMD_Msk      0x1F
#define CAN_DEVID_Pos    5
#define CAN_DEVID_Msk    0x1E0
#define CAN_DEVENUM_Pos  9
#define CAN_DEVENUM_Msk  0x600

#define CANID_MAKE(dev_enum, dev_id, cmd) \
  ((dev_enum << CAN_DEVENUM_Pos) | (dev_id << CAN_DEVID_Pos) | (cmd << CAN_CMD_Pos))
#define SENSCAP_MAKE_CANID(cmd) CANID_MAKE(SIDE, CAN_DEVID_SENSCAP, cmd)

int CAN_Init();
void CAN_send_button_state(ButtonStatus status);
void CAN_hearbeat(ButtonStatus status);
