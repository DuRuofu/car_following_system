/*
 * @Author: DuRuofu duruofu@qq.com
 * @Date: 2023-08-02 12-02-17
 * @LastEditors: DuRuofu
 * @LastEditTime: 2024-04-13 16-07-44
 * @FilePath: \CarFollowingSystem-00\Users\Components\key\key.c
 * @Description: 键盘扫描
 * Copyright (c) 2023 by duruofu@foxmail.com All Rights Reserved.
 */

#include "key.h"
#include "led.h"
#include "app.h"
#include "buzzer.h"

#define KEY1_Pin KEY_1_Pin
#define KEY2_Pin KEY_2_Pin
#define KEY3_Pin KEY_3_Pin
#define KEY4_Pin KEY_4_Pin
#define KEY5_Pin KEY_5_Pin
#define DEBOUNCE_DELAY 250 // 设置消抖延时为200毫秒

// 题目标志位
extern uint8_t Problem_Flag;

// 急停标志位
uint8_t Stop_Flag;

// 目标速度
extern int32_t Target_Speed;

/**
 * @description: 按键初始化 (使用CubeMX自动生成的宏定义，就不用写这个函数了)
 * @return {*}
 */
void Key_Init(void)
{
}

// 按键1 题目切换按钮
void Key_1_Callback(void)
{
    // Led_Toggle();
    // 更换题目
    if (Problem_Flag < 7)
    {
        Problem_Flag++;
    }
    else
    {
        Problem_Flag = 0;
    }

    //Target_Speed = 0;
    Buzzer_ShortBeep();
}

// 按键2  
void Key_2_Callback(void)
{
		Problem_Flag = 2;

    Buzzer_ShortBeep();
}

// 按键3  
void Key_3_Callback(void)
{
		Problem_Flag = 3;
    Buzzer_ShortBeep();
}
// 按键4
void Key_4_Callback(void)
{
		Problem_Flag = 4;
    Buzzer_ShortBeep();
}
// 按键5
void Key_5_Callback(void)
{
		Problem_Flag = 5;
		Buzzer_ShortBeep();
}

// 按键6
void Key_6_Callback(void)
{
		Problem_Flag = 6;
		Buzzer_ShortBeep();
}


/**
 * @description: 按键检测，外部中断回调函数
 * @param {uint16_t} GPIO_Pin
 * @return {*}
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

    /* Prevent unused argument(s) compilation warning */
    UNUSED(GPIO_Pin);
    /* NOTE: This function Should not be modified, when the callback is needed,
            the HAL_GPIO_EXTI_Callback could be implemented in the user file
    */
    // 按键按下
    if (GPIO_Pin == KEY1_Pin)
    {
        // printf("按键1");
        Debounce(GPIO_Pin, Key_1_Callback);
    }
    else if (GPIO_Pin == KEY2_Pin)
    {
        // 按键2按下的处理代码
        Debounce(GPIO_Pin, Key_2_Callback);
    }
    else if (GPIO_Pin == KEY3_Pin)
    {

        // 按键3按下的处理代码
        Debounce(GPIO_Pin, Key_3_Callback);
    }
    else if (GPIO_Pin == KEY4_Pin)
    {
        // 按键4按下的处理代码
        Debounce(GPIO_Pin, Key_4_Callback);
    }
		else if (GPIO_Pin == KEY5_Pin)
    {
        // 按键5按下的处理代码
        Debounce(GPIO_Pin, Key_5_Callback);
    }
}

// 通用的按键消抖函数
void Debounce(uint16_t GPIO_Pin, void (*callback)(void))
{
    static uint32_t lastTriggerTime = 0;
    uint32_t currentTime = HAL_GetTick(); // 获取当前时间戳

    if (currentTime - lastTriggerTime >= DEBOUNCE_DELAY)
    {
        // Buzzer_ShortBeep();
        callback();                    // 调用传入的回调函数
        lastTriggerTime = currentTime; // 更新上一次触发的时间戳
    }
}
