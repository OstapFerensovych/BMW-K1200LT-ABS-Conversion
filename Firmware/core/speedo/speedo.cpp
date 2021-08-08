#include "speedo.hpp"
#include <stm32f0xx.h>

std::array<std::pair <uint8_t, float>, 23> kmh_to_hz = {
    {
        {5, 3.5},
        {10, 7.5},
        {20, 14.8},
        {30, 22.5},
        {40, 30.2},
        {50, 38},
        {60, 45.7},
        {70, 53},
        {80, 61},
        {90, 69},
        {100, 77},
        {110, 85},
        {120, 93.5},
        {130, 102},
        {140, 110},
        {150, 118.5},
        {160, 128},
        {170, 137},
        {180, 146},
        {190, 155},
        {200, 164},
        {210, 173},
        {220, 182}
    }
};

void SPEEDO_Init() {
    //TIM3_CH2
    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);
    TIM3->PSC = 60000-1;
    TIM3->ARR = 240-1;
    TIM3->CCR2 = 120-1;
    TIM3->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;
    TIM3->CCER |= TIM_CCER_CC2E;
    TIM3->BDTR |= TIM_BDTR_MOE;
    TIM3->CR1 |= TIM_CR1_CEN;
    TIM3->EGR |= TIM_EGR_UG;
}

template<std::size_t SIZE>
uint16_t mmap (int val, std::array<std::pair <uint8_t, float>, SIZE>& arr) {
    if (val <= arr[0].first) return arr[0].second;
    if (val >= arr[SIZE - 1].first) return arr[SIZE - 1].second;
    uint8_t pos = 1;
    while (val > arr[pos].first) pos++;
    if (val == arr[pos].first) return arr[pos].second;
    return (val - arr[pos - 1].first) * (arr[pos].second - arr[pos - 1].second) / (arr[pos].first - arr[pos - 1].first) + arr[pos - 1].second;
}

void SPEEDO_Out(float kmh) {
    if(kmh > 5) {
        SET_BIT(TIM3->CR1, TIM_CR1_CEN);
        uint16_t pwm = 200000 / mmap(kmh, kmh_to_hz);
        TIM3->PSC = pwm;
    } else {
        CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN);
    }
}

