#include "mylib.h"

void RCC_INIT (void){
	
	FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY;        																															// Кэширование данных и задержка флеш памяти для стабильной работы
	
	RCC->CR &= ~RCC_CR_PLLON;																																															 	// Отключаем PLL для изменения коэффициентов
	while((RCC->CR & RCC_CR_PLLRDY) == RCC_CR_PLLRDY);		 																												          
	
	RCC->CFGR &= RCC_CFGR_HPRE;																																															 // AHB PRE set to 1
	RCC->CFGR = ((RCC->CFGR & ~RCC_CFGR_PPRE1) | RCC_CFGR_PPRE1_DIV4);                                                       // APB1 PRE set to 4
	RCC->CFGR = ((RCC->CFGR & ~RCC_CFGR_PPRE2) | RCC_CFGR_PPRE2_DIV2);                                                       // APB2 PRE set to 2
		
	RCC->CR |= RCC_CR_HSEON;																															                                   // Включаем внешний кварц
	while ((RCC->CR & RCC_CR_HSERDY) == 0);
	
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;																																									 // Источник PLL -> HSE
	
	RCC->PLLCFGR = ((RCC->PLLCFGR & ~RCC_PLLCFGR_PLLM) | RCC_PLLCFGR_PLLM_2);   																						 // Коэффициент M = 4
	RCC->PLLCFGR = ((RCC->PLLCFGR & ~RCC_PLLCFGR_PLLN) | RCC_PLLCFGR_PLLN_3 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_7);      // Коэффициент N = 168
	RCC-> PLLCFGR &= ~RCC_PLLCFGR_PLLP;   																																									 // Коэффициент P = 2
		
	RCC->CR |= RCC_CR_PLLON;																																																 // Включаем PLL
	while ((RCC->CR & RCC_CR_PLLRDY) == 0);
	
	RCC->CFGR = ((RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL);                                                              // Переключаем тактирование системной шины от PLL
	
}
