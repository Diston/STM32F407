#include "stm32f4xx.h"
#include "stdint.h"

#define LCD_Addr 0x27

void I2C1_Init (void);
void LCD_Send (uint8_t LCDAddr, uint8_t LCDData);
