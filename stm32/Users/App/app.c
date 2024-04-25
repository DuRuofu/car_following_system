#include "main.h"
#include "app.h"
#include "oled.h"
#include "key.h"
#include "led.h"
#include "uart_it_config.h"
#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "buzzer.h"
#include "pwm.h"
#include "yuntai.h"
// 红线控制右轮，橙线控制左轮
//  [K210]实时位置原始值
extern char k210_msg[64] = "000,100,100";
// 题目选择标志位
extern uint8_t Problem_Flag = 0;
// 消息接收标志位
extern uint8_t K210_Flag = 1;

uint32_t distance_to_end = 180;

extern int32_t Target_Speed;
extern int32_t Target_Location;
extern float Target_Speed_A; // 左轮目标速度
extern float Target_Speed_B; // 右轮目标速度
//--------------------------K210参考系---------------------

// [K210]实时位置
int x;
int y;
int distance;
// 实时速度
extern int32_t car_speed_1; // 电机1速度
extern int32_t car_speed_2; // 电机2速度

extern int leftMotorPWM;	// PA8
extern int rightMotorPWM; // PA9

extern int32_t PWM_Output_A;
extern int32_t PWM_Output_B;

extern int32_t car_location_1;
extern int32_t car_location_2;

extern int32_t car_speed_sum_A;
extern int32_t car_speed_sum_B; // 总转速
int32_t Target_Location_cm = 0;
uint8_t count = 0;

// OLED屏幕缓冲区
uint8_t str_buff1[64] = {0};
uint8_t str_buff2[64] = {0};
uint8_t str_buff3[64] = {0};
uint8_t str_buff4[64] = {0};
uint8_t str_buff5[64] = {0};
uint8_t str_buff6[64] = {0};

void Problem1(void);
void Problem2(void);
void Problem3(void);
void Problem4(void);

//--------------------------舵机参考系---------------------
// 中心位置处参数
extern uint16_t Centre_A = 1460;
extern uint16_t Centre_B = 1417;
// 舵机A（最值）
extern uint16_t Servo_MAX_A = 2800;
extern uint16_t Servo_MIN_A = 1000;
// 舵机B （最值）
extern uint16_t Servo_MIN_B = 1000;
extern uint16_t Servo_MAX_B = 2000;

// 实时位置
extern uint16_t pwm_A;
extern uint16_t pwm_B;

/**
 * @description: 系统应用初始化
 * @return {*}
 */
void App_Init(void)
{
	// HAL_Delay(1000);

	MOTOR_INIT();
	ENCODER_INIT();
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim1);
	UART_IT_Init(); // 总串口接收初始化
	OLED_Init();		// OLED初始化
	OLED_Clear();
	OLED_ShowString(4, 0, "Car parameter", 16);
	DEBUG_printf("APP", "系统初始化完成~");
	PWM_Init();
	Buzzer_ShortBeep();
	Yuntai_Control();
	HAL_Delay(200);
	yuntai(1500, 1300);
}

void App_Task(void)
{

	// 处理k210数据
	//	if(1 == K210_Flag)
	//	{
	//		// 提取数据
	//		sscanf(k210_msg, "%d,%d,%d", &x, &y, &distance);
	//		DEBUG_printf("APP", "K210-> X:%d Y:%d Distance:%d", x, y, distance);
	//		HAL_Delay(10);

	//		// 清除缓存
	//		memset(k210_msg, 0, sizeof(k210_msg));

	//		// 刷新屏幕
	//		sprintf((char *)str_buff1, "x:%04d y:%04d", x, y);
	//		sprintf((char *)str_buff2, "distance:%04d", distance);

	//		OLED_ShowString(0, 2, str_buff1, 8);
	//		OLED_ShowString(0, 3, str_buff2, 8);
	//		// 清除标志位
	//		K210_Flag = 0;
	//	}

	sprintf((char *)str_buff3, "problem_flag:%02d", Problem_Flag);
	sprintf((char *)str_buff4, "s-1:%04ds-2:%04d", car_speed_1, car_speed_2);
	OLED_ShowString(0, 4, str_buff3, 8);
	OLED_ShowString(0, 5, str_buff4, 8);

	static uint8_t last_Flag = 0;
	// 处理问题
	switch (Problem_Flag)
	{
	case 0:
		last_Flag = Problem_Flag;
	case 1:
		if (last_Flag != Problem_Flag)
		{
			Problem1();
			last_Flag = Problem_Flag;
		}
		break;
	case 2:
		if (last_Flag != Problem_Flag)
		{
			Problem2();
			last_Flag = Problem_Flag;
		}
		break;
	case 3:
		if (last_Flag != Problem_Flag)
		{
			Problem3();
			last_Flag = Problem_Flag;
		}
		break;
	case 4:
		if (last_Flag != Problem_Flag)
		{
			Problem4();
			last_Flag = Problem_Flag;
		}
		break;
	case 5: // 圆周
		if (last_Flag != Problem_Flag)
		{
			Problem5();
			//Problem6();
			last_Flag = Problem_Flag;
		}
		break;
	case 6: // S型
		if (last_Flag != Problem_Flag)
		{
			Problem6();
			last_Flag = Problem_Flag;
		}
		break;
	default:
		break;
	}
}

