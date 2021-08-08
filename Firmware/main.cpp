#include "errors.hpp"
#include "gpio.hpp"
#include "rcc.h"
#include "stm32f0xx.h"
#include "timing.hpp"
#include "can.hpp"
#include "speedo.hpp"

[[noreturn]] int main();
[[noreturn]] int main() {
    rcc_init();
    errors_init();
    timing_init();
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGCOMPEN);
    READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGCOMPEN);
    SET_BIT(SYSCFG->CFGR1, SYSCFG_CFGR1_PA11_PA12_RMP);
    gpio::init();
    timing_sleep_ms(1000);
    gpio::clear<gpio::line::tail_lights>();

    CAN_Init();
    SPEEDO_Init();
    SPEEDO_Out(0);

    for (;;){
        if(CAN_front_lever() || CAN_rear_lever()) {
            gpio::clear<gpio::line::brake_lights>();
        } else {
            gpio::set<gpio::line::brake_lights>();
        }

        if(CAN_front_sensor_ok() && CAN_rear_sensor_ok()) {
            gpio::clear<gpio::line::abs_warn_relay>();
        } else {
            gpio::set<gpio::line::abs_warn_relay>();
        }

        SPEEDO_Out(CAN_Speed());

//        gpio::set<gpio::line::speedo>();
//        timing_sleep_ms(10);
//        gpio::clear<gpio::line::speedo>();
//        timing_sleep_ms(10);
//        printf("f:%d r:%d sf:%d sr:%d\n", CAN_front_lever() , CAN_rear_lever() , CAN_front_sensor_ok() , CAN_rear_sensor_ok() );

        // Speed
//        gpio::set<gpio::line::speedo>();
//        timing_sleep_ms(10);
//        gpio::clear<gpio::line::speedo>();
//        timing_sleep_ms(10);

//        // Lights
//        gpio::set<gpio::line::tail_lights>();
//        gpio::clear<gpio::line::tail_lights>();


        //    continue;
    }
}
