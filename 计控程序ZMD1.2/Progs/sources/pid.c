//实现增量式PID算法 

#include "stdio.h" 


void pid_init(void);//PID参数初始化 
float pid_realise(float speed);//实现PID算法   

struct {
    float set_speed;//设定速度 
    float actual_speed;//实际速度
    float error;//偏差  
    float error_next;//上一个偏差  
    float error_last;//上上一个偏差 
    float kp,ki,kd;//定义比例，积分，微分参数  
     
}pid;


int pid_add()
{
	  int count = 0;
    pid_init();

    while(count<400)//进行400次 PID 运算，使初始值从0开始接近200.0   
    {
        float speed = pid_realise(200.0);//设定值设定为200.0  
        printf("%f\n",speed);//输出每一次PID 运算后的结果  
        count++;
    }
		return 0;
}


void pid_add_init()
{
    pid.set_speed = 0.0;
    pid.actual_speed = 0.0;
    pid.error = 0.0;
    pid.error_next = 0.0;
    pid.error_last = 0.0;
    //可调节PID 参数。使跟踪曲线慢慢接近阶跃函数200.0 // 
    pid.kp = 0.2;
    pid.ki = 0.01;
    pid.kd = 0.2;
}


float pid_realise(float speed)//实现pid  
{
	  float increment_speed;   //增量
    pid.set_speed = speed;//设置目标速度  
    pid.error = pid.set_speed - pid.actual_speed;

     
    increment_speed = pid.kp*(pid.error-pid.error_next)+pid.ki*pid.error+\
    pid.kd*(pid.error-2*pid.error_next+pid.error_last);//增量计算公式  
    
    pid.actual_speed+= increment_speed;
    pid.error_last = pid.error_next;//下一次迭代  
    pid.error_next = pid.error;
    return pid.actual_speed; 
    
}
