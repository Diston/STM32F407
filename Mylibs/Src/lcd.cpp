#include "lcd.h"


void LCD_Init (void){
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	
	GPIOB->MODER &= ~(GPIO_MODER_MODE6 | GPIO_MODER_MODE7);
	GPIOB->MODER |= GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;
	GPIOB->OTYPER |= GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7;
	GPIOB->AFR[0] |= GPIO_AFRL_AFRL6_2 | GPIO_AFRL_AFRL7_2;
	
	I2C1->CR1 &= ~I2C_CR1_PE;																		// ��������� ���� I2C
	
	I2C1->CR2 |= ((I2C1->CR2 & ~I2C_CR2_FREQ) | 48);								// ������������� ������� ���� APB2 48 ��� (110000)
	I2C1->CCR |= 0xF0;																					// ������������� ������� ���� I2C 100 ��� (standart) (f.APB/x = 200000)
	I2C1->TRISE = ((I2C1->TRISE & ~I2C_TRISE_TRISE) | 49);    // ������������� ����� ���������� ������� (rise time) 1000 ��� (1000 ��� / (1/f.AFP))

	I2C1->CR1 &= ~I2C_CR1_NOSTRETCH;															// �������� �������� ������
	
	I2C1->CR1 |= I2C_CR1_PE;
	I2C1->CR1 |= I2C_CR1_ACK;
}


void LCD_Send (void){
	
	I2C1->CR1 &= ~I2C_CR1_POS;
	I2C1->CR1 = ((I2C1->CR1 & ~I2C_CR1_ACK) | I2C_CR1_ACK);
	
	I2C1->CR1 |= I2C_CR1_START;
  while (!(I2C1->SR1 & I2C_SR1_SB));
	//(void)I2C1->SR1;

	I2C1->DR = ((I2C1->DR & ~I2C_DR_DR) | 0x4E);

	while (!(I2C1->SR1 & I2C_SR1_ADDR));
	(void)I2C1->SR1;
	(void)I2C1->SR2;
	while (!(I2C1->SR1 & I2C_SR1_ADDR));
	while (I2C1->SR2 && 0);
	I2C1->DR = (I2C1->DR & ~I2C_DR_DR) | 0x04;
  while (!(I2C1->SR1 & I2C_SR1_TXE));
	
	I2C1->CR1 |= I2C_CR1_STOP;
}
