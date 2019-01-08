#include "uart_common.h"

static int uartFd;    // 串口连接句柄

/**********************************************
 *  串口初始化并打开串口:
 *  返回 -1 代表打开串口失败
 *  返回 0 代表打开串口成功
 * ********************************************/
int uartCommInit(void)
{
    struct termios uart_cfg;
    uartFd=open("/dev/ttyUSB0",O_RDWR|O_NONBLOCK|O_NOCTTY);
//    uartFd=open("/dev/ttySAC3",O_RDWR|O_NONBLOCK|O_NOCTTY);
    if(uartFd<0)
    {
        perror("Failed to open serial:");
        return -1;
    }
    fcntl(uartFd,F_SETFL,0);
    cfmakeraw(&uart_cfg);
    cfsetspeed(&uart_cfg,B115200); //波特率:115200
    uart_cfg.c_cc[VTIME]=0;
    uart_cfg.c_cc[VMIN]=1;
    tcflush(uartFd,TCIOFLUSH);
    tcsetattr(uartFd,TCSANOW,&uart_cfg);
    return 0;
}

/**********************************************
 *  关闭串口
 * ********************************************/
void uartCommExit(void)
{
    close(uartFd);
}

/**********************************************
 *  串口接收数据包
 *  参数 *pData 是用来接收串口的数据
 *  返回值是接收到的数据包长度(小于等于0失败)
 * ********************************************/
int uartCommGetPacket(unsigned char  *pData)
{
    int count=0,i;
    char tmp[1];
    while(count<PACKETLEN)
    {
        int ret ;
        ret=read(uartFd,tmp,1);
        count = ret;
        if(ret<=0)
        {
            return ret;
        }
        if(tmp[0] == 0xff){
            pData[0] = 0xff;
            for(i = 1 ;i<PACKETLEN;i++)
            {
                ret=read(uartFd,tmp,1);
                pData[i] = tmp[0];
                count += ret;
            }
        }

    }
    return count;
}

/**********************************************
 *  向串口发送数据
 *  参数 *sData 是要向串口发送的数据
 * ********************************************/
void uartSend(char *sData)
{
    int i;
    printf("data to uart: ");
    for(i=0;i<6;i++)
        printf("%d ",sData[i]);
    printf("\n");
    write(uartFd, sData, 6);
}