// 题目1
void Problem1(void)
{
	DEBUG_printf("APP", "Problem1 runing--");
	HAL_Delay(2);
	// 逻辑控制
	//---------now----------
	Target_Location_cm = 165;
}

// 题目2
void Problem2(void)
{
	//	x=0;
	DEBUG_printf("APP", "Problem2 runing--");
	HAL_Delay(2);
	// 逻辑控制
	//---------now----------
	motorset(320, -305); // 原地转圈  （320，-305）
	while (1)
	{
		K210();
		car_speed_sum_A = 0;
		car_speed_sum_B = 0;
		if (250 >= x && 120 <= x)
		{
			if (340 >= x && 20 <= x)
			{
				motorset(0, 0);
				HAL_Delay(300);
				Target_Location_cm = 162;
				while (1)
				{
					DEBUG_printf("6", "6");
					HAL_Delay(10);
				}
				break;
			}
			break;
		}
	}
}

// 题目2-3 270度
void Problem3(void)
{
	DEBUG_printf("APP", "Problem3 runing--");
	HAL_Delay(2);
	// 逻辑控制

	//---------now----------
	motorset(-300, 295);
	while (1)
	{
		K210();
		car_speed_sum_A = 0;
		car_speed_sum_B = 0;
		if (160 >= x && 80 <= x)
		{
			if (340 >= x && 30 <= x)
			{
				motorset(0, 0);
				HAL_Delay(100);
				Target_Location_cm = 162;
				while (1)
				{
					DEBUG_printf("6", "3");

					HAL_Delay(10);
				}
				break;
			}
			break;
		}
	}
}
// 题目3
void Problem4(void)
{
	DEBUG_printf("APP", "Problem4 runing--");
	HAL_Delay(2);
	// 逻辑控制
	//--------------------------now-----------------
	Target_Location_cm = 100; // 直行
	HAL_Delay(2500);
	car_speed_sum_A = 0;
	car_speed_sum_B = 0;
	Target_Location_cm = 0;
	motorset(320, -305); // 转圈寻找
	while (1)
	{
		K210();
		car_speed_sum_A = 0;
		car_speed_sum_B = 0;
		if (260 >= x && 160 <= x)
		{
			if (340 >= x && 30 <= x)
			{
				Target_Location_cm = 65;
				while (1)
				{
					DEBUG_printf("6", "4");

					HAL_Delay(10);
				}
				break;
			}
			break;
		}
	}
}

// 题目提高一 ：圆周运动
void Problem5(void)
{
	DEBUG_printf("APP", "Problem5 runing--");

	HAL_GPIO_WritePin(CAR_EN_1_GPIO_Port, CAR_EN_1_Pin, GPIO_PIN_SET); // 发出指示灯
	int32_t x_location = 0;
	HAL_Delay(2);
	//------------------逻辑控制(前进)----------------
	Target_Location_cm = 175; // 直行距离光源40cm，范围20-50cm
	HAL_Delay(5000);
	Target_Location_cm = 0;
	//------------------逻辑控制(旋转90度)----------------
	motorset(320, -305);
	HAL_Delay(960); // 瞎猜的，具体实验
	motorset(0, 0);
	//------------------逻辑控制(云台旋转)----------------
	HAL_Delay(1560);
	yuntai(2475, 1300);

	// 进行判断识别，具体写逻辑
	while (1)
	{
		// K210();
		car_speed_sum_A = 0;
		car_speed_sum_B = 0;

		// 归中
		while (1)
		{
			K210();
			car_speed_sum_A = 0;
			car_speed_sum_B = 0;
			if (160 < x) // 后退
			{
				Target_Speed_A = 25; // 移动直到光源在中心
				Target_Speed_B = 25;
			}
			if (160 > x && x > 150)
			{
				// 无偏差
				x_location = x;
				Target_Speed_A = 0;
				Target_Speed_B = 0;
				break;
			}
			if (150 > x && x > 20) // 前进
			{
				Target_Speed_A = -25; // 移动直到光源在中心
				Target_Speed_B = -25;
			}
		}

		// 开始跟随
		while (1)
		{
			K210();
			float x_err = (x - x_location) / 3; // 计算一段时间后光源移动的坐标差

			// 差速运动
			Target_Speed_A = 1.29 * x_err;
			Target_Speed_B = x_err;

			// // 光源在边缘
			if (x > 270 || x < 20)
			{
				HAL_GPIO_WritePin(CAR_EN_1_GPIO_Port, CAR_EN_1_Pin, GPIO_PIN_SET); // 熄灭指示灯
			}
			else
			{
				HAL_GPIO_WritePin(CAR_EN_1_GPIO_Port, CAR_EN_1_Pin, GPIO_PIN_RESET); // 熄灭指示灯
			}
		}
	}
}

