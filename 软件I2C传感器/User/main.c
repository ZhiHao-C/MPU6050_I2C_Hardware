#include "stm32f10x.h"                  // Device header
#include "bps_led.h"
#include "bps_usart.h"
#include "bps_HW_i2c.h"
#include "bps_SW_i2c.h"
#include "bps_mpu6050.h"
#include "bps_systick.h"
#include "bps_ili9341_lcd.h"
#include "bps_Font.h" 
#include <stdio.h>
#include <string.h>

short dispBuff[100];
char* pStr;
float temp_Temperature[10];

short   a[3];//用于储存加速度的值
short   w[3];
float   Temperature[1];
char flag=0;

extern void delay_ms(int n);//延时函数



int main()
{
	LED_GPIO_Config();
	Init_ILI9341();
	USART_Config();
	I2C_GPIO_Config();
	MPU6050_Init();
	
	
	//显示屏初始化配置
	ILI9341_GramScan(6);//扫描模式
	LCD_SetBackColor(BACKGROUND);//背景色设置为白色
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);//清屏全为背景色
	LCD_SetTextColor(BLACK);//前景色设置为黑色
	
	
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
		pStr="成功检测到MPU6050传感器！";
		//%*c带星号在这里表示有多少个' '（空格）
		//用总的像素减去中文所消耗的像素再去除以一个字节所要的像素再除以2
		//英文中的空格相当于一个英文字符，中文相当于两个英文字符（两个字节）
		//所以strlen(pStr)除以2才得到有多少个中文字
		sprintf((char*)dispBuff,"%*c%s",(LCD_X_LENGTH-(strlen(pStr)/2*WIDTH_CH_CHAR))/(WIDTH_CH_CHAR/2)/2,' ',pStr);
		ILI9341_DispString_EN_CH(0,0+16*2,(char*)dispBuff);
	}
	delay_ms(100000);
	LCD_SetBackColor(BACKGROUND);//背景色设置为白色
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);//清屏全为背景色
  /* 配置SysTick定时器和中断 */
  SysTick_Init(); //配置 SysTick 为 1ms 中断一次，在中断里读取传感器数据
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //启动定时器
 while(1)
  {
    if(flag==1) //task_readdata_finish = 1 表示读取MPU6050数据完成
    {
			
			sprintf((char*)dispBuff,"加速度：%d %d %d",a[0],a[1],a[2]);
			ILI9341_DispString_EN_CH(10,160,(char*)dispBuff);//打印一个变量
			
			sprintf((char*)dispBuff,"陀螺仪：%d %d %d",w[0],w[1],w[2]);
			ILI9341_DispString_EN_CH(10,160+16*2,(char*)dispBuff);//打印一个变量
			
			sprintf((char*)temp_Temperature,"温度：%f ",Temperature[0]);
			ILI9341_DispString_EN_CH(10,160+16*4,(char*)temp_Temperature);//打印一个变量
			
			
			
      printf("加速度：%8d%8d%8d",a[0],a[1],a[2]);
      
      printf("    陀螺仪%8d%8d%8d",w[0],w[1],w[2]);
      
      printf("    温度%8.2f\r\n",Temperature[0]);
      
      flag = 0; // 清零标志位
    }
  }
	
	
}









