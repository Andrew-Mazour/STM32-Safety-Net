// Sensor Board
"""
    Includes sensor to send signals to other stm32
"""

#include "main.h"
#include <string.h>
#include <stdio.h>

#define TRIG_PIN GPIO_PIN_9  
#define ECHO_PIN GPIO_PIN_8  
#define TRIG_PORT GPIOA      
#define ECHO_PORT GPIOA

#define DISTANCE_THRESHOLD 10  // Distance in cm to trigger alert

TIM_HandleTypeDef htim1;
UART_HandleTypeDef huart2;  // Bluetooth via USART2

void delay_us(uint16_t us) {
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    while (__HAL_TIM_GET_COUNTER(&htim1) < us);
}

uint32_t get_distance() {
    uint32_t start_time = 0, stop_time = 0;

    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);

    while (HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_PIN_RESET);
    start_time = __HAL_TIM_GET_COUNTER(&htim1);

    while (HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_PIN_SET);
    stop_time = __HAL_TIM_GET_COUNTER(&htim1);

    uint32_t pulse_duration = stop_time - start_time;
    return pulse_duration * 0.034 / 2;
}

void send_alert() {
    char message[] = "ALERT\n";
    HAL_UART_Transmit(&huart2, (uint8_t *)message, strlen(message), 100);
}

int main(void) {
    HAL_Init();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();

    // GPIO Configurations
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // TRIG Output
    GPIO_InitStruct.Pin = TRIG_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(TRIG_PORT, &GPIO_InitStruct);

    // ECHO Input
    GPIO_InitStruct.Pin = ECHO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ECHO_PORT, &GPIO_InitStruct);

    // UART Config
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    HAL_UART_Init(&huart2);

    // Timer Config
    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 84 - 1;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = 0xFFFF;
    HAL_TIM_Base_Init(&htim1);
    HAL_TIM_Base_Start(&htim1);

    while (1) {
        uint32_t distance = get_distance();
        if (distance > 0 && distance < DISTANCE_THRESHOLD) {
            send_alert();
            HAL_Delay(1000);
        }
        HAL_Delay(500);
    }
}
