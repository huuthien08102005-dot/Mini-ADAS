#include "uart.h"

void UART2_Init(void)
{
    /**************************************
        Enable Clock
    **************************************/

    RCC->AHB1ENR |= (1<<0);

    RCC->APB1ENR |= (1<<17);

    /**************************************
        PA2 = TX
        PA3 = RX
    **************************************/

    GPIOA->MODER |= (2<<4);

    GPIOA->MODER |= (2<<6);

    GPIOA->AFR[0] |= (7<<8);

    GPIOA->AFR[0] |= (7<<12);

    /**************************************
        Baudrate 9600
    **************************************/

    USART2->BRR = 0x0683;

    /**************************************
        Enable TX RX
    **************************************/

    USART2->CR1 |= (1<<3);

    USART2->CR1 |= (1<<2);

    /**************************************
        Enable USART
    **************************************/

    USART2->CR1 |= (1<<13);
}

void UART_SendChar(char c)
{
    while(!(USART2->SR & (1<<7)));
    USART2->DR = c;
}

void UART_SendString(char *str)
{
    while(*str)
    {
        UART_SendChar(*str++);
    }
}
