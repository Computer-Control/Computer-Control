/*位置式pid程序
作者：周明德*/
#include  "pid.h"
/*
 *  函数名  :	    pid_init 
 *  参数    :	 	*pid         位置式PID结构体指针
 *                  setpoint        目标值
 *					P	     比例系数
 *					I	     积分系数
 *					D	     微分系数
 *  返回值  :	 	无
 *  说明    :	    位置式PID结构体初始化                                       
 */
void pid_init(pidObj *pid, float setpoint,float P,float I,float D)
{
    pid->setpoint   = setpoint;                                                      
    pid->P = P;                                                   
    pid->I = I;                                                     
    pid->D = D;                                                  
    pid->lasterror  = 0;                                                            
    pid->sumerror   = 0;
}
/*
 *   函数名   :		 pid_calc
 *   参数     :      *pid                计算所用的位置式PID结构体指针
 *                   currentpoint  		 当前采样值
 *   返回值   : 	 输出控制量
 *   说明     ：	 位置式PID计算部分
 *   修改说明 ：     微调代码结构，增加衰减常数，减小整型运算带来的精度误差
 */
float pid_calc(pidObj *pid, float currentpoint)
{
	float derr,err;
	err            = pid->setpoint - currentpoint; ///偏差
	pid->sumerror += err;                          ///积分
	derr           = err - pid->lasterror;         ///当前微分
	pid->lasterror = err;

	return (pid->P * err +              ///比例项
					pid->I * pid->sumerror +    ///积分项
					pid->D * derr);             ///微分项
}
