#ifndef __BPS_HW_I2C_H__
#define __BPS_HW_I2C_H__

#include "stm32f10x.h" 
#include "bps_usart.h"
#include <stdio.h>




#define             SENSORS_I2Cx                                I2C1
#define             SENSORS_I2C_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             SENSORS_I2C_CLK                             RCC_APB1Periph_I2C1
#define             SENSORS_I2C_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             SENSORS_I2C_GPIO_CLK                        RCC_APB2Periph_GPIOB     
#define             SENSORS_I2C_SCL_PORT                        GPIOB   
#define             SENSORS_I2C_SCL_PIN                         GPIO_Pin_6
#define             SENSORS_I2C_SDA_PORT                        GPIOB 
#define             SENSORS_I2C_SDA_PIN                         GPIO_Pin_7

/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

/*信息输出*/
#define MPU_DEBUG_ON         1

#define MPU_INFO(fmt,arg...)           printf("<<-MPU-INFO->> "fmt"\n",##arg)
#define MPU_ERROR(fmt,arg...)          printf("<<-MPU-ERROR->> "fmt"\n",##arg)
#define MPU_DEBUG(fmt,arg...)          do{\
                                          if(MPU_DEBUG_ON)\
                                          printf("<<-MPU-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)


#define I2C_OWN_ADDRESS               0X0A

void I2C_Config(void);
void I2C_WriteByte(uint8_t addr,uint8_t Data);
void I2C_ReadByte(uint8_t addr,uint8_t *Data,uint8_t n);
//void I2C_Write_Page_Byte(uint8_t addr,uint8_t *Data,uint8_t n);
																					

#endif
