#ifndef __BPS_SW_I2C_H__
#define __BPS_SW_I2C_H__

#include "stm32f10x.h" 

/**************************I2C参数定义，I2C1或I2C2********************************/
#define             EEPROM_I2Cx                                I2C1
#define             EEPROM_I2C_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             EEPROM_I2C_CLK                             RCC_APB1Periph_I2C1
#define             EEPROM_I2C_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             EEPROM_I2C_GPIO_CLK                        RCC_APB2Periph_GPIOB     
#define             EEPROM_I2C_SCL_PORT                        GPIOB   
#define             EEPROM_I2C_SCL_PIN                         GPIO_Pin_6
#define             EEPROM_I2C_SDA_PORT                        GPIOB 
#define             EEPROM_I2C_SDA_PIN                         GPIO_Pin_7


/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))


/*信息输出*/
#define EEPROM_DEBUG_ON         0

#define EEPROM_INFO(fmt,arg...)           printf("<<-EEPROM-INFO->> "fmt"\n",##arg)
#define EEPROM_ERROR(fmt,arg...)          printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)
#define EEPROM_DEBUG(fmt,arg...)          do{\
                                          if(EEPROM_DEBUG_ON)\
                                          printf("<<-EEPROM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)


/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */

/* EEPROM Addresses defines */
#define EEPROM_Block0_ADDRESS 0xA0   /* E2 = 0 */
//#define EEPROM_Block1_ADDRESS 0xA2 /* E2 = 0 */
//#define EEPROM_Block2_ADDRESS 0xA4 /* E2 = 0 */
//#define EEPROM_Block3_ADDRESS 0xA6 /* E2 = 0 */


void I2C_EE_Init(void);
void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite);
uint32_t I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr);
uint32_t I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);
uint32_t I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);
void I2C_EE_WaitEepromStandbyState(void);
																			
																																				
																					
																					
																					
#define SDA(a)               if(a)\
                             {GPIO_SetBits(EEPROM_I2C_SDA_PORT,EEPROM_I2C_SDA_PIN );}\
											 else\
                             {GPIO_ResetBits(EEPROM_I2C_SDA_PORT,EEPROM_I2C_SDA_PIN);} 
#define SCL(a)               if(a)\
                             {GPIO_SetBits(EEPROM_I2C_SCL_PORT,EEPROM_I2C_SCL_PIN );}\
											 else\
                             {GPIO_ResetBits(EEPROM_I2C_SCL_PORT,EEPROM_I2C_SCL_PIN);} 

													
																					
																				
void I2C_config(void);																				
void start(void);
void stop(void);
void I2C_sentdata(uint8_t byte);
uint8_t I2C_receivedata(void);
void I2C_sentACK(uint8_t ACK);
uint8_t I2C_receiveACK(void);		
void I2C_GPIO_Config(void);														 
void delay_ms(int n);
uint8_t I2C_Rbyte(uint8_t addr);
void I2C_Wbyte(uint8_t addr,uint8_t byte);

#endif
