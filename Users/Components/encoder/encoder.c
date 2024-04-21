#include "encoder.h"
#include <stdio.h>
#include "uart_1.h"

extern int32_t car_speed_1 = 0; // 电机1速度
extern int32_t car_speed_2 = 0; // 电机2速度
int32_t car_speed_sum_A = 0; 
int32_t car_speed_sum_B = 0;//总转速
extern int32_t Target_Speed;
extern int32_t Target_Location;
extern int32_t car_location_1 = 0;
extern int32_t car_location_2 = 0;

void ENCODER_INIT(void)
{
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    __HAL_TIM_SET_COUNTER(&htim3, 0);
}


//编码器计数初始化
void Encoder_Count_Init(void)
{
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}

// 编码器数据读取
int16_t Encoder_Value(uint8_t n)
{
    if (n == 1)
    {
        return (int16_t)(__HAL_TIM_GET_COUNTER(&htim2));
    }
    else if (n == 2)
    {
        return (int16_t)(__HAL_TIM_GET_COUNTER(&htim3));
    }
    else
    {
        return 0;
    }
}

//编码器清零
void Encoder_Count_Clear(uint8_t n)//看看编码器连的啥，给忘了
{
    if (n == 1)
    {
        __HAL_TIM_SET_COUNTER(&htim2, 0);
    }
    else if (n == 2)
    {
        __HAL_TIM_SET_COUNTER(&htim3, 0);
    }
}

void encoder_speed(void)//car_speed_1
{
	
//		// 清零编码器计数值
//		Encoder_Count_Clear(1);
//		Encoder_Count_Clear(2);
//	
	
	
// 读取编码器速度
    car_speed_1 = Encoder_Value(1);
    car_speed_2 = Encoder_Value(2);
	
		car_speed_sum_A += car_speed_1;
		car_speed_sum_B += car_speed_2;
	
		car_location_1 = car_speed_sum_A;
		car_location_2 = car_speed_sum_B;
	
		car_location_1 = car_location_1;  //左轮转的总圈数  原始值/1100=圈数  K*1100
		car_location_2 = car_location_2;  //右轮转的总圈数
		float c = (float)car_speed_1/car_speed_2;
//		DEBUG_printf("APP","%d,%d,%d",car_location_1,car_location_2,Target_Location);
    // 清零编码器计数值
		Encoder_Count_Clear(1);
		Encoder_Count_Clear(2);
		DEBUG_printf("APP","%d,%d,%f",car_speed_1,car_speed_2,c);

}


void encoder_speed_Sum(void)
{
	car_speed_sum_A = Encoder_Value(1);
	car_speed_sum_A = car_speed_sum_A/1100;
	car_speed_sum_B = Encoder_Value(2);
	car_speed_sum_B = car_speed_sum_B/1100;
//	DEBUG_printf("APP","%d,%d,%d",car_speed_sum_A,car_speed_sum_B,Target_Speed);
	
}


