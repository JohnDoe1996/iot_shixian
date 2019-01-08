#include "ioCC2530.h"
#include "stdio.h"
#include "light.h"

#define uchar unsigned char 
#define uint unsigned int

#define ADC_CHNN    0x05  //选用通道5

/********************************************************
 * @brief   读取ADC的值
********************************************************/
static uint16 readAdc(uint8 channal)
{
          uint16 value ; 
          APCFG |= 1 << channal ; // 模拟信号设置
          ADCIF = 0 ;
         
          ADCCON3 = channal;          
          while ( !ADCIF ) ;  //等待转换完成
          
          value = ADCL ;
          value |= ((uint16) ADCH) << 8 ;
          
          return value; 
}


uint8 getLight(void)
{
    uint AvgValue = 0;
    uint8 result;
    int i;
    for (i = 0 ; i < 64 ; i++)
    {
      AvgValue += readAdc(ADC_CHNN);             
      AvgValue >>= 1;
    }

    result = (uint8)(96-(((AvgValue-500)/1000)*3));

    return result;

}