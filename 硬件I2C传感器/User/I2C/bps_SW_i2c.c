#include "bps_SW_i2c.h"


void delay_ms(int n)//��ʱ����
{
	int i;
	while (n--)
	{
		for(i=0;i<552;i++);
	}
}

void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

	/* ʹ���� I2C �йص�ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	//��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
    
  /* I2C_SCL��I2C_SDA*/
  GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	       // ��©���
  GPIO_Init(EEPROM_I2C_SCL_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	       // ��©���
  GPIO_Init(EEPROM_I2C_SDA_PORT, &GPIO_InitStructure);	
	
}

void I2C_config(void)
{
	I2C_InitTypeDef I2C_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	//����I2C
	I2C_InitStruct.I2C_ClockSpeed=400000;//���ô�������
	I2C_InitStruct.I2C_Mode=I2C_Mode_I2C;//����ģʽ
	I2C_InitStruct.I2C_DutyCycle=I2C_DutyCycle_2 ;//����ʱ��ռ�ձ�
	I2C_InitStruct.I2C_OwnAddress1=0x01;//����stm32�Լ��ĵ�ַ
	I2C_InitStruct.I2C_Ack=I2C_Ack_Enable;//Ӧ��ʹ��
	I2C_InitStruct.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;//ʹ����λ��ַģʽ
	I2C_Init(I2C1,&I2C_InitStruct);
	
	I2C_Cmd(I2C1,ENABLE);
}

void start(void)
{
	SDA(1);
	SCL(1);
	delay_ms(1);
	SDA(0);
	delay_ms(1);
	SCL(0);
}

void stop(void)
{
	SCL(1);
	delay_ms(1);
	SDA(0);
	delay_ms(1);
	SDA(1);
}


void I2C_sentdata(uint8_t byte)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		if((byte&(0x80>>i))!=0)
		{
			SDA(1);
		}
		else
		{
			SDA(0);
		}
		SCL(1);
		delay_ms(1);
		SCL(0);
	}
	SDA(1);//�ͷ�����
}

uint8_t I2C_receivedata(void)
{
	
	uint8_t Data=0x00;
	uint8_t i=0;
	SDA(1);//�ͷ�����
	delay_ms(2);
	for(i=0;i<8;i++)
	{
		SCL(1);
		if(((GPIOB->IDR&GPIO_Pin_7)==1))
		{
			Data=Data|(0x80>>i);
		}
		delay_ms(2);
		SCL(0);
	}
	return Data;
	
//	uint8_t i;
//	uint8_t value;

//	/* ������1��bitΪ���ݵ�bit7 */
//	value = 0;
//	for (i = 0; i < 8; i++)
//	{
//		value <<= 1;
//		SCL(1);
//		delay_ms(1);
//		if ((GPIOB->IDR&GPIO_Pin_7))
//		{
//			value++;
//		}
//		SCL(0);
//		delay_ms(1);
//	}
//	return value;

	
}

void I2C_sentACK(uint8_t ACK)
{
	if(ACK==1)
	{
		SDA(0);
	}
	else
	{
		SDA(1);
	}
	SCL(1);
	delay_ms(2);
	SCL(0);
	SDA(1);
}


uint8_t I2C_receiveACK(void)//����ֵΪ0��ʾ�յ�Ӧ��
{
	SDA(1);
	delay_ms(2);
	SCL(1);
	delay_ms(2);
	if((GPIOB->IDR&0x0080)!=0)
	{
		SCL(0);
		return 1;
	}
	
	delay_ms(2);
	SCL(0);
	return 0;
}

void I2C_Wbyte(uint8_t addr,uint8_t byte)
{
	start();
	I2C_sentdata(0XA0);//д���豸��ַ
	while(I2C_receiveACK());//�ȴ��ӻ�����Ӧ��
	
	I2C_sentdata(addr);//д����Ҫд��ĵ�ַ
	while(I2C_receiveACK());//�ȴ��ӻ�����Ӧ��
	
	I2C_sentdata(byte);//д������
	while(I2C_receiveACK());//�ȴ��ӻ�����Ӧ��
	
	stop();
	
}

uint8_t I2C_Rbyte(uint8_t addr)
{
	uint8_t Data;
	start();
	I2C_sentdata(0XA0);//д���豸��ַ
	while(I2C_receiveACK());//�ȴ��ӻ�����Ӧ��
	
	I2C_sentdata(addr);//д����Ҫ��ȡ�ĵ�ַ
	while(I2C_receiveACK());//�ȴ��ӻ�����Ӧ��
	
	start();
	
	I2C_sentdata(0XA0+1);//д���豸��ַ
	while(I2C_receiveACK());//�ȴ��ӻ�����Ӧ��
	
	I2C_receivedata();
	Data=I2C_receivedata();
	
	I2C_sentACK(0);
	stop();
	return Data;
}
