#ifndef __KEY_H
#define __KEY_H

#include "main.h"
void Debounce(uint16_t GPIO_Pin, void (*callback)(void));
void Key_Init(void);
void Key_5_Callback(void);
void Key_6_Callback(void);
#endif // !__LED_H



