//pid接口函数
//作者：周明德
#ifndef _PID_H_
#define _PID_H_

/*位置式PID*/
typedef struct _PID
{
  float  setpoint;   //设定目标
  float  P;          //比例常数 p 
  float  I;          //积分常数 I 
  float  D;          //微分常数 D 
  float  lasterror;  //Error[-1]
  long long sumerror;   //Sums of Errors
}pidObj;


/*声明外部函数*/
extern void pid_init(pidObj *pid, float setpoint,float P,float I,float D);
extern float pid_calc(pidObj *pid, float nextPoint);

#endif


