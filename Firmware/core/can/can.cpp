#include "can.hpp"
#include "platform.hpp"
#include <cstring>
#include <stdio.h>
#include "can_def.hpp"

struct can_Message_t {
    uint32_t id = 0x000;  // 11-bit max is 0x7ff, 29-bit max is 0x1FFFFFFF
    bool isExt = false;
    bool rtr = false;
    uint8_t len = 8;
    uint8_t buf[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
} ;

volatile bool front_lever = false;
volatile bool rear_lever = false;
volatile bool front_sensor_ok = false; //After two turns of the wheel
volatile bool rear_sensor_ok = false; //After two turns of the wheel
volatile float speed_kmh = 0;
volatile bool abs_status = false; // ABS ON/OFF Status

bool CAN_front_lever() {return front_lever;}
bool CAN_rear_lever() {return rear_lever;}
bool CAN_front_sensor_ok() {return front_sensor_ok;}
bool CAN_rear_sensor_ok() {return rear_sensor_ok;}
bool CAN_abs_on() {return abs_status;}
uint8_t CAN_Speed() {return speed_kmh;}

//static void CAN_Transmit(can_Message_t msg);
static void abs_status_callback(can_Message_t *msg);
static void process_message(CanCmd_e cmd, can_Message_t *msg);

int CAN_Init() {
    uint32_t tmo;

    /* RCC CAN Config */
    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_CANEN);

    /* Try to enter Initialization mode */
    SET_BIT(CAN->MCR, CAN_MCR_INRQ);

    /* Wait init mode */
    tmo = CAN_INIT_MODE_TMO;
    while(tmo-- && READ_BIT(CAN->MSR, CAN_MSR_INAK) != CAN_MSR_INAK);

    if(READ_BIT(CAN->MSR, CAN_MSR_INAK) != CAN_MSR_INAK){
        return -1;
    }

    CLEAR_BIT(CAN->MCR, CAN_MCR_SLEEP);

    CLEAR_BIT(CAN->MCR, CAN_MCR_TTCM);   //Time triggered communication mode OFF
    SET_BIT(CAN->MCR, CAN_MCR_ABOM);     //Bus-Off recovery ON
    SET_BIT(CAN->MCR, CAN_MCR_AWUM);     //Automatic wakeup mode ON
    CLEAR_BIT(CAN->MCR, CAN_MCR_NART);   //Automatic retransmission ON
    CLEAR_BIT(CAN->MCR, CAN_MCR_RFLM);   //Receive FIFO not locked on overrun
    CLEAR_BIT(CAN->MCR, CAN_MCR_TXFP);   //Priority not driven by the identifier of the message

    //  WRITE_REG(CAN->BTR, (uint32_t)(CAN_MODE_NORMAL |
    //                                 CAN_SJW_1TQ  |
    //                                 CAN_BS1_13TQ |
    //                                 CAN_BS2_2TQ  |
    //                                 (3 - 1))); // 1Mbit/s 0x001c0002

    WRITE_REG(CAN->BTR, (uint32_t)(CAN_MODE_NORMAL |
                                   CAN_SJW_1TQ  |
                                   CAN_BS1_13TQ |
                                   CAN_BS2_2TQ  |
                                   (6 - 1))); // 500kbit/s

//    WRITE_REG(CAN->BTR, (uint32_t)(CAN_MODE_NORMAL |
//                                   CAN_SJW_1TQ  |
//                                   CAN_BS1_13TQ |
//                                   CAN_BS2_2TQ  |
//                                   (12 - 1))); // 250kbit/s

    /* Exit Initialization mode */
    CLEAR_BIT(CAN->MCR, CAN_MCR_INRQ);

    /* Wait exit */
    tmo = CAN_INIT_MODE_TMO;
    while(tmo-- && READ_BIT(CAN->MSR, CAN_MSR_INAK) != CAN_MSR_INAK);
    if(READ_BIT(CAN->MSR, CAN_MSR_INAK) != CAN_MSR_INAK){
        return -1;
    }

    /* Enter filter init mode */
    SET_BIT(CAN->FMR, CAN_FMR_FINIT);
    CAN->FS1R |= (CAN_FS1R_FSC0);
//    CAN->FS1R |= (CAN_FS1R_FSC0 | CAN_FS1R_FSC1);

    /* Acivate filter 0 */
    SET_BIT(CAN->FA1R, CAN_FA1R_FACT0);
//    SET_BIT(CAN->FA1R, CAN_FA1R_FACT1);

    /*  Set the Id and the mask */
    CAN->sFilterRegister[0].FR1 = can_id_abs_status << 21; //ID CAN_CMD_LED
    CAN->sFilterRegister[0].FR2 = CAN_ID_MASK << 21; //MASK

