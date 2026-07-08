#ifndef HCSR04_H_
#define HCSR04_H_

#include "stm32f4xx.h"

extern int distance;

void HCSR04_Init(void);

int HCSR04_Read(void);

#endif
