#include "bps_mpu6050.h"
#include "bps_HW_i2c.h"
#include "bps_SW_i2c.h"
#include <stdio.h>

/**
  * @brief   写数据到MPU6050寄存器
  * @param   
  * @retval  
  */
void MPU6050_WriteReg (u8 reg_add,u8 reg_dat)
{
	I2C_Wbyte(reg_add,reg_dat); 
}



/**
  * @brief   从MPU6050寄存器读取数据
  * @param   
  * @retval  
  */
void MPU6050_ReadData(uint8_t addr,unsigned char* Read,uint8_t num)
{
	I2C_R_Page(addr,Read,num);
}


/**
  * @brief   初始化MPU6050芯片
  * @param   
  * @retval  
  */
void MPU6050_Init(void)
{
  int i=0,j=0;
  //在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
  for(i=0;i<1000;i++)
  {
    for(j=0;j<1000;j++)
    {
      ;
    }
  }
	
	//通过对电源管理寄存器1（0x6b）的bit7位写1实现对MPU6050复位。设置该寄存器为0x00唤醒MPU6050.
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);
	
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //陀螺仪采样率为1K/(1+0x07)=125Hz
	
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	        //低通滤波器的截止频率为1K,带宽为5Hz
	
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x01);	  //配置加速度传感器工作在2G模式
	
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
}

/**
  * @brief   读取MPU6050的ID
  * @param   
  * @retval  正常返回1，异常返回0
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
  * @brief   读取MPU6050的加速度数据
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
  * @brief   读取MPU6050的角加速度数据
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
  * @brief   读取MPU6050的原始温度数据
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
	u8 temp[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,temp,2);     //读取温度值
    *tempData = (temp[0] << 8) | temp[1];
}


/**
  * @brief   读取MPU6050的温度数据，转化成摄氏度
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

