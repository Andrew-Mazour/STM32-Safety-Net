// Alarm/Light Board
"""
    Receives data from other stm32 to activate

"""

#include "main.h"
#include <string.h>
#include <stdio.h>

#define BUZZER_PIN GPIO_PIN_6  
#define BUZZER_PORT GPIOB
#define LED_PIN GPIO_PIN_5     
#define LED_PORT GPIOB

UART_HandleTypeDef huart2;
char received_data[10];

void activate_alarm() {
    for (int i = 0; i < 5; i++) {
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET);
        HAL_Delay(200);
        HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
        HAL_Delay(200);
    }
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (strstr(received_data, "ALERT")) {
        activate_alarm();
    }
    HAL_UART_Receive_IT(&huart2, (uint8_t *)received_data, 6);
}

int main(void) {
    HAL_Init();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // LED and Buzzer Outputs
    GPIO_InitStruct.Pin = LED_PIN | BUZZER_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // UART Config
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    HAL_UART_Init(&huart2);

    // Start receiving data
    HAL_UART_Receive_IT(&huart2, (uint8_t *)received_data, 6);

    while (1) {
        // Nothing needed here, interrupt handles alerts
    }
}