    //  CAN->sFilterRegister[1].FR1 = SENSCAP_MAKE_CANID(CS_MSG_SET_CONFIG) << 21; //ID CAN_CMD_SERVICE
    //  CAN->sFilterRegister[1].FR2 = CAN_ID_MASK << 21; //MASK

    /* Leave filter init */
    CLEAR_BIT(CAN->FMR, CAN_FMR_FINIT);

    /* Set FIFO0 message pending IT enable */
    SET_BIT(CAN->IER, CAN_IER_FMPIE0);
    //  SET_BIT(CAN->IER, CAN_IER_FMPIE1);

    NVIC_EnableIRQ(CEC_CAN_IRQn);
    NVIC_SetPriority(CEC_CAN_IRQn, 0);

    return 0;
}

static void CAN_Transmit(can_Message_t msg) {
    uint32_t txmailbox;
    uint32_t tsr = READ_REG(CAN->TSR);

    if (((tsr & CAN_TSR_TME0) != 0U) ||
            ((tsr & CAN_TSR_TME1) != 0U) ||
            ((tsr & CAN_TSR_TME2) != 0U)) {
        txmailbox = (tsr & CAN_TSR_CODE) >> CAN_TSR_CODE_Pos;
        if (txmailbox > 2U) {
            return; //ERROR
        }

        CAN->sTxMailBox[txmailbox].TDTR = msg.len; //fill data length

        if(msg.len != 0) {
            WRITE_REG(CAN->sTxMailBox[txmailbox].TDHR,
                      ((uint32_t)msg.buf[7] << CAN_TDH0R_DATA7_Pos) |
                    ((uint32_t)msg.buf[6] << CAN_TDH0R_DATA6_Pos) |
                    ((uint32_t)msg.buf[5] << CAN_TDH0R_DATA5_Pos) |
                    ((uint32_t)msg.buf[4] << CAN_TDH0R_DATA4_Pos));
            WRITE_REG(CAN->sTxMailBox[txmailbox].TDLR,
                      ((uint32_t)msg.buf[3] << CAN_TDL0R_DATA3_Pos) |
                    ((uint32_t)msg.buf[2] << CAN_TDL0R_DATA2_Pos) |
                    ((uint32_t)msg.buf[1] << CAN_TDL0R_DATA1_Pos) |
                    ((uint32_t)msg.buf[0] << CAN_TDL0R_DATA0_Pos));
        }

        if (!msg.isExt) {
            CAN->sTxMailBox[txmailbox].TIR = (msg.id << CAN_TI0R_STID_Pos);
            CLEAR_BIT(CAN->sTxMailBox[txmailbox].TIR, CAN_TI0R_IDE);
        } else {
            CAN->sTxMailBox[txmailbox].TIR = (msg.id << CAN_TI0R_EXID_Pos);
            SET_BIT(CAN->sTxMailBox[txmailbox].TIR, CAN_TI0R_IDE);
        }
        CAN->sTxMailBox[txmailbox].TIR |= msg.rtr << CAN_TI0R_RTR_Pos;
        CAN->sTxMailBox[txmailbox].TIR |= CAN_TI0R_TXRQ; // request a transmission
    }
}

