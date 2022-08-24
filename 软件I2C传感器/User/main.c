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

short   a[3];//���ڴ�����ٶȵ�ֵ
short   w[3];
float   Temperature[1];
char flag=0;

extern void delay_ms(int n);//��ʱ����



int main()
{
	LED_GPIO_Config();
	Init_ILI9341();
	USART_Config();
	I2C_GPIO_Config();
	MPU6050_Init();
	
	
	//��ʾ����ʼ������
	ILI9341_GramScan(6);//ɨ��ģʽ
	LCD_SetBackColor(BACKGROUND);//����ɫ����Ϊ��ɫ
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);//����ȫΪ����ɫ
	LCD_SetTextColor(BLACK);//ǰ��ɫ����Ϊ��ɫ
	
	
	printf("MPU6050����");
	if(MPU6050ReadID()==0)
	{
		printf("û�м�⵽MPU6050��������\r\n");
		LED_RED;
		while(1);	//��ⲻ��MPU6050 ������Ȼ����
	}
	else
	{
		printf("�ɹ���⵽MPU6050��������\r\n");
		pStr="�ɹ���⵽MPU6050��������";
		//%*c���Ǻ��������ʾ�ж��ٸ�' '���ո�
		//���ܵ����ؼ�ȥ���������ĵ�������ȥ����һ���ֽ���Ҫ�������ٳ���2
		//Ӣ���еĿո��൱��һ��Ӣ���ַ��������൱������Ӣ���ַ��������ֽڣ�
		//����strlen(pStr)����2�ŵõ��ж��ٸ�������
		sprintf((char*)dispBuff,"%*c%s",(LCD_X_LENGTH-(strlen(pStr)/2*WIDTH_CH_CHAR))/(WIDTH_CH_CHAR/2)/2,' ',pStr);
		ILI9341_DispString_EN_CH(0,0+16*2,(char*)dispBuff);
	}
	delay_ms(100000);
	LCD_SetBackColor(BACKGROUND);//����ɫ����Ϊ��ɫ
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);//����ȫΪ����ɫ
  /* ����SysTick��ʱ�����ж� */
  SysTick_Init(); //���� SysTick Ϊ 1ms �ж�һ�Σ����ж����ȡ����������
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //������ʱ��
 while(1)
  {
    if(flag==1) //task_readdata_finish = 1 ��ʾ��ȡMPU6050�������
    {
			
			sprintf((char*)dispBuff,"���ٶȣ�%d %d %d",a[0],a[1],a[2]);
			ILI9341_DispString_EN_CH(10,160,(char*)dispBuff);//��ӡһ������
			
			sprintf((char*)dispBuff,"�����ǣ�%d %d %d",w[0],w[1],w[2]);
			ILI9341_DispString_EN_CH(10,160+16*2,(char*)dispBuff);//��ӡһ������
			
			sprintf((char*)temp_Temperature,"�¶ȣ�%f ",Temperature[0]);
			ILI9341_DispString_EN_CH(10,160+16*4,(char*)temp_Temperature);//��ӡһ������
			
			
			
      printf("���ٶȣ�%8d%8d%8d",a[0],a[1],a[2]);
      
      printf("    ������%8d%8d%8d",w[0],w[1],w[2]);
      
      printf("    �¶�%8.2f\r\n",Temperature[0]);
      
      flag = 0; // �����־λ
    }
  }
	
	
}









