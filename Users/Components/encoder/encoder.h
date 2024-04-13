#ifndef _ENCODER_H_
#define _ENCODER_H_
#include "main.h"
#include "tim.h"
//int32_t car_speed_1 = 0; // 电机1速度
//int32_t car_speed_2 = 0; // 电机2速度
void ENCODER_INIT(void);
//void Encodercount();

int16_t Encoder_Value(uint8_t n);
void Encoder_Count_Clear(uint8_t n);
void encoder_speed(void);
float encoder_speed1(float car_speed_1);
float encoder_speed2(float car_speed_2);
#endif
