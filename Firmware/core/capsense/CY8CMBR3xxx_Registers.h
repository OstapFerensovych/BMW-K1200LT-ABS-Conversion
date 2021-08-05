/*****************************************************************************
* File Name: CY8CMBR3xxx_Registers.h
*
* Version 1.00
*
* Description:
*   This file contains the macros to define the offset addresses for all the  
*   registers of CY8CMBR3xxx family. To access a register via the APIs, you should
*   use these macros in place of numerals.
*
* Note:
*   N/A
*
* Owner:
*   SRVS
*
* Related Document:
*   MBR3 Device Datasheet
*   MBR3 Device Register TRM
*
* Hardware Dependency:
*   N/A
*
* Code Tested With:
*   PSoC Creator 3.0 CP7
*   CY3280-MBR3 Evaluation Kit
*   CY8CKIT-042 Pioneer Kit
*
******************************************************************************
* Copyright (2014), Cypress Semiconductor Corporation.
******************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and
* foreign), United States copyright laws and international treaty provisions.
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the
* Cypress Source Code and derivative works for the sole purpose of creating
* custom software in support of licensee product to be used only in conjunction
* with a Cypress integrated circuit as specified in the applicable agreement.
* Any reproduction, modification, translation, compilation, or representation of
* this software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the
* materials described herein. Cypress does not assume any liability arising out
* of the application or use of any product or circuit described herein. Cypress
* does not authorize its products for use as critical components in life-support
* systems where a malfunction or failure may reasonably be expected to result in
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of
* such use and in doing so indemnifies Cypress against all charges. Use may be
* limited by and subject to the applicable Cypress software license agreement.
*****************************************************************************/

#if !defined(CY8CMBR3xxx_REGISTERS_H)
#define CY8CMBR3xxx_REGISTERS_H

#define CY8CMBR3xxx_CY8CMBR3102                (1 << 5)
#define CY8CMBR3xxx_DEVICE (CY8CMBR3xxx_CY8CMBR3102)

/*******************************************************************************
* Supported Commands
*******************************************************************************/
// There is no command currently executing.
// The device writes this value at startup and upon completion of any command.
#define CY8CMBR3xxx_CMD_NO_COMMAND                             (0)

// The device calculates a CRC checksum over the configuration data in this register map and compares the result with the content of CONFIG_CRC.
// If the two values match, the device saves the configuration and the CRC checksum to nonvolatile memory.
#define CY8CMBR3xxx_CMD_SAVE_CHECK_CRC                         (2)

// The device discontinues scanning and enters the low power mode.
// The device will exit this mode upon an I2C address match event.
#define CY8CMBR3xxx_CMD_SLEEP                                  (7)

// The device sets the contents of LATCHED_BUTTON_STAT and LATCHED_PROX_STAT to 0 and
// sets the contents of LIFTOFF_SLIDER1_POSITION and LIFTOFF_SLIDER2_POSITION to 0xFF.
#define CY8CMBR3xxx_CMD_CLEAR_LATCHED_STATUS                   (8)

#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3106S))
// The device resets the Advanced Low Pass filter for proximity sensor PS0
#define CY8CMBR3xxx_CMD_RESET_PROX0_FILTER                     (9)

// The device resets the Advanced Low Pass filter for proximity sensor PS1
#define CY8CMBR3xxx_CMD_RESET_PROX1_FILTER                     (10)
#endif

// The device resets itself
#define CY8CMBR3xxx_CMD_SOFTWARE_RESET                         (255)

/*******************************************************************************
* Device Registers
*******************************************************************************/

