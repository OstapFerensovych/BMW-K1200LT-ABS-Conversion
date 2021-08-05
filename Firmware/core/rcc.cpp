#include "rcc.h"
#include "platform.hpp"

void rcc_init() {
  /* Set PLL Multiplier to 6x, Set PLL Clock Source to HSI */
  SET_BIT(RCC->CFGR, RCC_CFGR_PLLMUL6 | RCC_CFGR_PLLSRC_HSI_PREDIV);

  /*  Enable PLL and wait for PLL settling */
  SET_BIT(RCC->CR, RCC_CR_PLLON);
  while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) == 0) continue;

  /* Set Flash Latency, Enable Prefetcher and wait for Prefetcher settling */
  SET_BIT(FLASH->ACR, (0x01 << FLASH_ACR_LATENCY_Pos) | FLASH_ACR_PRFTBE);
  while (READ_BIT(FLASH->ACR, FLASH_ACR_PRFTBS) == 0) continue;

  /* Select PLL as System Clock */
  SET_BIT(RCC->CFGR, RCC_CFGR_SW_PLL);

  /* Update SystemCoreClock variable */
  SystemCoreClockUpdate();
}
