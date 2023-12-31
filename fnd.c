#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "fnd.h"
#define FND_DRIVER_NAME "/dev/perifnd"

typedef struct FNDWriteDataForm_tag
{
char DataNumeric[FND_DATA_BUFF_LEN]; //숫자 0-9
char DataDot[FND_DATA_BUFF_LEN]; //숫자 0 or 1
char DataValid[FND_DATA_BUFF_LEN]; //숫자 0 or 1
}stFndWriteForm,*pStFndWriteForm;

int fnd(int num, char mode)
{

if (mode == MODE_STATIC_DIS )
{
fndDisp(num , 0);
}
else if(mode == MODE_TIME_DIS )
{
struct tm *ptmcur;
time_t tTime;
if ( -1 == time(&tTime) )
return -1;
ptmcur = localtime(&tTime);
num = ptmcur->tm_hour * 10000;
num += ptmcur->tm_min *100;
num += ptmcur->tm_sec;
fndDisp(num , 0b1010);
}
else if (mode == MODE_COUNT_DIS)
{
int counter = 0;
while(1)
{
if (!fndDisp(counter , 0))
break;
counter++;
sleep(1);
if (counter > num )
break;
}
}
}

int fndDisp(int num , int dotflag) //0-999999 숫자, 비트로 인코딩된 dot on/off
{
int fd;
int temp,i;
stFndWriteForm stWriteData;
for (i = 0; i < MAX_FND_NUM ; i++ )
{
stWriteData.DataDot[i] = (dotflag & (0x1 << i)) ? 1 : 0;
stWriteData.DataValid[i] = 1;
}
// if 6 fnd
temp = num % 1000000; stWriteData.DataNumeric[0]= temp /100000;
temp = num % 100000; stWriteData.DataNumeric[1]= temp /10000;
temp = num % 10000; stWriteData.DataNumeric[2] = temp /1000;
temp = num %1000; stWriteData.DataNumeric[3] = temp /100;
temp = num %100; stWriteData.DataNumeric[4] = temp /10;
stWriteData.DataNumeric[5] = num %10;   

fd = open(FND_DRIVER_NAME,O_RDWR);
if ( fd < 0 )
{
perror("driver open error.\n");
return 0;
}
write(fd, &stWriteData, sizeof(stFndWriteForm));
close(fd);
return 1;
}


