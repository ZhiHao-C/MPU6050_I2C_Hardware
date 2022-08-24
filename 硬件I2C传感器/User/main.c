#include "stm32f10x.h"                  // Device header
#include "bps_led.h"
#include "bps_usart.h"
#include "bps_HW_i2c.h"
#include "bps_mpu6050.h"
#include "bps_systick.h"
#include <stdio.h>
short   a[3];//���ڴ�����ٶȵ�ֵ
short   w[3];
float   Temperature[1];
char flag=0;

extern void delay_ms(int n);//��ʱ����



int main()
{
	LED_GPIO_Config();
	
	USART_Config();
	I2C_Config();
	MPU6050_Init();
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
	}
  /* ����SysTick��ʱ�����ж� */
  SysTick_Init(); //���� SysTick Ϊ 1ms �ж�һ�Σ����ж����ȡ����������
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //������ʱ��
 while(1)
  {
    if(flag==1) //task_readdata_finish = 1 ��ʾ��ȡMPU6050�������
    {
      printf("���ٶȣ�%8d%8d%8d",a[0],a[1],a[2]);
      
      printf("    ������%8d%8d%8d",w[0],w[1],w[2]);
      
      printf("    �¶�%8.2f\r\n",Temperature[0]);
      
      flag = 0; // �����־λ
    }
  }
	
	
}









