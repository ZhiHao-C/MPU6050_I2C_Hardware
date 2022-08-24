#include "bps_mpu6050.h"
#include "bps_HW_i2c.h"
#include "bps_SW_i2c.h"
#include <stdio.h>

/**
  * @brief   д���ݵ�MPU6050�Ĵ���
  * @param   
  * @retval  
  */
void MPU6050_WriteReg (u8 reg_add,u8 reg_dat)
{
	I2C_Wbyte(reg_add,reg_dat); 
}



/**
  * @brief   ��MPU6050�Ĵ�����ȡ����
  * @param   
  * @retval  
  */
void MPU6050_ReadData(uint8_t addr,unsigned char* Read,uint8_t num)
{
	I2C_R_Page(addr,Read,num);
}


/**
  * @brief   ��ʼ��MPU6050оƬ
  * @param   
  * @retval  
  */
void MPU6050_Init(void)
{
  int i=0,j=0;
  //�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬��û����ʱ����ϵ�����ϵ����ݿ��ܻ����
  for(i=0;i<1000;i++)
  {
    for(j=0;j<1000;j++)
    {
      ;
    }
  }
	
	//ͨ���Ե�Դ����Ĵ���1��0x6b����bit7λд1ʵ�ֶ�MPU6050��λ�����øüĴ���Ϊ0x00����MPU6050.
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);
	
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //�����ǲ�����Ϊ1K/(1+0x07)=125Hz
	
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	        //��ͨ�˲����Ľ�ֹƵ��Ϊ1K,����Ϊ5Hz
	
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x01);	  //���ü��ٶȴ�����������2Gģʽ
	
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
}

/**
  * @brief   ��ȡMPU6050��ID
  * @param   
  * @retval  ��������1���쳣����0
  */
uint8_t MPU6050ReadID(void)
{
	uint8_t Re=0;
	MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);
	if(Re==0x68)
	{
		return 1;
	}
	else 
	{
		return 0;
	}
	
}

/**
  * @brief   ��ȡMPU6050�ļ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadAcc(short *accData)
{
	uint8_t temp[6];
	MPU6050_ReadData(MPU6050_ACC_OUT,temp,6);
	accData[0]=(temp[0]<<8)|temp[1];
	accData[1]=(temp[2]<<8)|temp[3];
	accData[2]=(temp[4]<<8)|temp[5];
}

/**
  * @brief   ��ȡMPU6050�ĽǼ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadGyro(short *gyroData)
{
    u8 temp[6];
    MPU6050_ReadData(MPU6050_GYRO_OUT,temp,6);
    gyroData[0] = (temp[0] << 8) | temp[1];
    gyroData[1] = (temp[2] << 8) | temp[3];
    gyroData[2] = (temp[4] << 8) | temp[5];
}

/**
  * @brief   ��ȡMPU6050��ԭʼ�¶�����
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
	u8 temp[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,temp,2);     //��ȡ�¶�ֵ
    *tempData = (temp[0] << 8) | temp[1];
}


/**
  * @brief   ��ȡMPU6050���¶����ݣ�ת�������϶�
  * @param   
  * @retval  
  */
void MPU6050_ConvertTemp(float *Temperature)
{
	short regval;
	short temp[1];
	MPU6050ReadTemp(temp);
	regval=temp[0];
	Temperature[0]= 36.53 + ((float)regval/340.0);
}

