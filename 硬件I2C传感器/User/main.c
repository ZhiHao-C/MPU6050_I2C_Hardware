#include "stm32f10x.h"                  // Device header
#include "bps_led.h"
#include "bps_usart.h"
#include "bps_HW_i2c.h"
#include "bps_mpu6050.h"
#include "bps_systick.h"
#include <stdio.h>
short   a[3];//用于储存加速度的值
short   w[3];
float   Temperature[1];
char flag=0;

extern void delay_ms(int n);//延时函数



int main()
{
	LED_GPIO_Config();
	
	USART_Config();
	I2C_Config();
	MPU6050_Init();
	printf("MPU6050测试");
	if(MPU6050ReadID()==0)
	{
		printf("没有检测到MPU6050传感器！\r\n");
		LED_RED;
		while(1);	//检测不到MPU6050 会红灯亮然后卡死
	}
	else
	{
		printf("成功检测到MPU6050传感器！\r\n");
	}
  /* 配置SysTick定时器和中断 */
  SysTick_Init(); //配置 SysTick 为 1ms 中断一次，在中断里读取传感器数据
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //启动定时器
 while(1)
  {
    if(flag==1) //task_readdata_finish = 1 表示读取MPU6050数据完成
    {
      printf("加速度：%8d%8d%8d",a[0],a[1],a[2]);
      
      printf("    陀螺仪%8d%8d%8d",w[0],w[1],w[2]);
      
      printf("    温度%8.2f\r\n",Temperature[0]);
      
      flag = 0; // 清零标志位
    }
  }
	
	
}









