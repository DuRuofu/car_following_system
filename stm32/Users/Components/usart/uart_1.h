#include "main.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifndef __UART_3_H
#define __UART_3_H

#define DEBUG

#ifdef DEBUG
// 如果 DEBUG 宏已定义，则定义以下宏用于调试输出
#define DEBUG_printf(tag, format, ...) Usart3DmaPrintf("DEBUG_printf[" tag "]:" format "\r\n", ##__VA_ARGS__)
#define DEBUG_info(format, ...) Usart3DmaPrintf(format"\r\n", ##__VA_ARGS__)
#define DEBUG_warnig(tag, format, ...) Usart3DmaPrintf("DEBUG_warnig[" tag "]:" format "\r\n", ##__VA_ARGS__)
#define DEBUG__error(tag, format, ...) Usart3DmaPrintf("DEBUG__error[" tag "]:" format "\r\n", ##__VA_ARGS__)
#else
// 如果 DEBUG 宏未定义，则定义以下宏为空，以屏蔽调试输出
#define DEBUG_printf(format, ...) printf(format "\r\n", ##__VA_ARGS__)
#define DEBUG_info(tag, format, ...)
#define DEBUG_warnig(tag, format, ...)
#define DEBUG__error(tag, format, ...)
#endif

// 串口3接收中断初始化
void UART3_Init(void);
// 串口3接收完成回调函数
void UARTE3_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
// 串口3错误回调
void UART3_ErrorCallback(UART_HandleTypeDef *huart);
// 串口3发送
void Usart3DmaPrintf(const char *format, ...);

#endif // !__UART_1_H
