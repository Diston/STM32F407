#include "rcc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "gpio.h"
#include "lcd.h"

bool FlagPushButton;
uint8_t portlcd;

void vTask1 (void *argument);
void vTask2 (void *argument);
void vTask3 (void *argument);
void vTask4 (void *argument);

int main (void){


	RCC_Init();
	GPIO_Init();
	I2C1_Init();
	LCD_Send(0x4E,portlcd|=0x08);
	
	
	xTaskCreate (vTask1, "Blink", 50, NULL, 2, NULL);
	xTaskCreate (vTask2, "Blink2", 50, NULL, 2, NULL);
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
	
	while (1) {
	
		
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
		vTaskDelay (20);
			
	}
	
}


