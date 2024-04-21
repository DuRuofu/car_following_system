#include "pid.h"
#include "main.h"
#include "uart_1.h"



// extern int32_t Car_Speed_A;    // 电机实际速度（电机A）
// extern int32_t Car_Speed_B;    // 电机实际速度（电机B）

extern int32_t car_speed_1; // 电机1速度
extern int32_t car_speed_2; // 电机2速度

extern int32_t car_location_1 ;
extern int32_t car_location_2 ;
extern int32_t car_speed_sum_A; 
extern int32_t car_speed_sum_B;

float PWM_Output_A = 0;
float PWM_Output_B = 0;

#define MOTOR_PWM_MAX 1000000
#define MOTOR_SPEED_MAX 88
//--------------------------速度环参考系---------------------
//---------------速度环(PID)----------------内环
extern float Target_Speed = 0; // 目标速度
extern float Target_Speed_A = 0; 
extern float Target_Speed_B = 0; 
float Kp_Speed = 35;      // 速度环比例系数  135
float Ki_Speed = 0.5;        // 速度环积分系数  0.5
float Kd_Speed = 0;      // 速度环微分系数   1



//--------------------------位置环参考系---------------------
//---------------位置环(PID)----------------外环
int32_t Target_Location=0;
float Kp_Location =0.15;      // 速度环比例系数  0.15
float Ki_Location = 0.08;        // 速度环积分系数   0.08
float Kd_Location = 0;      // 速度环微分系数    0




/**
 * @description: 电机A速度环
 * @param {int32_t} target_speed
 * @param {int32_t} fact_speed
 * @return {*}
 */
