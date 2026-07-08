#ifndef HC05_H
#define HC05_H

#include "stm32f4xx.h"

void HC05_Init(void);

void HC05_SendChar(char c);

void HC05_SendString(char *str);

void HC05_Init_Scan(uint16_t scan_brr);

#endif
