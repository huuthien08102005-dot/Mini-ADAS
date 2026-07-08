#ifndef __OV7670_H
#define __OV7670_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include <stdint.h>

void OV7670_Delay(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_WriteByte(uint8_t data);
void SCCB_Write(uint8_t reg, uint8_t data);
void XCLK_Init(void);
void OV7670_Init(void);
int OV7670_MotionDetect(void);

#ifdef __cplusplus
}
#endif

#endif
