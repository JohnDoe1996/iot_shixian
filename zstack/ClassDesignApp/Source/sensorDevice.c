#include "smartHome.h"
#include "sapi.h"
#include "hal_led.h"
#include "dht11.h"
#include "light.h"
#include "voice.h"
#include "hal_uart.h"

#define SNUM  1
uint8 sIdList[SNUM]={2};

// #define NUM_IN_CMD_SDEVICE 1
#define NUM_OUT_CMD_SDEVICE 3

uint8 stateData[4] = {0x0,0x0,0x0,0x0};

void sendCon(void);
void dataCollcet(void);

// const cId_t SDeviceInputCommandList[NUM_IN_CMD_SDEVICE]=
//                                 {TOGGLE_S_CMD_ID};
const cId_t sDeviceOutputCommandList[NUM_OUT_CMD_SDEVICE]=
                                {SJOINNET_CMD_ID,HEART_BEAT_CMD_ID,MSG_CMD_ID};
const SimpleDescriptionFormat_t zb_SimpleDesc=
{
  ENDPOINT_ID_SMARTHOME,
  PROFILE_ID_SMARTHOME,
  DEVICE_ID_SDEVICE,
  DEVIDE_VERSION_ID,
  0, 
  // NUM_IN_CMD_SDEVICE,
  // (cId_t*)sDeviceInputCommandList,
  0,
  NULL,
  NUM_OUT_CMD_SDEVICE,
  (cId_t*)sDeviceOutputCommandList  
};
/***********
执行时机：发送的数据包被接收方收到时被调用
handle:包的编号；
status:ZSUCCESS表示成功接收
************/
void zb_SendDataConfirm( uint8 handle, uint8 status )
{
  
}

/***********
执行时机：接收到的数据包被调用
************/
void zb_ReceiveDataIndication( uint16 source, uint16 command, 
                              uint16 len, uint8 *pData  )
{
}


void zb_AllowBindConfirm( uint16 source )
{
}

void zb_HandleKeys( uint8 shift, uint8 keys )
{
  
}

void zb_BindConfirm( uint16 commandId, uint8 status )
{
}


//void zb_SendDataRequest ( uint16 destination, uint16 commandId, uint8 len,
//                          uint8 *pData, uint8 handle, uint8 ack, uint8 radius );
void zb_StartConfirm( uint8 status )
{
  if(status==ZSUCCESS)
  {
    zb_SendDataRequest(0X0,SJOINNET_CMD_ID,
                       SNUM,sIdList,0,FALSE,AF_DEFAULT_RADIUS);
    osal_start_timerEx(sapi_TaskID,TIMER_TIMEOUT_EVT,2000);
    osal_start_timerEx(sapi_TaskID,SEND_TIMER_EVT,8000);
    dataCollcet();
  }
}

void zb_HandleOsalEvent( uint16 event )
{
  if(event&TIMER_TIMEOUT_EVT)
  {
    osal_start_timerEx(sapi_TaskID,TIMER_TIMEOUT_EVT,2000);
    zb_SendDataRequest(0X0,HEART_BEAT_CMD_ID,
                       0,NULL,0,FALSE,AF_DEFAULT_RADIUS); 
  }
  if(event&SEND_TIMER_EVT)
  {
    osal_start_timerEx(sapi_TaskID,SEND_TIMER_EVT,8000);
    dataCollcet();
    sendCon();
  }
}

void zb_FindDeviceConfirm( uint8 searchType, 
                          uint8 *searchKey, uint8 *result )
{
  
}

void dataCollcet(void)
{
  uint8 t=0,h=0,l=0,v=0;
  getHumiture(&t,&h);
  l = getLight();
  v = getVoice();
  stateData[0] = t;
  stateData[1] = h;
  stateData[2] = l;
  stateData[3] = v;
}

void sendCon(void)
{ 
  uint8 tmpData[7];
  tmpData[0] = 0xff;
  tmpData[1] = 0x04;
  tmpData[2] = stateData[0];
  tmpData[3] = stateData[1];
  tmpData[4] = stateData[2];
  tmpData[5] = stateData[3];
  tmpData[6] = 0xaa;
  zb_SendDataRequest(0X0,MSG_CMD_ID,7,tmpData,
                      0,FALSE,AF_DEFAULT_RADIUS);
  //////////////////////////
  // HalUARTWrite(0,tmpData,7);
  //////////////////////////
}