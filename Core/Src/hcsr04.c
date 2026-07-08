#include "hcsr04.h"

int distance;

/****************************************/

void delay_us(uint32_t time)
{
    for(uint32_t i=0;i<time*16;i++);
}

/****************************************/

void HCSR04_Init(void)
{
    RCC->AHB1ENR |= (1<<0);

    GPIOA->MODER |= (1 << 0);

    GPIOA->MODER &= ~(3 << 2);
}

/****************************************/

int HCSR04_Read(void)
{
    uint32_t time = 0;

    GPIOA->BSRR = (1 << 0);

    delay_us(10);

    GPIOA->BSRR = (1 << 16);

    while(!(GPIOA->IDR & (1 << 1)));

    while(GPIOA->IDR & (1 << 1))
    {
        time++;
    }

    distance = time / 58;

    return distance;
}
