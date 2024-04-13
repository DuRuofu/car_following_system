#include "encoder.h"
#include <stdio.h>
#include "uart_1.h"

extern int32_t car_speed_1 = 0; // 电机1速度
extern int32_t car_speed_2 = 0; // 电机2速度

//extern int32_t car_speed1;
//extern int32_t car_speed2;

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
// 读取编码器速度
    car_speed_1 = Encoder_Value(1);
    car_speed_2 = Encoder_Value(2);

    // 放大10倍
    // car_speed_1 = car_speed_1*10;
    // car_speed_2 = car_speed_2*10;

	//if(car_speed_1>=65535) car_speed_1=((car_speed_1/10)-65535)*10;
	//if(car_speed_2>=65535) car_speed_2=((car_speed_2/10)-65535)*10;

    // 清零编码器计数值
    Encoder_Count_Clear(1);
    Encoder_Count_Clear(2);

	//printf("car_speed_1:%d\r\n",car_speed_1/10);
	//printf("car_speed_2:%d\r\n",car_speed_2/10);

    // 打印速度值
    //printf("car_speed_1:%d\r\n",car_speed_1);

}


float encoder_speed1(float car_speed_1)
{
	car_speed_1= Encoder_Value(1);
	car_speed_1 = car_speed_1*10;
	if(car_speed_1>=65535) car_speed_1=((car_speed_1/10)-65535)*10;
	Encoder_Count_Clear(1);
	//printf("car_speed_1:%d\r\n",car_speed_1);
	return car_speed_1;
	
}

float encoder_speed2(float car_speed_2)
{
	car_speed_2= Encoder_Value(1);
	car_speed_2 = car_speed_1*10;
	if(car_speed_2>=65535) car_speed_2=((car_speed_2/10)-65535)*10;
	Encoder_Count_Clear(2);
	//printf("car_speed_2:%d\r\n",car_speed_2);
	return car_speed_2;
	
}
