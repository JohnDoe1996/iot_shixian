#include "websocket_common.h"
#include "uart_common.h"

void charToInt(int output[],char input[]);
void getSeverData();
void setData(int buf[]);
void dealData(char buf[]);
void conSwitch(int buf[]);
void *thread_websocket();
void *thread_uart();
void getUartData();
void dealSwitch(unsigned char sData[]);
void dealSensor(unsigned char sData[]);

#define DATA_MAX_LEN 7
int fd;
int data[DATA_MAX_LEN] = {1,0,1,24,15,35,0};//{0};

char ip[] = "119.23.19.129";
int port = 8888;        // 服务器端口

int main()
{
    pthread_t thread1, thread2;
    uartCommInit();
    //
    fd = webSocket_clientLinkToServer(ip, port, "/chat");
    if(fd <= 0)
    {
        printf("client link to server failed !\r\n");
        return -1;
    }

    //////////////////////////////start/////////////////////////////////////
    if(pthread_create(&thread1,NULL,thread_websocket,NULL)!=0)
    {
         perror("Create thread1 failed");
         exit(0);
    }
    if(pthread_create(&thread2,NULL,thread_uart,NULL)!=0)
    {
         perror("Create thread2 failed");
         exit(0);
    }


    while(1)
    {
        /*******留着以后添加界面*******/
    }
    ////////////////////////////////end///////////////////////////////////

    printf("client close !\r\n");
    uartCommExit();
    return 0;
}

/**********************************************
 *  线程1:负责 websocket
 * ********************************************/
void *thread_websocket()
{
    getSeverData();
}

/**********************************************
 *  线程2:负责 uart
 * ********************************************/
void *thread_uart()
{
    getUartData();
}

/**********************************************
 *  处理空格,把char数据转成int数据
 * *********************************************/
void charToInt(int output[],char input[])
{ //结束符是 0x0 ; 空格 0x20
    char tmp[1024];
    int iIn=4, iTmp=0, iOut=0, iFor=0;
    int val=0;
    while(input[iIn]!=0x0)
    {
        iTmp=0;
        while(input[iIn]!=0x20 && input[iIn]!=0x0)
        {
            tmp[iTmp++] = (int) input[iIn++] - '0';
        }
        val = 0;
        for(--iTmp,iFor=0; iTmp>=0; iTmp--,iFor++)
        {
            val += tmp[iFor] * pow(10, iTmp);
        }
        output[iOut++] = val;
        iIn++;
    }
}

/***********************************************
 *  判断新的数据是不是和原来的数据不同,
 *  若不同则修改data数据并发送指令到websocket
 * *********************************************/
void setData(int buf[]){
    int i, ret, isChange = 0, allVoid = 1;
    char str[1024];
    for(i = 0; i < DATA_MAX_LEN; i++)
    {
        if( data[i] != 0 )
        {
            allVoid = 0;
        }
        if( data[i] != buf[i] )
        {
            isChange = 1;
        }
    }
    printf("allVoid == %d && isChange == %d\n",allVoid,isChange);
    if(allVoid == 0 && isChange == 1)
    {
        //实时数据不一样,发送数据给websocket服务器
        sprintf(str,"set_all %d %d %d %d %d %d %d",
                buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6]);
        ret = webSocket_send(fd, str, strlen(str), true, WCT_TXTDATA);
        if(ret > 0)
        {
            printf("success: len:%d message:%s\r\n",ret,str);
             for(i = 0; i < DATA_MAX_LEN; i++)
             {
                 data[i] = buf[i];
             }
        }

    }
}

/************************************************
 *  串口发送数据,控制开关三个
 * **********************************************/
void conSwitch(int buf[])
{
    //TODO:串口发送数据
    char con[6];
    int i;
    ////////////////////////////////暂时为了调试先这样写/////////////////////////////
    ////    int i, ret;
    ////    char str[1024];
    ////    for(i=0;i<3;i++)
    ////        data[i] = buf[i];
    ////    sprintf(str,"set_all %d %d %d %d %d %d %d",
    ////            data[0],data[1],data[2],data[3],data[4],data[5],data[6]);
    ////   ret = webSocket_send(fd, str, strlen(str), true, WCT_TXTDATA);
    /////////////////////////////以上代码是直接返回消息//////////////////////////////
    con[0] = 0xff; // 起始字节
    con[1] = 0x0c; //代表是控制语句
    for(i=0;i<3;i++) //三位控制数据代入 con[2~4]
        con[i+2] = (char) buf[i];
    con[5] = 0xaa;//(unsigned char)(con[2]^con[3]^con[4]);
    ////////////////
    printf("data to uart: ");
    for(i=0;i<6;i++)
        printf("%d ",con[i]);
    printf("\n");
    /////////////////
    uartSend(con);
}