int32_t PID_Speed_A(float target_speed, float fact_speed)
{
    static int32_t Speed_PWM_Out = 0, Err_Speed = 0, L_Speed_Err = 0, L_L_Speed_Err = 0, Err_Speed_Sum = 0; // 速度环输出

    // 速度误差
    Err_Speed = target_speed - fact_speed; // 计算速度误差

    // 死区限制
    if (Err_Speed > -2 && Err_Speed < 2)  //2
    {
        Err_Speed = 0;
    }

    // PID计算(增量)
    Speed_PWM_Out =Speed_PWM_Out +  Kp_Speed * (Err_Speed - L_Speed_Err) + Ki_Speed * Err_Speed + Kd_Speed * (Err_Speed - 2 * L_Speed_Err + L_L_Speed_Err);
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
int32_t PID_Speed_B(float target_speed, float fact_speed)
{
    static int32_t Speed_PWM_Out = 0, Err_Speed = 0, L_Speed_Err = 0, L_L_Speed_Err = 0; // 速度环输出
                                                                                         // 计算速度误差
    Err_Speed = target_speed - fact_speed;  
    
    // 消抖                                            
     if (Err_Speed > -2 && Err_Speed < 2)  //2
    {
        Err_Speed = 0;
    }

    // PID计算(增量)
    Speed_PWM_Out =Speed_PWM_Out +  Kp_Speed * (Err_Speed - L_Speed_Err) + Ki_Speed * Err_Speed + Kd_Speed * (Err_Speed - 2 * L_Speed_Err + L_L_Speed_Err);
    
    // 调参用
    // Speed_PWM_Out = Speed_PWM_Out +  Kp * (Err_Speed-L_Speed_Err) + Ki * Err_Speed + Kd * (Err_Speed - 2.0*L_Speed_Err + L_L_Speed_Err);
    //限幅
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
 * @description: 电机A位置环
 * @param {int32_t} target_Location
 * @param {int32_t} fact_Location
 * @return {*}
 */
int32_t PID_Location_A(int32_t target_Location, int32_t fact_Location)
{
    static int32_t Location_PWM_Out = 0, Err_Location = 0, L_Location_Err = 0, L_L_Location_Err = 0, Err_Location_Sum = 0; // 位置环输出

    // 位置误差
    Err_Location = target_Location - fact_Location; // 计算位置误差

    // 死区限制
    if (Err_Location > -3 && Err_Location < 3)  //2
    {
        Err_Location = 0;
    }

    // PID计算(增量)
    Location_PWM_Out = Location_PWM_Out + Kp_Location * (Err_Location - L_Location_Err) + Ki_Location* Err_Location + Kd_Location * (Err_Location - 2 * L_Location_Err + L_L_Location_Err);   
		

    // 限幅
   if (Location_PWM_Out > MOTOR_SPEED_MAX)
   {
       Location_PWM_Out = MOTOR_SPEED_MAX;
   }
   else if (Location_PWM_Out < -MOTOR_SPEED_MAX)
   {
       Location_PWM_Out = -MOTOR_SPEED_MAX;
   }
    // 保存误差
    L_Location_Err = Err_Location;     // 保存上一次的误差
    L_L_Location_Err = L_Location_Err; // 保存上上次的误差
    return Location_PWM_Out;
}

/**
 * @description: 电机B位置环
 * @param {int32_t} target_speed
 * @param {int32_t} fact_speed
 * @return {*}
 */
int32_t PID_Location_B(int32_t target_Location, int32_t fact_Location)
{
    static int32_t Location_PWM_Out = 0, Err_Location = 0, L_Location_Err = 0, L_L_Location_Err = 0, Err_Location_Sum = 0; // 位置环输出

    // 位置误差
    Err_Location = target_Location - fact_Location; // 计算位置误差

    // 死区限制
    if (Err_Location > -3 && Err_Location < 3)  //2
    {
        Err_Location = 0;
    }

    // PID计算(增量)
    Location_PWM_Out = Location_PWM_Out + Kp_Location * (Err_Location - L_Location_Err) + Ki_Location* Err_Location + Kd_Location * (Err_Location - 2 * L_Location_Err + L_L_Location_Err);   
		

    // 限幅
   if (Location_PWM_Out > MOTOR_SPEED_MAX)
   {
       Location_PWM_Out = MOTOR_SPEED_MAX;
   }
   else if (Location_PWM_Out < -MOTOR_SPEED_MAX)
   {
       Location_PWM_Out = -MOTOR_SPEED_MAX;
   }
    // 保存误差
    L_Location_Err = Err_Location;     // 保存上一次的误差
    L_L_Location_Err = L_Location_Err; // 保存上上次的误差
    return Location_PWM_Out;
}










/**
 * @description: 小车PID总输出 位置环+速度环
 * @return {*}
 */
void Car_PID_Ctrl(void)
{
	
		//位置环
		int32_t PWM_Out_Location_A = PID_Location_A(Target_Location, car_location_1);
		int32_t PWM_Out_Location_B = PID_Location_B(Target_Location, car_location_2);
	
//	  DEBUG_printf("pid","PWM_Out_Location_A:%d ,PWM_Out_Location_B:%d",PWM_Out_Location_A,PWM_Out_Location_B);
	  
		
    // 速度环
		int32_t PWM_Out_Speed_A=PID_Speed_A(PWM_Out_Location_A, car_speed_1);
		int32_t PWM_Out_Speed_B=PID_Speed_B(PWM_Out_Location_B, car_speed_2);

    int32_t PWM_Output_A = PWM_Out_Speed_A;
    int32_t PWM_Output_B = PWM_Out_Speed_B;


    // 防止0速噪音
//    if (PWM_Output_A<= 1 && PWM_Output_A>= -1)
//    {
//        PWM_Output_A = 0;
//    }

//    if (PWM_Output_B <= 1 && PWM_Output_B>= -1)
//    {
//        PWM_Output_B = 0;
//    }

    // 限幅
    if (PWM_Output_A >= MOTOR_PWM_MAX)
    {
        PWM_Output_A = MOTOR_PWM_MAX;
    }
    else if (PWM_Output_A <= -MOTOR_PWM_MAX)
    {
        PWM_Output_A = -MOTOR_PWM_MAX;
    }
    if (PWM_Output_B >= MOTOR_PWM_MAX)
    {
        PWM_Output_B = MOTOR_PWM_MAX;
    }
    else if (PWM_Output_B <= -MOTOR_PWM_MAX)
    {
        PWM_Output_B = -MOTOR_PWM_MAX;
    }
    // 作用到电机
    motorset(PWM_Output_A, PWM_Output_B);
}






/**
 * @description: 小车PID总输出 速度环
 * @return {*}
 */
void Car_PID_Ctrl_Speed(void)
{
		
    // 速度环
    float PWM_Out_Speed_A = PID_Speed_A(Target_Speed, car_speed_1);
    float PWM_Out_Speed_B = PID_Speed_B(Target_Speed, car_speed_2);;
//		DEBUG_printf("pid","PWM_Out_Speed_A:%d ,PWM_Out_Speed_B:%d",PWM_Out_Speed_A,PWM_Out_Speed_B);


    float PWM_Output_A = PWM_Out_Speed_A;
    float PWM_Output_B = PWM_Out_Speed_B;


    // 防止0速噪音
//    if (PWM_Output_A<= 170 && PWM_Output_A>= -170)
//    {
//        PWM_Output_A = 0;
//    }
//	

//    if (PWM_Output_B<= 170 && PWM_Output_B >= -170)
//    {
//        PWM_Output_B = 0;
//    }

    // 限幅
    if (PWM_Output_A >= MOTOR_PWM_MAX)
    {
        PWM_Output_A = MOTOR_PWM_MAX;
    }
    else if (PWM_Output_A <= -MOTOR_PWM_MAX)
    {
        PWM_Output_A = -MOTOR_PWM_MAX;
    }
    if (PWM_Output_B >= MOTOR_PWM_MAX)
    {
        PWM_Output_B = MOTOR_PWM_MAX;
    }
    else if (PWM_Output_B <= -MOTOR_PWM_MAX)
    {
        PWM_Output_B = -MOTOR_PWM_MAX;
    }
    // 作用到电机
    motorset(PWM_Output_A, PWM_Output_B);
}



/**
 * @description: 小车PID总输出 圆周速度环
 * @return {*}
 */
void Car_PID_Ctrl_Speed_Circle(void)
{
		
    // 速度环
    float PWM_Out_Speed_A = PID_Speed_A(Target_Speed_A, car_speed_1);
    float PWM_Out_Speed_B = PID_Speed_B(Target_Speed_B, car_speed_2);
//		DEBUG_printf("pid","PWM_Out_Speed_A:%d ,PWM_Out_Speed_B:%d",PWM_Out_Speed_A,PWM_Out_Speed_B);


    float PWM_Output_A = PWM_Out_Speed_A;
    float PWM_Output_B = PWM_Out_Speed_B;


    // 防止0速噪音
//    if (PWM_Output_A<= 170 && PWM_Output_A>= -170)
//    {
//        PWM_Output_A = 0;
//    }
//	

//    if (PWM_Output_B<= 170 && PWM_Output_B >= -170)
//    {
//        PWM_Output_B = 0;
//    }

    // 限幅
    if (PWM_Output_A >= MOTOR_PWM_MAX)
    {
        PWM_Output_A = MOTOR_PWM_MAX;
    }
    else if (PWM_Output_A <= -MOTOR_PWM_MAX)
    {
        PWM_Output_A = -MOTOR_PWM_MAX;
    }
    if (PWM_Output_B >= MOTOR_PWM_MAX)
    {
        PWM_Output_B = MOTOR_PWM_MAX;
    }
    else if (PWM_Output_B <= -MOTOR_PWM_MAX)
    {
        PWM_Output_B = -MOTOR_PWM_MAX;
    }
    // 作用到电机
    motorset(PWM_Output_A, PWM_Output_B);
}


