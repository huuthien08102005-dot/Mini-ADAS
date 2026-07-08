#include "led.h"

void LED_Init(void)
{
    /**************************************
        Enable GPIOA clock
    **************************************/

    RCC->AHB1ENR |= (1<<0);

    /**************************************
        PA4 = WARNING LED
        PA5 = DANGER LED
    **************************************/

    GPIOA->MODER |= (1<<8);

    GPIOA->MODER |= (1<<10);
}

/**************************************
    WARNING LED
**************************************/

void Warning_LED_ON(void)
{
    GPIOA->ODR |= (1<<4);
}

void Warning_LED_OFF(void)
{
    GPIOA->ODR &= ~(1<<4);
}

/**************************************
    DANGER LED
**************************************/

void Danger_LED_ON(void)
{
    GPIOA->ODR |= (1<<5);
}

void Danger_LED_OFF(void)
{
    GPIOA->ODR &= ~(1<<5);
}
