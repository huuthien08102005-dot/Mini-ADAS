#ifndef BUZZER_H
#define BUZZER_H

#include "stm32f4xx.h"

void Buzzer_Init(void);

void Buzzer_ON(void);

void Buzzer_OFF(void);

void Buzzer_Control(int distance);

#endif
