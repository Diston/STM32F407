#include "mylib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void vTask1 (void *argument);
void vTask2 (void *argument);

int main (void){

	RCC_INIT();
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= GPIO_MODER_MODER15_0;
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15_0;
	GPIOD->MODER |= GPIO_MODER_MODER13_0;
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13_0;
		GPIOD->MODER |= GPIO_MODER_MODER12_0;
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12_0;
	
	xTaskCreate (vTask1, "Blink", 50, NULL, 1, NULL);
	xTaskCreate (vTask2, "Blink2", 50, NULL, 2, NULL);
	
	vTaskStartScheduler();
	
		while (1) {
				GPIOD->BSRR |= GPIO_BSRR_BS12;
	}
}



void vTask1 (void *argument) {
	
	while (1) {
				
		GPIOD->BSRR |= GPIO_BSRR_BS15;
		vTaskDelay (300);
		GPIOD->BSRR |= GPIO_BSRR_BR15;
		vTaskDelay (300);
		
	}
}

void vTask2 (void *argument){
	
	while (1) {
				
		GPIOD->BSRR |= GPIO_BSRR_BS13;
		vTaskDelay (300);
		GPIOD->BSRR |= GPIO_BSRR_BR13;
		vTaskDelay (300);
		
	}
	
}