#define CY8CMBR3xxx_SENSOR_EN                                                 (0x00)
#define CY8CMBR3xxx_FSS_EN                                                    (0x02)
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3106S))
#define CY8CMBR3xxx_TOGGLE_EN                                                 (0x04) // Except CY8CMBR3106S
#define CY8CMBR3xxx_LED_ON_EN                                                 (0x06) // Except CY8CMBR3106S
#endif
#define CY8CMBR3xxx_SENSITIVITY0                                              (0x08)
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3102))
#define CY8CMBR3xxx_SENSITIVITY1                                              (0x09) // Except CY8CMBR3102
#endif
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3102 | CY8CMBR3xxx_CY8CMBR3108))
#define CY8CMBR3xxx_SENSITIVITY2                                              (0x0A) // Except CY8CMBR3102 | CY8CMBR3108
#endif
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3102 | CY8CMBR3xxx_CY8CMBR3108 | CY8CMBR3xxx_CY8CMBR3110))
#define CY8CMBR3xxx_SENSITIVITY3                                              (0x0B) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110
#endif
#define CY8CMBR3xxx_BASE_THRESHOLD0                                           (0x0C)
#define CY8CMBR3xxx_BASE_THRESHOLD1                                           (0x0D)
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3102))
#define CY8CMBR3xxx_FINGER_THRESHOLD2                                         (0x0E) // Except CY8CMBR3102
#define CY8CMBR3xxx_FINGER_THRESHOLD3                                         (0x0F) // Except CY8CMBR3102
#define CY8CMBR3xxx_FINGER_THRESHOLD4                                         (0x10) // Except CY8CMBR3102
#define CY8CMBR3xxx_FINGER_THRESHOLD5                                         (0x11) // Except CY8CMBR3102
#define CY8CMBR3xxx_FINGER_THRESHOLD6                                         (0x12) // Except CY8CMBR3102
#define CY8CMBR3xxx_FINGER_THRESHOLD7                                         (0x13) // Except CY8CMBR3102
#endif
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3102 | CY8CMBR3xxx_CY8CMBR3108))
#define CY8CMBR3xxx_FINGER_THRESHOLD8                                         (0x14) // Except CY8CMBR3102 | CY8CMBR3108
#define CY8CMBR3xxx_FINGER_THRESHOLD9                                         (0x15) // Except CY8CMBR3102 | CY8CMBR3108
#endif
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3102 | CY8CMBR3xxx_CY8CMBR3108 | CY8CMBR3xxx_CY8CMBR3110))
#define CY8CMBR3xxx_FINGER_THRESHOLD10                                        (0x16) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110
#define CY8CMBR3xxx_FINGER_THRESHOLD11                                        (0x17) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110
#define CY8CMBR3xxx_FINGER_THRESHOLD12                                        (0x18) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110
#define CY8CMBR3xxx_FINGER_THRESHOLD13                                        (0x19) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110
#define CY8CMBR3xxx_FINGER_THRESHOLD14                                        (0x1A) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110
#define CY8CMBR3xxx_FINGER_THRESHOLD15                                        (0x1B) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110
#endif
#define CY8CMBR3xxx_SENSOR_DEBOUNCE                                           (0x1C)
#define CY8CMBR3xxx_BUTTON_HYS                                                (0x1D)
#define CY8CMBR3xxx_BUTTON_LBR                                                (0x1F)
#define CY8CMBR3xxx_BUTTON_NNT                                                (0x20)
#define CY8CMBR3xxx_BUTTON_NT                                                 (0x21)
#define CY8CMBR3xxx_PROX_EN                                                   (0x26)
#define CY8CMBR3xxx_PROX_CFG                                                  (0x27)
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3106S))
#define CY8CMBR3xxx_PROX_CFG2                                                 (0x28) // Except CY8CMBR3106S
#endif
#define CY8CMBR3xxx_PROX_TOUCH_TH0                                            (0x2A)
#define CY8CMBR3xxx_PROX_TOUCH_TH1                                            (0x2C)
#define CY8CMBR3xxx_PROX_RESOLUTION0                                          (0x2E)
#define CY8CMBR3xxx_PROX_RESOLUTION1                                          (0x2F)
#define CY8CMBR3xxx_PROX_HYS                                                  (0x30)
#define CY8CMBR3xxx_PROX_LBR                                                  (0x32)
#define CY8CMBR3xxx_PROX_NNT                                                  (0x33)
#define CY8CMBR3xxx_PROX_NT                                                   (0x34)
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3106S))
#define CY8CMBR3xxx_PROX_POSITIVE_TH0                                         (0x35) // Except CY8CMBR3106S
#define CY8CMBR3xxx_PROX_POSITIVE_TH1                                         (0x36) // Except CY8CMBR3106S
#define CY8CMBR3xxx_PROX_NEGATIVE_TH0                                         (0x39) // Except CY8CMBR3106S
#define CY8CMBR3xxx_PROX_NEGATIVE_TH1                                         (0x3A) // Except CY8CMBR3106S
#define CY8CMBR3xxx_LED_ON_TIME                                               (0x3D) // Except CY8CMBR3106S
#endif
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3102))
#define CY8CMBR3xxx_BUZZER_CFG                                                (0x3E) // Except CY8CMBR3102
#define CY8CMBR3xxx_BUZZER_ON_TIME                                            (0x3F) // Except CY8CMBR3102
#endif
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3106S))
#define CY8CMBR3xxx_GPO_CFG                                                   (0x40) // Except CY8CMBR3106S
#define CY8CMBR3xxx_PWM_DUTYCYCLE_CFG0                                        (0x41) // Except CY8CMBR3106S
#endif
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3102 | CY8CMBR3xxx_CY8CMBR3106S))
#define CY8CMBR3xxx_PWM_DUTYCYCLE_CFG1                                        (0x42) // Except CY8CMBR3102 | CY8CMBR3106S
#define CY8CMBR3xxx_PWM_DUTYCYCLE_CFG2                                        (0x43) // Except CY8CMBR3102 | CY8CMBR3106S
#define CY8CMBR3xxx_PWM_DUTYCYCLE_CFG3                                        (0x44) // Except CY8CMBR3102 | CY8CMBR3106S
#endif
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3102 | CY8CMBR3xxx_CY8CMBR3106S | CY8CMBR3xxx_CY8CMBR3108))
#define CY8CMBR3xxx_PWM_DUTYCYCLE_CFG4                                        (0x45) // Except CY8CMBR3102 | CY8CMBR3106S | CY8CMBR3108
#endif
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3102 | CY8CMBR3xxx_CY8CMBR3106S | CY8CMBR3xxx_CY8CMBR3108 | CY8CMBR3xxx_CY8CMBR3110))
#define CY8CMBR3xxx_PWM_DUTYCYCLE_CFG5                                        (0x46) // Except CY8CMBR3102 | CY8CMBR3106S | CY8CMBR3108 | CY8CMBR3110
#define CY8CMBR3xxx_PWM_DUTYCYCLE_CFG6                                        (0x47) // Except CY8CMBR3102 | CY8CMBR3106S | CY8CMBR3108 | CY8CMBR3110
#define CY8CMBR3xxx_PWM_DUTYCYCLE_CFG7                                        (0x48) // Except CY8CMBR3102 | CY8CMBR3106S | CY8CMBR3108 | CY8CMBR3110
#endif
#define CY8CMBR3xxx_SPO_CFG                                                   (0x4C)
#define CY8CMBR3xxx_DEVICE_CFG0                                               (0x4D)
#define CY8CMBR3xxx_DEVICE_CFG1                                               (0x4E)
#define CY8CMBR3xxx_DEVICE_CFG2                                               (0x4F)
#define CY8CMBR3xxx_DEVICE_CFG3                                               (0x50)
#define CY8CMBR3xxx_I2C_ADDR                                                  (0x51)
#define CY8CMBR3xxx_REFRESH_CTRL                                              (0x52)
#define CY8CMBR3xxx_STATE_TIMEOUT                                             (0x55)
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3102 | CY8CMBR3xxx_CY8CMBR3108 | CY8CMBR3xxx_CY8CMBR3110 | CY8CMBR3xxx_CY8CMBR3116))
#define CY8CMBR3xxx_SLIDER_CFG                                                (0x5D) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110 | CY8CMBR3116
#define CY8CMBR3xxx_SLIDER1_CFG                                               (0x61) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110 | CY8CMBR3116
#define CY8CMBR3xxx_SLIDER1_RESOLUTION                                        (0x62) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110 | CY8CMBR3116
#define CY8CMBR3xxx_SLIDER1_THRESHOLD                                         (0x63) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110 | CY8CMBR3116
#define CY8CMBR3xxx_SLIDER2_CFG                                               (0x67) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110 | CY8CMBR3116
#define CY8CMBR3xxx_SLIDER2_RESOLUTION                                        (0x68) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110 | CY8CMBR3116
#define CY8CMBR3xxx_SLIDER2_THRESHOLD                                         (0x69) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110 | CY8CMBR3116
#define CY8CMBR3xxx_SLIDER_LBR                                                (0x71) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110 | CY8CMBR3116
#define CY8CMBR3xxx_SLIDER_NNT                                                (0x72) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110 | CY8CMBR3116
#define CY8CMBR3xxx_SLIDER_NT                                                 (0x73) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110 | CY8CMBR3116
#endif
#define CY8CMBR3xxx_CONFIG_CRC                                                (0x7E)
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3106S))
#define CY8CMBR3xxx_GPO_OUTPUT_STATE                                          (0x80) // Except CY8CMBR3106S
#endif
#define CY8CMBR3xxx_SENSOR_ID                                                 (0x82)
#define CY8CMBR3xxx_CTRL_CMD                                                  (0x86)
#define CY8CMBR3xxx_CTRL_CMD_STATUS                                           (0x88)
#define CY8CMBR3xxx_CTRL_CMD_ERR                                              (0x89)
#define CY8CMBR3xxx_SYSTEM_STATUS                                             (0x8A)
#define CY8CMBR3xxx_PREV_CTRL_CMD_CODE                                        (0x8C)
#define CY8CMBR3xxx_FAMILY_ID                                                 (0x8F)
#define CY8CMBR3xxx_DEVICE_ID                                                 (0x90)
#define CY8CMBR3xxx_DEVICE_REV                                                (0x92)
#define CY8CMBR3xxx_TOTAL_WORKING_SNS                                         (0x97)
#define CY8CMBR3xxx_SNS_CP_HIGH                                               (0x98)
#define CY8CMBR3xxx_SNS_VDD_SHORT                                             (0x9A)
#define CY8CMBR3xxx_SNS_GND_SHORT                                             (0x9C)
#define CY8CMBR3xxx_SNS_SNS_SHORT                                             (0x9E)
#define CY8CMBR3xxx_CMOD_SHIELD_TEST                                          (0xA0)
#define CY8CMBR3xxx_BUTTON_STAT                                               (0xAA)
#define CY8CMBR3xxx_LATCHED_BUTTON_STAT                                       (0xAC)
#define CY8CMBR3xxx_PROX_STAT                                                 (0xAE)
#define CY8CMBR3xxx_LATCHED_PROX_STAT                                         (0xAF)
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3102 | CY8CMBR3xxx_CY8CMBR3108 | CY8CMBR3xxx_CY8CMBR3110 | CY8CMBR3xxx_CY8CMBR3116))
#define CY8CMBR3xxx_SLIDER1_POSITION                                          (0xB0) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110 | CY8CMBR3116
#define CY8CMBR3xxx_LIFTOFF_SLIDER1_POSITION                                  (0xB1) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110 | CY8CMBR3116
#define CY8CMBR3xxx_SLIDER2_POSITION                                          (0xB2) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110 | CY8CMBR3116
#define CY8CMBR3xxx_LIFTOFF_SLIDER2_POSITION                                  (0xB3) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110 | CY8CMBR3116
#endif
#define CY8CMBR3xxx_SYNC_COUNTER0                                             (0xB9)
#define CY8CMBR3xxx_DIFFERENCE_COUNT_SENSOR0                                  (0xBA)
#define CY8CMBR3xxx_DIFFERENCE_COUNT_SENSOR1                                  (0xBC)
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3102))                                         
#define CY8CMBR3xxx_DIFFERENCE_COUNT_SENSOR2                                  (0xBE) // Except CY8CMBR3102
#define CY8CMBR3xxx_DIFFERENCE_COUNT_SENSOR3                                  (0xC0) // Except CY8CMBR3102
#define CY8CMBR3xxx_DIFFERENCE_COUNT_SENSOR4                                  (0xC2) // Except CY8CMBR3102
#define CY8CMBR3xxx_DIFFERENCE_COUNT_SENSOR5                                  (0xC4) // Except CY8CMBR3102
#define CY8CMBR3xxx_DIFFERENCE_COUNT_SENSOR6                                  (0xC6) // Except CY8CMBR3102
#define CY8CMBR3xxx_DIFFERENCE_COUNT_SENSOR7                                  (0xC8) // Except CY8CMBR3102
#endif                                                                            
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3102 | CY8CMBR3xxx_CY8CMBR3108))                           
#define CY8CMBR3xxx_DIFFERENCE_COUNT_SENSOR8                                  (0xCA) // Except CY8CMBR3102 | CY8CMBR3108
#define CY8CMBR3xxx_DIFFERENCE_COUNT_SENSOR9                                  (0xCC) // Except CY8CMBR3102 | CY8CMBR3108
#endif
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3102 | CY8CMBR3xxx_CY8CMBR3108 | CY8CMBR3xxx_CY8CMBR3110))
#define CY8CMBR3xxx_DIFFERENCE_COUNT_SENSOR10                                 (0xCE) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110
#define CY8CMBR3xxx_DIFFERENCE_COUNT_SENSOR11                                 (0xD0) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110
#define CY8CMBR3xxx_DIFFERENCE_COUNT_SENSOR12                                 (0xD2) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110
#define CY8CMBR3xxx_DIFFERENCE_COUNT_SENSOR13                                 (0xD4) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110
#define CY8CMBR3xxx_DIFFERENCE_COUNT_SENSOR14                                 (0xD6) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110
#define CY8CMBR3xxx_DIFFERENCE_COUNT_SENSOR15                                 (0xD8) // Except CY8CMBR3102 | CY8CMBR3108 | CY8CMBR3110
#endif
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3106S))
#define CY8CMBR3xxx_GPO_DATA                                                  (0xDA) // Except CY8CMBR3106S
#endif
#define CY8CMBR3xxx_SYNC_COUNTER1                                             (0xDB)
#define CY8CMBR3xxx_DEBUG_SENSOR_ID                                           (0xDC)
#define CY8CMBR3xxx_DEBUG_CP                                                  (0xDD)
#define CY8CMBR3xxx_DEBUG_DIFFERENCE_COUNT0                                   (0xDE)
#define CY8CMBR3xxx_DEBUG_BASELINE0                                           (0xE0)
#define CY8CMBR3xxx_DEBUG_RAW_COUNT0                                          (0xE2)
#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3106S))
#define CY8CMBR3xxx_DEBUG_AVG_RAW_COUNT0                                      (0xE4) // Except CY8CMBR3106S
#endif
#define CY8CMBR3xxx_SYNC_COUNTER2                                             (0xE7)

#endif

/****************************End of File***************************************/
