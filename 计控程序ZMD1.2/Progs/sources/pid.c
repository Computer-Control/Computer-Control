//ʵ������ʽPID�㷨 

#include "stdio.h" 


void pid_init(void);//PID������ʼ�� 
float pid_realise(float speed);//ʵ��PID�㷨   

struct {
    float set_speed;//�趨�ٶ� 
    float actual_speed;//ʵ���ٶ�
    float error;//ƫ��  
    float error_next;//��һ��ƫ��  
    float error_last;//����һ��ƫ�� 
    float kp,ki,kd;//������������֣�΢�ֲ���  
     
}pid;


int pid_add()
{
	  int count = 0;
    pid_init();

    while(count<400)//����400�� PID ���㣬ʹ��ʼֵ��0��ʼ�ӽ�200.0   
    {
        float speed = pid_realise(200.0);//�趨ֵ�趨Ϊ200.0  
        printf("%f\n",speed);//���ÿһ��PID �����Ľ��  
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
    //�ɵ���PID ������ʹ�������������ӽ���Ծ����200.0 // 
    pid.kp = 0.2;
    pid.ki = 0.01;
    pid.kd = 0.2;
}


float pid_realise(float speed)//ʵ��pid  
{
	  float increment_speed;   //����
    pid.set_speed = speed;//����Ŀ���ٶ�  
    pid.error = pid.set_speed - pid.actual_speed;

     
    increment_speed = pid.kp*(pid.error-pid.error_next)+pid.ki*pid.error+\
    pid.kd*(pid.error-2*pid.error_next+pid.error_last);//�������㹫ʽ  
    
    pid.actual_speed+= increment_speed;
    pid.error_last = pid.error_next;//��һ�ε���  
    pid.error_next = pid.error;
    return pid.actual_speed; 
    
}
