/*
 * @Author: DuRuofu duruofu@qq.com
 * @Date: 2024-02-12 16-22-52
 * @LastEditors: DuRuofu
 * @LastEditTime: 2024-04-07 11-47-45
 * @FilePath: \STM32F103_Demo\02.UART\03.USART_IDLE_DMA\Users\Components\Usart\uart_it_config.c
 * @Description: 串口中断配置(HAL)
 * Copyright (c) 2024 by duruofu@foxmail.com All Rights Reserved.
 */

#include "uart_it_config.h"

/**
 * @description: 串口接收中断初始化(总)
 * @return {*}Debug_Init
 */
void UART_IT_Init(void)
{
    // 串口3接收中断初始化
    UART3_Init();
    // 串口2接收中断初始化
    UART2_Init();
    // 串口3接收中断初始化
    // UART3_Init();
}

// 串口接收完成回调函数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1)
    {
        UARTE3_RxEventCallback(huart, Size);
    }
    else if (huart->Instance == USART2)
    {
        UARTE2_RxEventCallback(huart, Size);
    }
    else if (huart->Instance == USART3)
    {
        // UART3_RxCpltCallback(huart);
    }
}

// 错误回调
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        UART3_ErrorCallback(huart);
    }
    else if (huart->Instance == USART2)
    {
        UART2_ErrorCallback(huart);
    }
    else if (huart->Instance == USART3)
    {
        // USART3_ErrorCallback(huart);
    }
}
