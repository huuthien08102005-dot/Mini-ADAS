#include "motor.h"

/****************************************
                DELAY
****************************************/

void delay_motor(uint32_t time)
{
    for(uint32_t i = 0; i < time * 4000; i++);
}

/****************************************
            MOTOR INIT
****************************************/

void Motor_Init(void)
{
    /************************************
            ENABLE CLOCK
    ************************************/

    RCC->AHB1ENR |= (1<<1);

    RCC->APB1ENR |= (1<<1);

    /************************************
            PB0 PB1 PWM
            TIM3_CH3 CH4
    ************************************/

    GPIOB->MODER &= ~(3 << 0);
    GPIOB->MODER &= ~(3 << 2);

    GPIOB->MODER |= (2 << 0);
    GPIOB->MODER |= (2 << 2);

    /*
        AF2 = TIM3
    */

    GPIOB->AFR[0] |= (2 << 0);
    GPIOB->AFR[0] |= (2 << 4);

    /************************************
        PB3 PB4 PB5 PB6 OUTPUT
    ************************************/

    GPIOB->MODER &= ~(3 << 6);
    GPIOB->MODER &= ~(3 << 8);
    GPIOB->MODER &= ~(3 << 10);
    GPIOB->MODER &= ~(3 << 12);

    GPIOB->MODER |= (1 << 6);
    GPIOB->MODER |= (1 << 8);
    GPIOB->MODER |= (1 << 10);
    GPIOB->MODER |= (1 << 12);

    /************************************
                TIM3 PWM
    ************************************/

    /*
        Thiết lập tần số và chu kỳ cho PWM (~1kHz)
    */

    TIM3->PSC = 84 - 1;

    TIM3->ARR = 1000 - 1;

    /*
        Độ rộng xung ban đầu (70% tốc độ)
    */

    TIM3->CCR3 = 700;

    TIM3->CCR4 = 700;

    /************************************
        PWM MODE CH3
    ************************************/

    TIM3->CCMR2 |= (6 << 4);

    TIM3->CCMR2 |= (1 << 3);

    /************************************
        PWM MODE CH4
    ************************************/

    TIM3->CCMR2 |= (6 << 12);

    TIM3->CCMR2 |= (1 << 11);

    /************************************
            ENABLE OUTPUT
    ************************************/

    TIM3->CCER |= (1 << 8);

    TIM3->CCER |= (1 << 12);

    /************************************
                START TIMER
    ************************************/

    TIM3->CR1 |= (1 << 0);
}

/****************************************
            SET SPEED
****************************************/

void Motor_SetSpeed(uint8_t speed)
{
    if(speed > 100)
        speed = 100;

    uint16_t pwm;

    pwm = (speed * 1000) / 100;

    TIM3->CCR3 = pwm;

    TIM3->CCR4 = pwm;
}

/****************************************
            MOTOR FORWARD
****************************************/

void Motor_Forward(void)
{
    /*
        IN1 = 1
        IN2 = 0

        IN3 = 1
        IN4 = 0
    */

    GPIOB->BSRR = (1 << 3);

    GPIOB->BSRR = (1 << (4 + 16));

    GPIOB->BSRR = (1 << 5);

    GPIOB->BSRR = (1 << (6 + 16));
}

/****************************************
            MOTOR BACKWARD
****************************************/

void Motor_Backward(void)
{
    GPIOB->BSRR = (1 << (3 + 16));

    GPIOB->BSRR = (1 << 4);

    GPIOB->BSRR = (1 << (5 + 16));

    GPIOB->BSRR = (1 << 6);
}

/****************************************
            MOTOR LEFT
****************************************/

void Motor_Left(void)
{
    GPIOB->BSRR = (1 << (3 + 16));

    GPIOB->BSRR = (1 << 4);

    GPIOB->BSRR = (1 << 5);

    GPIOB->BSRR = (1 << (6 + 16));
}

/****************************************
            MOTOR RIGHT
****************************************/

void Motor_Right(void)
{
    GPIOB->BSRR = (1 << 3);

    GPIOB->BSRR = (1 << (4 + 16));

    GPIOB->BSRR = (1 << (5 + 16));

    GPIOB->BSRR = (1 << 6);
}

/****************************************
            MOTOR STOP
****************************************/

void Motor_Stop(void)
{
    GPIOB->BSRR = (1 << (3 + 16));

    GPIOB->BSRR = (1 << (4 + 16));

    GPIOB->BSRR = (1 << (5 + 16));

    GPIOB->BSRR = (1 << (6 + 16));

    TIM3->CCR3 = 0;

    TIM3->CCR4 = 0;
}

/****************************************
        SAFE DISTANCE
****************************************/

float Calculate_SafeDistance(float speed)
{
    /*
        d = v*t + v^2 / 2a
    */

    float t = 0.5f;

    float a = 26.6f;

    float d;

    d =
        (speed * t) +
        ((speed * speed) / (2 * a));

    return d;
}

/****************************************
        SAFE CONTROL
****************************************/

void Motor_SafeControl(float speed,
                       int distance)
{
    float safe_distance;

    safe_distance =
        Calculate_SafeDistance(speed);

    /************************************
            EXTREME DANGER
    ************************************/

    if(distance < 30)
    {
        Motor_SetSpeed(5);
    	delay_motor(10);
    	Motor_Stop();
    }

    /************************************
                DANGER
    ************************************/

    else if(distance < safe_distance)
    {
        Motor_SetSpeed(10);

        Motor_Forward();
    }

    /************************************
                WARNING
    ************************************/

    else if(distance >= safe_distance && distance <= (safe_distance+50))
    {
        Motor_SetSpeed(25);

        Motor_Forward();
    }

    /************************************
                    SAFE
    ************************************/

    else
    {
        Motor_SetSpeed((uint32_t)speed);

        Motor_Forward();
    }
}
