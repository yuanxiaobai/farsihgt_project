/*******************************************************************/
/*                           serial.h                              */
/*******************************************************************/
#ifndef __SERIAL_H__ 
#define __SERIAL_H__
 
extern int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
extern int open_port(int fd,int comport);
 
#endif
 
 
/*******************************************************************/
/*                          serial.c                               */
/*******************************************************************/
 
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include "serial.h"
/*******************************************************************
* 函数名称： set_opt
* 功能描述： 设置串口基本参数
* 输入参数： fd     打开的串口标识符 （通过open_port函数返回）
    nSpeed 波特率 2400、4800、9600、115200
    nBits  数据位 7、8
    nEvent 奇偶校验 'O' 'N' 'E'
    nStop  停止位 1、2
* 输出参数： 无
* 返 回 值： 0  设置成功
    -1 设置过程出错
* 其它说明： 无
* 修改日期   版本号     修改人     修改内容
* --------------------------------------------------------------------
* 2010/09/27      V1.0             ***     创建函数
***********************************************************************/
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;
 //保存测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息
    if ( tcgetattr( fd,&oldtio) != 0) 
    { 
        perror("SetupSerial 1");
        return -1;
    }
 //extern void bzero(void *s, int n); 置字节字符串s的前n个字节为零
    bzero( &newtio, sizeof( newtio ) );
 //设置字符大小
    newtio.c_cflag |= CLOCAL | CREAD; 
    newtio.c_cflag &= ~CSIZE;
 //设置数据位
    switch( nBits )
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }
 //设置校验位
    switch( nEvent )
    {
    case 'O':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E': 
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N': 
        newtio.c_cflag &= ~PARENB;
        break;
    }
 //设置波特率
 switch( nSpeed )
    {
    case 2400:
        cfsetispeed(&newtio, B2400);
        cfsetospeed(&newtio, B2400);
        break;
    case 4800:
        cfsetispeed(&newtio, B4800);
        cfsetospeed(&newtio, B4800);
        break;
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }
 //设置停止位
    if( nStop == 1 )
        newtio.c_cflag &= ~CSTOPB;
    else if ( nStop == 2 )
  newtio.c_cflag |= CSTOPB;
 //设置等待时间和最小接收字符 
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;
 //处理未接收字符
    tcflush(fd,TCIFLUSH);
 //激活新配置
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error");//打印com set error及出错原因
        return -1;
    }
    printf("set done!\n");
    return 0;
}
/**********************************************************************
* 函数名称： open_port
* 功能描述： 打开指定串口
* 输入参数： fd 文件描述符
    comport 串口号（1、2、3）
* 输出参数： 无
* 返 回 值： 出错 返回 -1 
             成功 返回 fd文件描述符
* 其它说明： 无
* 修改日期   版本号     修改人     修改内容
* --------------------------------------------------------------------
* 2010/09/27      V1.0             ***     创建函数
***********************************************************************/
int open_port(int fd,int comport)
{
    char *dev[]={"/dev/ttyS0","/dev/ttyS1","/dev/ttyS2"};
    long vdisable;//没用
 //打开串口
    if (comport==1)
    {    
  fd = open("/dev/ttyS0",O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd)
  {
            perror("Can't Open Serial Port");
            return(-1);
        }
        else 
            printf("open ttyS0 .....\n");
    }
    else if(comport==2)
    {    
  fd = open("/dev/ttyS1",O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd)
  {
            perror("Can't Open Serial Port");
            return(-1);
        }
        else 
            printf("open ttyS1 .....\n");
    }
    else if (comport==3)
    {
        fd = open("/dev/ttyS2",O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd)
  {
            perror("Can't Open Serial Port");
            return(-1);
        }
        else 
            printf("open ttyS2 .....\n");
    }
 else if (comport==4)
 {
  fd = open("/dev/ttyS3",O_RDWR|O_NOCTTY|O_NDELAY);
  if (-1 == fd)
  {
   perror("Can't Open Serial Port");
   return(-1);
  }
  else 
   printf("open ttyS3 .....\n");
 }
 //恢复串口的状态为阻塞状态，用于等待串口数据的读入
    if(fcntl(fd, F_SETFL, 0) < 0)
        printf("fcntl failed!\n");
    else
        printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
 //测试打开的文件描述符是否引用一个终端设备，以进一步确认串口是否正确打开
    if(isatty(STDIN_FILENO)==0)
        printf("standard input is not a terminal device\n");
    else
        printf("isatty success!\n");
    printf("fd-open=%d\n",fd);
    return fd;
}
 
 
/*******************************************************************/
/*                          main.c                                 */
/*******************************************************************/
 
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include "serial.h"
int main(void)
{
 int fd;
 int nread,i;
 char buff[512];
 bzero(buff, 512);
 if((fd=open_port(fd,4)) < 0)
 {
  perror("open_port error");
  return -1;
 }
 if((i=set_opt(fd,115200,8,'N',1)) < 0)
 {
  perror("set_opt error");
  return -1;
 }
 printf("fd=%d\n",fd);
 while (1)
 {
  sleep(2);
  //while ((nread = read(fd,buff,512)) > 0)
  {
   nread = read(fd,buff,512);
   printf("nread = %d\n",nread);
   printf("%s\n",buff);
   bzero(buff,nread);//清空
   //usleep(100000);
  }
 }
 close(fd);
 return 0;
}
 