/************************************************
 *  处理websocket传过来的数据,分发给对应处理的函数
 * **********************************************/
void dealData(char buf[])
{
    int tmpData[7] = {0};
//    int i;
    if(buf[0]=='a' || buf[1]=='l' || buf[2]=='l')
    {
        charToInt(tmpData,buf);
//        for(i=0;i<7;i++) printf("%d ",tmpData[i]);
        setData(tmpData);
    }
    else if(buf[0]=='c' || buf[1]=='o' || buf[2]=='n')
    {
       charToInt(tmpData,buf);
//       for(i=0;i<3;i++) printf("%d ",tmpData[i]);
       conSwitch(tmpData);
    }
}


/************************************************
 *  接收来自服务器的数据,并填写到data数组中
************************************************/
void getSeverData()
{
    int ret;
    char buf[1024];

    while(1)
    {
        memset(buf, 0, sizeof(buf));
        ret = webSocket_recv(fd, buf, sizeof(buf));
        if(ret > 0)
        {
            printf("get : len:%d message:%s\r\n", ret, buf);
            dealData(buf);
        }
        else    // 检查错误, 是否连接已断开
        {
            if(!(errno == EAGAIN || errno == EINTR))
            {
                close(fd);
                break;
            }
        }
    }
}

/************************************************
 *  接收串口的数据,判断数据是否正确并交给对应的处理函数
************************************************/
void getUartData()
{
//    unsigned char buf[1024];
    char buf[7];
    int ret, i;
//    unsigned char check;
    while(1)
    {
         ret = uartCommGetPacket(buf);
         if(ret>0)
         {
             printf("Uart get: len:%d msg:",ret);
             for(i=0; i<ret;i++)
                 printf("%d ",buf[i]);
             printf("\r\n");
//             check = buf[1];  //先获取数据中的第一个字节
//             for(i=2; i<ret-1;i++)  //除去前面起始字节和已经代入check的第1个字节以及最后一的终止字节
//             {
//                 check ^= buf[i];  //进行异或运算
//             }
             if(buf[0]==0xff &&  //判断起始字节
                     buf[ret-1]==0xaa)//check)    //校验终止字节
             {
                 switch(buf[1])  // 判断是switch还是传感器(同时该字节数据也是数据的长度)
                 {
                 case 0x03:  //接收到的是来自switch的数据,数据长度为3
                        printf(">>data of switch:\r\n");
                        dealSwitch(buf);
                        break;                        
                 case 0x13:  //接收到的是来自switch的--上线--提示
                     printf(">>switch connect:\r\n");
                     break;
                 case 0x23:  //接收到的是来自switch的--离线--提示
                     printf(">>switch leave:\r\n");
                     break;

                 case 0x04: //接收到的是来自传感器的数据,数据长度为4
                      printf(">>data of sensor:\r\n");
                      dealSensor(buf);
                      break;
                 case 0x14: //接收到的是来自传感器的--上线--提示
                      printf(">>sensor connect:\r\n");
                      break;
                 case 0x24: //接收到的是来自传感器的--离线--提示
                      printf(">>sensor leave:\r\n");
                      break;

                 case 0x10: // 其他节点_上线-提示
                     printf(">>other connect:\r\n");
                     break;
                 case 0x20: // 其他节点_离线-提示
                     printf(">>other leave:\r\n");
                     break;

                 default:
                      printf(">>data error!\r\n");
                        break;
                 }
             }
         }
    }
}

/************************************************
 *  switch数据处理,并做出对应的操作
************************************************/
void dealSwitch(unsigned char sData[])
{
    int bufData[7];
    int i;
    for(i=0;i<3;i++)
    {
        bufData[i] = (int)sData[i+2];
    }
    for(i=3;i<7;i++)
    {
        bufData[i] = data[i];
    }
    printf("switch: ");
    for(i=0;i<7;i++) printf("%d ",bufData[i]);
    printf("\r\n");
    setData(bufData);
}

/************************************************
 *  传感器数据处理,并做出对应的操作
************************************************/
void dealSensor(unsigned char  sData[])
{
    int bufData[7];
    int i;
    for(i=0;i<3;i++)
    {
        bufData[i] = data[i];
    }
    for(i=3;i<7;i++)
    {
        bufData[i] = (int)sData[i-1];
    }
    printf("sensor: ");
    for(i=0;i<7;i++) printf("%d ",bufData[i]);
    printf("\r\n");
    setData(bufData);
}
