#include "lcd.h"

uint8_t portlcd;

static inline void LCDDelay (uint32_t delay){
	delay = delay * (168/1000);
	while (delay--);
}

void I2C1_Init (void){
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	
	GPIOB->MODER &= ~(GPIO_MODER_MODE6 | GPIO_MODER_MODE7);
	GPIOB->MODER |= GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;
	GPIOB->OTYPER |= GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7;
	GPIOB->AFR[0] |= GPIO_AFRL_AFRL6_2 | GPIO_AFRL_AFRL7_2;
	
	I2C1->CR1 &= ~I2C_CR1_PE;																		// Отключаем шину I2C
	
	I2C1->CR2 |= ((I2C1->CR2 & ~I2C_CR2_FREQ) | 48);								// Устанавливаем частоту шины APB2 48 МГц (110000)
	I2C1->CCR |= 0xF0;																					// Устанавливаем частоту шины I2C 100 кГц (standart) (f.APB/x = 200000)
	I2C1->TRISE = ((I2C1->TRISE & ~I2C_TRISE_TRISE) | 49);    // Устанавливаем время нарастания сигнала (rise time) 1000 мкс (1000 мкс / (1/f.AFP))

	I2C1->CR1 &= ~I2C_CR1_NOSTRETCH;															// Включаем ожидание ответа
	
	I2C1->CR1 |= I2C_CR1_PE;
	I2C1->CR1 |= I2C_CR1_ACK;
	
	
}


void LCD_Send (uint8_t LCDAddr, uint8_t LCDData){
	
	I2C1->CR1 &= ~I2C_CR1_POS;
	
	I2C1->CR1 |= I2C_CR1_START;
  while (!(I2C1->SR1 & I2C_SR1_SB));

	I2C1->DR = ((I2C1->DR & ~I2C_DR_DR) | LCDAddr);
	while (!(I2C1->SR1 & I2C_SR1_ADDR));
	
	volatile uint16_t LCDReadSR2 = I2C1->SR2;
	
	I2C1->DR = (I2C1->DR & ~I2C_DR_DR) | LCDData;
  while (!(I2C1->SR1 & I2C_SR1_TXE));
	
	I2C1->CR1 |= I2C_CR1_STOP;
}

void LCDSendHalfByte (uint8_t LCDAddr, uint8_t LCDData){
	LCDData<<=4;
	LCD_Send (LCDAddr, portlcd|=0x04);
	//LCDDelay (1000);
	LCD_Send (LCDAddr, portlcd|LCDData);
	LCD_Send (LCDAddr, portlcd&=~0x04);
	//LCDDelay (1000);
}

void LCDSendByte (uint8_t LCDAddr, uint8_t LCDData, uint8_t mode){
	if (mode==0)  LCD_Send (LCDAddr, portlcd&=~0x01);
	else LCD_Send (LCDAddr, portlcd|=0x01);
	uint8_t hc=0;
	hc=LCDData>>4;
	LCDSendHalfByte (LCDAddr, hc);
	LCDSendHalfByte (LCDAddr, LCDData);
}

void LCDLight (void) {
	
	LCD_Send (0x4E, portlcd|=0x08);

}

void LCDSendChar (uint8_t LCDAddr, char LCDData){
	
	LCDSendByte (LCDAddr, (uint8_t) LCDData, 1);
	
}

void LCDSendString (uint8_t LCDAddr, char* LCDData){
	
	uint8_t i=0;
	
	while (LCDData[i]!=0)
	{
		LCDSendByte (LCDAddr, LCDData[i], 1);
		i++;
	}
}

void LCDClear (uint8_t LCDAddr){
	LCDSendByte (LCDAddr, 0x01, 0);
}

void LCDSendReg (uint8_t LCDAddr, uint32_t LCDData){
	for (int8_t i=28; i>=0; i=i-4){
		int32_t LCDReg = LCDData>>i;
		LCDReg &= 0xF;
		if (LCDReg <=0x9) {
			LCDSendChar (LCDAddr, LCDReg + '0');
		}
		else {
			LCDSendChar (LCDAddr, LCDReg - 0x9 + '@');
		}
	}
}

void LCDSendDec (uint8_t LCDAddr, uint32_t LCDData){

	int8_t i=0;
	uint32_t LCDData2 = LCDData;
	uint8_t LCDData3[10];
	
	while (LCDData2>0){
		LCDData3[i] = LCDData2 % 10;
		LCDData2 = LCDData2 / 10;
		i++;
	}
	if (i>0) i--;
	
	while (i>=0){
		LCDSendChar (LCDAddr, LCDData3[i]+'0');
		i--;
	}
}
		
	
		



