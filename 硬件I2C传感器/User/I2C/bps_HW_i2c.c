#include "bps_HW_i2c.h"
#include "bps_mpu6050.h"

void I2C_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;
	//��APB1�µ�I2C1ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	//��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//����I2C
	I2C_InitStruct.I2C_ClockSpeed=400000;//���ô�������
	I2C_InitStruct.I2C_Mode=I2C_Mode_I2C;//����ģʽ
	I2C_InitStruct.I2C_DutyCycle=I2C_DutyCycle_2 ;//����ʱ��ռ�ձ�
	I2C_InitStruct.I2C_OwnAddress1=I2C_OWN_ADDRESS;//����stm32�Լ��ĵ�ַ
	I2C_InitStruct.I2C_Ack=I2C_Ack_Enable;//Ӧ��ʹ��
	I2C_InitStruct.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;//ʹ����λ��ַģʽ
	I2C_Init(I2C1,&I2C_InitStruct);
	//����GPIO
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//ʹ��I2C
	I2C_Cmd(I2C1,ENABLE);
}



void I2C_WriteByte(uint8_t addr,uint8_t Data)
{
	//������ʼ��־
	I2C_GenerateSTART(I2C1,ENABLE);
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	//���ʹӻ���ַ
	I2C_Send7bitAddress(I2C1,MPU6050_SLAVE_ADDRESS,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	//����һ�����ݣ�Ҫ�ڴӻ��ĸ�λ��д���ݵĵ�ַ��
	I2C_SendData(I2C1,addr);
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING)==ERROR);
	//����д�������
	I2C_SendData(I2C1,Data);
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	//����ֹͣ��־
	I2C_GenerateSTOP(I2C1,ENABLE);
}

//void I2C_Write_Page_Byte(uint8_t addr,uint8_t *Data,uint8_t n)//������д��addr��������addr%8=0
//{
//	uint8_t i;
//	//������ʼ��־
//	I2C_GenerateSTART(I2C1,ENABLE);
//	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);//�ж�EV�¼��ȴ�׼�����
//	//���ʹӻ���ַ
//	I2C_Send7bitAddress(I2C1,EEPROM_ADDR_W,I2C_Direction_Transmitter);
//	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
//	//����һ�����ݣ�Ҫ�ڴӻ��ĸ�λ��д���ݵĵ�ַ��
//	I2C_SendData(I2C1,addr);
//	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING)==ERROR);
//	//����д�������
//	for(i=0;i<n;i++)
//	{
//		I2C_SendData(I2C1,*(Data+i));
//		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
//	}
//	
//	//����ֹͣ��־
//	I2C_GenerateSTOP(I2C1,ENABLE);
//}


//������ȡn������
void I2C_ReadByte(uint8_t addr,uint8_t *Data,uint8_t n)
{
	//�ȴ����߲�æ
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	//������ʼ��־
	I2C_GenerateSTART(I2C1,ENABLE);
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	//���ʹӻ���ַ
	I2C_Send7bitAddress(I2C1,MPU6050_SLAVE_ADDRESS,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	
	I2C_Cmd(I2C1, ENABLE);
	//����һ�����ݣ�Ҫ�ڴӻ��ĸ�λ�ö����ݵĵ�ַ��
	I2C_SendData(I2C1,addr);
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING)==ERROR);
	//�ڶ��η�����ʼ��־
	I2C_GenerateSTART(I2C1,ENABLE);
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	//���ʹӻ���ַ������
	I2C_Send7bitAddress(I2C1,MPU6050_SLAVE_ADDRESS,I2C_Direction_Receiver);
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==ERROR);
	//��ȡһ���ֽ�����
	while(n)  
  {
    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))//�ж����ݼĴ����Ƿ�Ϊ�գ���Ϊ��ʱ��ȡ����
    {      
      /* Read a byte from the slave */
      *Data = I2C_ReceiveData(I2C1);

      /* Point to the next location where the byte read will be saved */
      Data++; 
      
      /* Decrement the read bytes counter */
      n--;        
    }
    if(n == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C1, DISABLE);//���һ�����ݴӻ�ͨ��IC2��������ʱ������Ҫ��һ����Ӧ��
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C1, ENABLE);//ֹͣ��־
    }		
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C1, ENABLE);
}


//	for(i=0;i<n;i++)
//	{
//		if(i==n-1)//���һ�����ݴӻ�ͨ��IC2��������ʱ������Ҫ��һ����Ӧ��
//		{
//			*(Data+i)=I2C_ReceiveData(I2C1);
//			I2C_AcknowledgeConfig(I2C1,DISABLE);
//			
//		}
//		else
//		{
//			*(Data+i)=I2C_ReceiveData(I2C1);
//		}
//		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)==ERROR);//�ж����ݼĴ����Ƿ�Ϊ�գ���Ϊ��ʱ��ȡ����
//	}
//	I2C_GenerateSTOP(I2C1,ENABLE);//ֹͣ��־
//	I2C_AcknowledgeConfig(I2C1,ENABLE);
	
