#ifndef LED_H
#define LED_H

#include "stm32f4xx.h"

void LED_Init(void);

void Warning_LED_ON(void);

void Warning_LED_OFF(void);

void Danger_LED_ON(void);

void Danger_LED_OFF(void);

#endif
