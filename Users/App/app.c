#include "main.h"
#include "app.h"
#include "oled.h"
#include "key.h"
#include "led.h"
#include "uart_it_config.h"
#include "motor.h"
#include "encoder.h"
#include "pid.h"

// [K210]实时位置原始值
extern char k210_msg[64]="100,100,100";
//题目选择标志位
extern uint8_t Problem_Flag = 0;
//消息接收标志位
extern uint8_t K210_Flag =1;

uint32_t distance_to_end = 180; 


//--------------------------K210参考系---------------------

// [K210]实时位置 
int x ;
int y ;
int distance;
//实时速度
extern int32_t car_speed_1 ; // 电机1速度
extern int32_t car_speed_2 ; // 电机2速度

extern int leftMotorPWM;//PA8
extern int rightMotorPWM;//PA9

extern Pid pidmotor1speed;
extern Pid pidmotor2speed;

uint8_t count=0;


// OLED屏幕缓冲区
uint8_t str_buff1[64]= {0};
uint8_t str_buff2[64]= {0};
uint8_t str_buff3[64]= {0};
uint8_t str_buff4[64]= {0};   
uint8_t str_buff5[64]= {0};
uint8_t str_buff6[64]= {0};


/**
 * @description: 系统应用初始化
 * @return {*}
 */
void App_Init(void)
{
	//HAL_Delay(1000);
	
	MOTOR_INIT();
	ENCODER_INIT();
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim1);
	UART_IT_Init();	     //总串口接收初始化
	OLED_Init();				 //OLED初始化
	OLED_Clear();
	OLED_ShowString(4,0,"Car parameter",16);
	DEBUG_printf("APP","系统初始化完成~");

}
	
	
void App_Task(void)
{
	//Led_Toggle();
	//HAL_Delay(500);
    
  // 测试串口3发送
  //DEBUG_printf("APP","sys task runing--");
	//HAL_Delay(10);
   

	// 处理k210数据 
	if(1 == K210_Flag)
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
	}

	sprintf((char *)str_buff3, "problem_flag:%02d", Problem_Flag);
	sprintf((char *)str_buff4, "s-1:%04ds-2:%04d", car_speed_1, car_speed_2);
	OLED_ShowString(0, 4, str_buff3, 8);
	OLED_ShowString(0, 5, str_buff4, 8);

	// 测试串口2发送
	// Usart2DmaPrintf("[UART2] sys task runing--\r\n");

}

// 题目1
void Problem1(void)
{

	MotorControl(0, 1, 1);
	if (distance_to_end > 0)
	{
		// Speed control logic based on distance remaining
		car_speed_1 = car_speed_1 * (distance_to_end / 100);
		car_speed_2 = car_speed_2 * (distance_to_end / 100);

		// 更新距离
		distance_to_end -= 1;
	}
}
// 题目2的1
void Problem21(void)
{
	MotorControl(3, 1, 0);
	HAL_Delay(1000);
	MotorControl(0, 1, 1);
	HAL_Delay(1000);
	Problem1();
}
// 题目2的2
void Problem22(void)
{
	MotorControl(3, 1, 0);
	HAL_Delay(2000);
	MotorControl(0, 1, 1);
	HAL_Delay(1000);
	Problem1();
}
// 题目2的3
void Problem23(void)
{
	MotorControl(2, 0, 1);
	HAL_Delay(1000);
	MotorControl(0, 1, 1);
	HAL_Delay(1000);
	Problem1();
}
// 题目3
void Problem3(void)
{
}


// 定时器1ms中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // 1ms一次
{
	static uint16_t count = 0;
	count++;

	// 5ms执行一次操作
	if (count % 5 == 0)
	{
		// 读取编码器速度
		encoder_speed();
		// 电机速度环控制
		Car_PID_Ctrl();
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
