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
 