void Problem6(void) // s型
{
	while (1)
	{
		// 读取K210数据
		K210();

		uint32_t Target_A, Target_B = 0;

		// 调整方向
		if (160 < x) // 后退
		{
			Target_A = -25; // 移动直到光源在中心
			Target_B = 25;
		}
		if (180 > x && x > 130)
		{
			// 无偏差 
			Target_A = 0;
			Target_B = 0;
		}
		if (150 > x && x > 20)
		{
			Target_A = 25; // 移动直到光源在中心
			Target_B = -25;
		}

		// 控制距离
		if (180 < y) // 后退
		{
			Target_A = Target_A - 25; // 移动直到光源在中心
			Target_B = Target_B - 25;
		}
		if (180 > y && y > 70)
		{
			// 无偏差
			Target_A = Target_A + 0;
			Target_B = Target_B + 0;
		}
		if (70 > y && y > 0)
		{
			Target_A = Target_A + 25; // 移动直到光源在中心
			Target_B = Target_B + 25;
		}

		// 控制速度
		Target_Speed_A = Target_A;
		Target_Speed_B = Target_A;
		HAL_Delay(10);
	}
}

int K210(void)
{
	// 处理k210数据
	if (1 == K210_Flag)
	{
		// 提取数据
		sscanf(k210_msg, "%d,%d,%d", &x, &y, &distance);
		DEBUG_printf("APP", "K210-> X:%d Y:%d Distance:%d", x, y, distance);
		HAL_Delay(10);

		// 清除缓存
		memset(k210_msg, 0, sizeof(k210_msg));

		// 刷新屏幕
		sprintf((char *)str_buff1, "x:%04d y:%04d", x, y);
		sprintf((char *)str_buff2, "distance:%04d", distance);

		OLED_ShowString(0, 2, str_buff1, 8);
		OLED_ShowString(0, 3, str_buff2, 8);

		// 清除标志位
		K210_Flag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}
//-------------分割线，以下测试位置环--------------

// 定时器1ms中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // 1ms一次
{
	static uint16_t count = 0, time = 0;
	count++;
	// 20ms执行一次操作
	if (count % 20 == 0)
	{
		// 编码器计数
		encoder_speed();
		Target_Location = (Target_Location_cm / 18.8) * 1496;

		// 电机位置环控制  无问题
		if (1 == Problem_Flag)
		{

			Car_PID_Ctrl();
		}
		// 题目二，现在需要在调用位置环前将转的圈数清零
		if (2 == Problem_Flag)
		{
			if (Target_Location == 0)
			{
			}
			if (250 >= x && 120 <= x)
			{

				Car_PID_Ctrl();
			}
		}

		if (3 == Problem_Flag)
		{
			if (Target_Location == 0)
			{
			}
			if (160 >= x && 80 <= x)
			{
				Car_PID_Ctrl();
			}
		}

		if (4 == Problem_Flag)
		{
			if (0 < Target_Location)
			{
				Car_PID_Ctrl();
			}
			else
				;
			if (140 >= x && 130 <= x)
			{
				Car_PID_Ctrl();
			}
			if (Target_Location == 0)
			{
			}
		}
		// 圆周
		if (5 == Problem_Flag)
		{
			if (0 < Target_Location)
			{
				Car_PID_Ctrl();
			}
			else
				;
			if (x > 10)
			{
				Car_PID_Ctrl_Speed_Circle();
			}
			if (150 < x && x < 160)
			{
				HAL_GPIO_WritePin(CAR_EN_1_GPIO_Port, CAR_EN_1_Pin, GPIO_PIN_RESET); // 发出指示灯
			}
		}
	}
	if (6 == Problem_Flag)
	{
		Car_PID_Ctrl_Speed_Circle();
	}

	// 500ms执行一次操作
	if (count == 500)
	{
		// 心跳灯
		Led_Toggle();
		// 重置计数器
		count = 0;
	}
}

int yuntai(int32_t pwm_A, int32_t pwm_B)
{
	// 参数限幅
	if (pwm_A > 2800)
		pwm_A = Servo_MAX_A;
	if (pwm_A < Servo_MIN_A)
		pwm_A = Servo_MIN_A;
	if (pwm_B > Servo_MAX_B)
		pwm_B = Servo_MAX_B;
	if (pwm_B < Servo_MIN_B)
		pwm_B = Servo_MIN_B;

	// 控制
	Yuntaiz_AB_Move(pwm_A, pwm_B, 1);
}
