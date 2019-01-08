#ifndef __UART_COMMON_H__
#define __UART_COMMON_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

#define PACKETLEN 7

int uartCommInit(void);
void uartCommExit(void);
int uartCommGetPacket(unsigned char  *pData);
void uartSend(char *sData);

#endif // __UART_COMMON_H__
