#include "stm32f4xx.h"
#include "hcsr04.h"
#include "led.h"
#include "uart.h"
#include "buzzer.h"
#include "hc05.h"
#include "ov7670.h"
#include "motor.h"
#include <stdio.h>

char msg[100];
extern int distance;


uint32_t system_tick = 0; //Bộ đếm nhịp chu kỳ
uint32_t camera_tx_timer = 0; //Bộ đếm thời gian trễ giữa các lần gửi dữ liệu camera
uint32_t buzzer_toggle_timer = 0; //Bộ đếm nhịp để điều khiển còi kêu

void delay_ms(uint32_t time)
{
    for(uint32_t i=0; i<time*4000; i++);
}

int main(void)
{
    float car_speed = 40.0;

    HCSR04_Init();
    LED_Init();
    UART2_Init();
    Buzzer_Init();
    HC05_Init();
    OV7670_Init();
    Motor_Init();

    UART_SendString("SYSTEM STARTED\r\n");

    while(1)
    {
        system_tick++;
        if (system_tick % 2 == 1)
        {
            distance = HCSR04_Read();

            sprintf(msg, "Distance = %d cm\r\n", distance);
            UART_SendString(msg);
            HC05_SendString(msg);

            if(distance > 100)
                    {
                        UART_SendString("SAFE\r\n\n");
                        HC05_SendString("SAFE\r\n\n");
                        Warning_LED_OFF();
                        Danger_LED_OFF();
                    }
                    else if(distance >= 50)
                    {
                        UART_SendString("WARNING\r\n\n");
                        HC05_SendString("WARNING\r\n\n");
                        Warning_LED_ON();
                        Danger_LED_OFF();
                    }
                    else
                    {
                        UART_SendString("DANGER\r\n\n");
                        HC05_SendString("DANGER\r\n\n");
                        Warning_LED_OFF();
                        Danger_LED_ON();
                    }
        }
        else
        {
            if(OV7670_MotionDetect())
            {
                if(camera_tx_timer == 0)
                {
                    UART_SendString("MOTION DETECTED\r\n\n");
                    HC05_SendString("MOTION DETECTED\r\n\n");
                    camera_tx_timer = 15;
                }
            }

            if(camera_tx_timer > 0) camera_tx_timer--;
        }
        Motor_SafeControl(car_speed, distance);
        if(distance > 100)
        {
            Buzzer_OFF();
            buzzer_toggle_timer = 0;
        }
        else if(distance >= 50)
        {
            Buzzer_OFF();
            buzzer_toggle_timer = 0;
        }
        else
        {
            buzzer_toggle_timer++;
            if(buzzer_toggle_timer >= (distance / 2))
            {
                static uint8_t buzzer_state = 0;
                if(buzzer_state == 0) {
                    Buzzer_ON();
                    buzzer_state = 1;
                } else {
                    Buzzer_OFF();
                    buzzer_state = 0;
                }
                buzzer_toggle_timer = 0;
            }
        }
        delay_ms(10);
    }
}
