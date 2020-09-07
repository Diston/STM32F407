#include "stm32f4xx.h"
#include "stdint.h"

#define LCD_Addr 0x27



static inline void LCDDelay (uint32_t);
void I2C1_Init (void);
void LCD_Send (uint8_t LCDAddr, uint8_t LCDData);
void LCDSendHalfByte (uint8_t LCDAddr, uint8_t LCDData);
void LCDSendByte (uint8_t LCDAddr, uint8_t LCDData, uint8_t mode);
void LCDLight (void);
void LCDSendChar (uint8_t LCDAddr, char LCDData);
void LCDSendString (uint8_t LCDAddr, char* LCDData);
void LCDClear (uint8_t LCDAddr);
void LCDSendReg (uint8_t LCDAddr, uint32_t LCDData);
void LCDSendDec (uint8_t LCDAddr, uint32_t LCDData);