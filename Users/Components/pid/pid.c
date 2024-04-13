#include "pid.h"
#include "main.h"
#include "uart_1.h"
int err_sum = 0;
float kp = 1.9;
float ki = 0;

float PI_realize(float actual_val, float target_val) // 作为速度环
{
    int err;
    err = target_val - actual_val;
    err_sum += err;
    actual_val = kp * err + ki * err_sum;
    return actual_val;
}

//---------------速度环(PID)----------------
extern int32_t Target_Speed = 40; // 目标速度
float Kp_Speed = 10;      // 速度环比例系数
float Ki_Speed = 0.0;        // 速度环积分系数
int32_t Kd_Speed = 0.5;      // 速度环微分系数

// extern int32_t Car_Speed_A;    // 电机实际速度（电机A）
// extern int32_t Car_Speed_B;    // 电机实际速度（电机B）

extern int32_t car_speed_1; // 电机1速度
extern int32_t car_speed_2; // 电机2速度


#define MOTOR_PWM_MAX 1000

/**
 * @description: 电机A速度环
 * @param {int32_t} target_speed
 * @param {int32_t} fact_speed
 * @return {*}
 */
int32_t PID_Speed_A(int32_t target_speed, int32_t fact_speed)
{
    static int32_t Speed_PWM_Out = 0, Err_Speed = 0, L_Speed_Err = 0, L_L_Speed_Err = 0, Err_Speed_Sum = 0; // 速度环输出
    
    // 速度误差
    Err_Speed = target_speed - fact_speed;                                                                  // 计算速度误差

    // 死区限制
    if (Err_Speed > -2 && Err_Speed < 2)
    {
        Err_Speed = 0;
    }

    // PID计算(增量)
    Speed_PWM_Out = Speed_PWM_Out + Kp_Speed * (Err_Speed - L_Speed_Err) + Ki_Speed * Err_Speed + Kd_Speed * (Err_Speed - 2 * L_Speed_Err + L_L_Speed_Err);
    // 调参用
    // Speed_PWM_Out = Speed_PWM_Out +  Kp * (Err_Speed-L_Speed_Err) + Ki * Err_Speed + Kd * (Err_Speed - 2.0*L_Speed_Err + L_L_Speed_Err);
    //		Err_Speed_Sum+=Err_Speed;
    //		Speed_PWM_Out = Kp_Speed * (Err_Speed ) + Ki_Speed * Err_Speed_Sum ;
    // 限幅
    if (Speed_PWM_Out > MOTOR_PWM_MAX)
    {
        Speed_PWM_Out = MOTOR_PWM_MAX;
    }
    else if (Speed_PWM_Out < -MOTOR_PWM_MAX)
    {
        Speed_PWM_Out = -MOTOR_PWM_MAX;
    }
    // 保存误差
    L_Speed_Err = Err_Speed;     // 保存上一次的误差
    L_L_Speed_Err = L_Speed_Err; // 保存上上次的误差
    return Speed_PWM_Out;
}

/**
 * @description: 电机B速度环
 * @param {int32_t} target_speed
 * @param {int32_t} fact_speed
 * @return {*}
 */
int32_t PID_Speed_B(int32_t target_speed, int32_t fact_speed)
{
    static int32_t Speed_PWM_Out = 0, Err_Speed = 0, L_Speed_Err = 0, L_L_Speed_Err = 0; // 速度环输出
                                                                                         // 计算速度误差
    Err_Speed = target_speed - fact_speed;  
    
    // 消抖                                            
    if (Err_Speed > -2 && Err_Speed < 2)
    {
        Err_Speed = 0;
    }

    // PID计算(增量)
    Speed_PWM_Out = Speed_PWM_Out + Kp_Speed * (Err_Speed - L_Speed_Err) + Ki_Speed * Err_Speed + Kd_Speed * (Err_Speed - 2 * L_Speed_Err + L_L_Speed_Err);
    // 调参用
    // Speed_PWM_Out = Speed_PWM_Out +  Kp * (Err_Speed-L_Speed_Err) + Ki * Err_Speed + Kd * (Err_Speed - 2.0*L_Speed_Err + L_L_Speed_Err);
    // 限幅
    if (Speed_PWM_Out > MOTOR_PWM_MAX)
    {
        Speed_PWM_Out = MOTOR_PWM_MAX;
    }
    else if (Speed_PWM_Out < -MOTOR_PWM_MAX)
    {
        Speed_PWM_Out = -MOTOR_PWM_MAX;
    }
    // 保存误差
    L_Speed_Err = Err_Speed;     // 保存上一次的误差
    L_L_Speed_Err = L_Speed_Err; // 保存上上次的误差
    return Speed_PWM_Out;
}

/**
 * @description: 小车PID总输出
 * @return {*}
 */
void Car_PID_Ctrl(void)
{
    // 速度环
    int32_t PWM_Out_Speed_A = PID_Speed_A(Target_Speed, car_speed_1);
    int32_t PWM_Out_Speed_B = PID_Speed_B(Target_Speed, car_speed_2);

    // 转向环
    // int32_t PWM_Out_Direction = PID_Direction(64, CCD_Zhongzhi);

    // 最终输出PWM
    // int32_t PWM_Output_A = PWM_Out_Speed_A + PWM_Out_Direction;
    // int32_t PWM_Output_B = PWM_Out_Speed_B - PWM_Out_Direction;

    int32_t PWM_Output_A = PWM_Out_Speed_A;
    int32_t PWM_Output_B = PWM_Out_Speed_B;

    // 限幅
    if (PWM_Output_A >= MOTOR_PWM_MAX)
    {
        PWM_Output_A = MOTOR_PWM_MAX;
    }
    else if (PWM_Output_A <= -MOTOR_PWM_MAX)
    {
        PWM_Output_A = -MOTOR_PWM_MAX;
    }
    else if (PWM_Output_B >= MOTOR_PWM_MAX)
    {
        PWM_Output_B = MOTOR_PWM_MAX;
    }
    else if (PWM_Output_B <= -MOTOR_PWM_MAX)
    {
        PWM_Output_B = -MOTOR_PWM_MAX;
    }
    
    DEBUG_info("Target_Speed=%d",Target_Speed);
    HAL_Delay(1);
    // 作用到电机
    motorset(PWM_Output_A, PWM_Output_B);
}
