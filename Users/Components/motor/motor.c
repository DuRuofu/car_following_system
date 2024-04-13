#include "motor.h"
#include <stdlib.h>

int leftMotorPWM;
int rightMotorPWM;

void MOTOR_INIT(void)
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // 车尾对人，右电机
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); // 车尾对人，左电机
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
}

void LeftMotor_Go()
{

	HAL_GPIO_WritePin(CAR_EN_1_GPIO_Port, CAR_EN_1_Pin, GPIO_PIN_RESET);
}
void LeftMotor_Back()
{

	HAL_GPIO_WritePin(CAR_EN_1_GPIO_Port, CAR_EN_1_Pin, GPIO_PIN_SET);
}
void RightMotor_Go()
{

	HAL_GPIO_WritePin(CAR_EN_1_GPIO_Port, CAR_EN_2_Pin, GPIO_PIN_SET);
}
void RightMotor_Back()
{

	HAL_GPIO_WritePin(CAR_EN_1_GPIO_Port, CAR_EN_2_Pin, GPIO_PIN_RESET);
}

void RightMotor_Stop()
{
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
}

void LeftMotor_Stop()
{
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
}

// 控制方向
void MotorControl(char motorDirection, int leftMotorPWM, int rightMotorPWM)
{
	switch (motorDirection)
	{
	case 0:
		while (1)
		{
			LeftMotor_Go(); // 前进
			RightMotor_Go();
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, leftMotorPWM);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, rightMotorPWM);
			//		HAL_Delay(5000);
			//		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
			//		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
			break;
		}
		break;
	case 1:
		LeftMotor_Back(); // 后退
		RightMotor_Back();

		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, leftMotorPWM);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, rightMotorPWM);
		break;
	case 2:
		RightMotor_Go(); // 左转
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, rightMotorPWM);
		break;
	case 3:
		LeftMotor_Go(); // 右转
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, leftMotorPWM);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
		break;
	default:
		break;
	}
}

void motorset(int leftMotorPWM, int rightMotorPWM) // 速度
{
	if (leftMotorPWM >= 0 && rightMotorPWM >= 0) // 直行
	{
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, leftMotorPWM);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, rightMotorPWM);
		LeftMotor_Go();
		RightMotor_Go();
	}
	else if (leftMotorPWM <= 0 && rightMotorPWM >= 0)
	{
		LeftMotor_Back();
		leftMotorPWM = abs(leftMotorPWM);
		RightMotor_Go();
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, leftMotorPWM);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, rightMotorPWM);
	}

	else if (leftMotorPWM >= 0 && rightMotorPWM <= 0)
	{
		LeftMotor_Go();
		rightMotorPWM = abs(rightMotorPWM);
		RightMotor_Back();
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, leftMotorPWM);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, rightMotorPWM);
	}

	else if (leftMotorPWM <= 0 && rightMotorPWM <= 0) // 后退
	{
		LeftMotor_Back();
		RightMotor_Back();
		leftMotorPWM = abs(leftMotorPWM);
		rightMotorPWM = abs(rightMotorPWM);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, leftMotorPWM);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, rightMotorPWM);
	}
}



