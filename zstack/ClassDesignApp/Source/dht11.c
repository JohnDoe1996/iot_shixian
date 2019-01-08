#include "ioCC2530.h"
#include <stdio.h>
#include "dht11.h"
#include <zcomdef.h>

#define uchar unsigned char 
#define uint unsigned int

#define  DHT11_DATA  P0_4

/*****************全局变量的定义******************/
uchar  Overtime_counter;  //判断等待是否超时的计数器。利用uchar型的数值范围进行自动延时控制（时长由初值决定），并判断是否超时
uchar  bit_value;          //从DATA总线上读到的位值
uchar  T_data_H=0, T_data_L=0, RH_data_H, RH_data_L, checkdata;//校验过的温度高8位,温度低8位,湿度高8位,湿度低8位,校验和8位
uchar  T_data_H_temp, T_data_L_temp, RH_data_H_temp, RH_data_L_temp, checkdata_temp;//未经校验的数据
uchar  comdata;            //从DHT11读取的一个字节的数据

// void Delay(uint);
void Delay_us(void); //1 us延时
void Delay_10us(void); //10 us延时
void Delay_ms(uint Time);//n ms延时

void InitialAD(void);
void Read_Byte(void);
void Read_DHT11(void);
uchar  str[2];

/*****************延时函数******************/
// static void Delay(unsigned int n)
// {
// 	unsigned int i;
// 	for(i = 0; i < n; i++);
//         for(i = 0; i < n; i++);
//         for(i = 0; i < n; i++);
//         for(i = 0; i < n; i++);
//         for(i = 0; i < n; i++);
// }

// static void Delay_10us(unsigned char n)
// {
// 	for( ; n > 0; n--);
// }

void Delay_us() //1 us延时
{
   MicroWait(1);   
}

void Delay_10us() //10 us延时
{
  MicroWait(10);
}

void Delay_ms(uint Time)//n ms延时
{
  unsigned char i;
  while(Time--)
  {
    for(i=0;i<100;i++)
     Delay_10us();
  }
}

/*****************从DHT11读取一个字节函数******************/        
void  Read_Byte(void)
{
        uchar i;
        for (i = 0; i < 8; i++)                     //循环8次，读取8bit的数据
        {
          Overtime_counter = 2;                          //读取并等待DHT11发出的12-14us低电平开始信号
          P0DIR &= ~0x10;
          while ((!DHT11_DATA) && Overtime_counter++);
          Delay_10us();
          Delay_10us();
          Delay_10us();
          //  MicroWait(27);                   //26-28us的低电平判断门限
          bit_value = 0;                          //跳过门限后判断总线是高还是低，高为1，低为0
          if(DHT11_DATA)
            bit_value = 1;
          Overtime_counter=2;                          //等待1bit的电平信号结束，不管是0是1在118us后都变为低电平，否则错误超时
          while (DHT11_DATA && Overtime_counter++);  //当U8FLAG加到255后溢出为0，跳出循环，并后加加为1
          if (Overtime_counter == 1)
            break;                           //超时则跳出for循环        
          comdata <<= 1;                      //左移1位，LSB补0
          comdata |= bit_value;                  //LSB赋值
        }
}

/*****************DHT11读取五个字节函数******************/
void Read_DHT11(void)
{
        uchar checksum;
        P0DIR |= 0x10;
        DHT11_DATA = 0;                //主机拉低18ms
        // MicroWait(30);
        Delay_ms(19);
        DHT11_DATA = 1;                //总线由上拉电阻拉高 主机延时20us-40us
        Delay_10us();
        Delay_10us();						
        Delay_10us();
        Delay_10us(); 
        // MicroWait(30);
        DHT11_DATA = 1;                //主机转为输入或者输出高电平，DATA线由上拉电阻拉高，准备判断DHT11的响应信号
        P0DIR &= ~0x10;
        if (!DHT11_DATA)                //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行        
        {
          Overtime_counter = 2;   //判断DHT11发出的80us的低电平响应信号是否结束
          while ((!DHT11_DATA)&&Overtime_counter++);
          Overtime_counter=2;   //判断DHT11是否发出80us的高电平，如发出则进入数据接收状态
          while ((DHT11_DATA)&&Overtime_counter++);
          Read_Byte();                //读取湿度值整数部分的高8bit
          RH_data_H_temp = comdata;
          Read_Byte();                //读取湿度值小数部分的低8bit
          RH_data_L_temp = comdata;
          Read_Byte();                //读取温度值整数部分的高8bit
          T_data_H_temp = comdata;
          Read_Byte();                //读取温度值小数部分的低8bit
          T_data_L_temp = comdata;
          Read_Byte();                //读取校验和的8bit
          checkdata_temp = comdata;
          P0DIR |= 0x10;
          DHT11_DATA = 1;                //读完数据将总线拉高
          checksum = (T_data_H_temp + T_data_L_temp + RH_data_H_temp + RH_data_L_temp);//进行数据校验
          if (checksum == checkdata_temp)
            {
              RH_data_H = RH_data_H_temp;
              RH_data_L = RH_data_L_temp;
              T_data_H  = T_data_H_temp;
              T_data_L  = T_data_L_temp;
              checkdata = checkdata_temp;
            }
        }
}


void getHumiture(uint8 *t,uint8 *h)
{
	Read_DHT11();//调用温湿度读取子程序
	//sprintf(str, "%d%d", T_data_H, RH_data_H);
        *t = T_data_H;
        *h = RH_data_H;
}