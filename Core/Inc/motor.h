#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f4xx.h"

/****************************************
            MOTOR FUNCTIONS
****************************************/

void Motor_Init(void);

void Motor_Forward(void);

void Motor_Backward(void);

void Motor_Left(void);

void Motor_Right(void);

void Motor_Stop(void);

/****************************************
                PWM
****************************************/

void Motor_SetSpeed(uint8_t speed);

/****************************************
        SAFE DISTANCE SYSTEM
****************************************/

float Calculate_SafeDistance(float speed);

void Motor_SafeControl(float speed,
                       int distance);

#endif
