#include "rcc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "gpio.h"
#include "lcd.h"
#include <stdio.h>
#include "encoder.h"

bool FlagPushButton;

void vTask1 (void *argument);
void vTask2 (void *argument);
void vTask3 (void *argument);
void vTask4 (void *argument);

int main (void){


	RCC_Init();
	GPIO_Init();
	I2C1_Init();
	LCDLight();
	Encoder_Init();
	
	
	xTaskCreate (vTask1, "Blink", 50, NULL, 2, NULL);
	xTaskCreate (vTask2, "Blink2", 50, NULL, 1, NULL);
	xTaskCreate (vTask3, "Blink3", 50, NULL, 2, NULL);
	xTaskCreate (vTask4, "Blink4", 50, NULL, 2, NULL);
	
	
	vTaskStartScheduler();
	
		while (1) {
				GPIOD->BSRR |= GPIO_BSRR_BS12;
	}
}



void vTask1 (void *argument) {
	
	
	while (1) {
				
		GPIOD->BSRR |= GPIO_BSRR_BS15;
		vTaskDelay (500);
		GPIOD->BSRR |= GPIO_BSRR_BR15;
		vTaskDelay (500);
		
	}
}

void vTask2 (void *argument){
	
	vTaskDelay (40);
	LCDSendHalfByte(0x4E, 0x03);
	vTaskDelay (1);
	LCDSendHalfByte(0x4E, 0x03);
	vTaskDelay (1);
	LCDSendHalfByte(0x4E, 0x03);
	vTaskDelay (1);
	LCDSendHalfByte(0x4E, 0x02);
	LCDSendByte(0x4e, 0x28, 0);
	LCDSendByte(0x4e, 0x08, 0);
	vTaskDelay (2);
	LCDSendByte(0x4e, 0x06, 0);
	vTaskDelay (1);
	LCDSendByte(0x4e, 0x0C, 0);
	LCDSendByte(0x4e, 0x02, 0);
	vTaskDelay (1);
	LCDSendString (0x4E, "STM32F407 Diston erskjinsfiuhseiufhesui");
	LCDClear(0x4E);
	vTaskDelay (2);
	//int32_t test = 0x24424466>>28;
	LCDSendString (0x4E, "RCC->CR = ");
	//LCDSendChar (0x4E, (uint8_t) test + '0');
	LCDSendReg (0x4E, TIM3->CNT);
	//LCDSendChar (0x4E, '5');
	//LCDSendDec (0x4E, TIM3->CNT);*/
	
	while (1) {
	
		LCDClear(0x4E);
		vTaskDelay (2);
		LCDSendString (0x4E, "Encoder 0x");
		//LCDSendReg (0x4E, TIM3->CNT);
		LCDSendDec (0x4E, TIM3->CNT);
		vTaskDelay (100);
		
	}
	
}

void vTask3 (void *argument){
	
	while (1) {
				
		GPIOD->BSRR |= GPIO_BSRR_BS12;
		vTaskDelay (1000);
		GPIOD->BSRR |= GPIO_BSRR_BR12;
		vTaskDelay (1000);
		
	}
	
}

void vTask4 (void *argument){
	
	
	while (1) {
		
		if (((GPIOA->IDR & GPIO_IDR_ID0) == 0) &&  (FlagPushButton)) {
			FlagPushButton = false;
		}
		if (((GPIOA->IDR & GPIO_IDR_ID0) != 0) && (FlagPushButton == false))  { 
			GPIOD->ODR ^= GPIO_ODR_ODR_14;
			FlagPushButton = true;
		}
		if (TIM3->CNT > 90) { TIM3->CNT = 90; }
		else if (TIM3->CNT < 10) { TIM3->CNT = 10; }
		
		vTaskDelay (20);
			
	}
	
}