extern "C" void CEC_CAN_IRQHandler(void);
extern "C" void CEC_CAN_IRQHandler(void) {
    uint8_t rx_fifo = 0;
    can_Message_t rxmsg;
    uint16_t mailbox_id;
    // check if a message is filtered and received by FIFO 0,1
    if ((CAN->RF0R & CAN_RF0R_FMP0) != 0) {
        __disable_irq();
        mailbox_id = (CAN->sFIFOMailBox[rx_fifo].RDTR & CAN_RDT0R_FMI) >> CAN_RDT0R_FMI_Pos;
        rxmsg.len = CAN->sFIFOMailBox[rx_fifo].RDTR & CAN_RDT0R_DLC;
        rxmsg.isExt = (CAN->sFIFOMailBox[rx_fifo].RIR & CAN_RI0R_IDE_Msk) >> CAN_RI0R_IDE_Pos;

        if (!rxmsg.isExt) {
            rxmsg.id = (CAN->sFIFOMailBox[rx_fifo].RIR & CAN_RI0R_STID_Msk) >> CAN_RI0R_STID_Pos;
        } else {
            rxmsg.id = ((CAN_RI0R_EXID | CAN_RI0R_STID) & CAN->sFIFOMailBox[rx_fifo].RIR) >> CAN_RI0R_EXID_Pos;
        }

        rxmsg.rtr = (CAN->sFIFOMailBox[0].RIR & CAN_RI0R_RTR_Msk) >> CAN_RI0R_RTR_Pos;

        if (rx_fifo == 0) {
            /* Release RX FIFO 0 */
            SET_BIT(CAN->RF0R, CAN_RF0R_RFOM0);
        } else {
            /* Release RX FIFO 1 */
            SET_BIT(CAN->RF1R, CAN_RF1R_RFOM1);
        }

        if(rxmsg.len != 0) {
            rxmsg.buf[0] = (uint8_t)((CAN_RDL0R_DATA0 & CAN->sFIFOMailBox[rx_fifo].RDLR) >> CAN_RDL0R_DATA0_Pos);
            rxmsg.buf[1] = (uint8_t)((CAN_RDL0R_DATA1 & CAN->sFIFOMailBox[rx_fifo].RDLR) >> CAN_RDL0R_DATA1_Pos);
            rxmsg.buf[2] = (uint8_t)((CAN_RDL0R_DATA2 & CAN->sFIFOMailBox[rx_fifo].RDLR) >> CAN_RDL0R_DATA2_Pos);
            rxmsg.buf[3] = (uint8_t)((CAN_RDL0R_DATA3 & CAN->sFIFOMailBox[rx_fifo].RDLR) >> CAN_RDL0R_DATA3_Pos);
            rxmsg.buf[4] = (uint8_t)((CAN_RDH0R_DATA4 & CAN->sFIFOMailBox[rx_fifo].RDHR) >> CAN_RDH0R_DATA4_Pos);
            rxmsg.buf[5] = (uint8_t)((CAN_RDH0R_DATA5 & CAN->sFIFOMailBox[rx_fifo].RDHR) >> CAN_RDH0R_DATA5_Pos);
            rxmsg.buf[6] = (uint8_t)((CAN_RDH0R_DATA6 & CAN->sFIFOMailBox[rx_fifo].RDHR) >> CAN_RDH0R_DATA6_Pos);
            rxmsg.buf[7] = (uint8_t)((CAN_RDH0R_DATA7 & CAN->sFIFOMailBox[rx_fifo].RDHR) >> CAN_RDH0R_DATA7_Pos);
        }
        __enable_irq();

//        printf("id:0x%lX data:0x%02X%02X%02X%02X%02X%02X%02X%02X\n", rxmsg.id, rxmsg.buf[7], rxmsg.buf[6], rxmsg.buf[5], rxmsg.buf[4], rxmsg.buf[3], rxmsg.buf[2], rxmsg.buf[1], rxmsg.buf[0]);
//        printf("f:%d r:%d sf:%d sr:%d\n", front_lever, rear_lever, front_sensor_ok, rear_sensor_ok);
//        printf("speed:%d\n", speed_kmh);
        process_message(static_cast<CanCmd_e>(mailbox_id), &rxmsg);
    }
}

static void process_message(CanCmd_e cmd, can_Message_t *msg) {
    //TODO: Actually cmd is not a command id. (32bit mailbox reserves 2 filters)
    if(cmd == 0){
       abs_status_callback(msg);
    }
}

static void abs_status_callback(can_Message_t *msg) {
    uint64_t data;
    memcpy(&data, msg->buf, sizeof(uint64_t));

    front_lever = (data & CAN_FR_LEVR_Msk) >> CAN_FR_LEVR_Pos;
    rear_lever = (data & CAN_RR_LEVR_Msk) >> CAN_RR_LEVR_Pos;
    front_sensor_ok = (data & CAN_FR_SENSOR_OK_Msk) >> CAN_FR_SENSOR_OK_Pos; //Goes 0 after two turns of the wheel
    rear_sensor_ok = (data & CAN_RR_SENSOR_OK_Msk) >> CAN_RR_SENSOR_OK_Pos; //Goes 0 after two turns of the wheel
    speed_kmh = ((data & CAN_SPEED_Msk) >> CAN_SPEED_Pos) * 0.06f; //0.0625f;
    abs_status = ((data & CAN_ABS_STATUS_Msk) >> CAN_ABS_STATUS_Pos) == 0x5;
}

void send_abs_status() {
    can_Message_t msg;
    msg.id = 0x10C;
    msg.buf[1] = 0x0;
    msg.buf[2] = 0x30;// RPM 1100
    msg.buf[3] = 0x11;// RPM 1100
    msg.buf[5] = (0x6 << 4) | 0x5;
    msg.buf[6] = 0xF0;
//     msg.buf[5] = (0x5 << 4) | 0x5;
    CAN_Transmit(msg);
}
