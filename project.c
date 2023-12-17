#include <stdio.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> 
#include <fcntl.h>
#include <sys/ioctl.h>

#include "touch.h"
#include "bitmap.h"
#include "textlcd.h"
#include "button.h"
#include "fnd.h"
#include "buzzer.h"
#include "led.h"
#include "colorled.h"

#define INPUT_DEVICE_LIST "/dev/input/event"
#define PROBE_FILE "/proc/bus/input/devices"


int main(int argc,char **argv)
{
touchInit();
buttonInit();
buttonStart();
buzzerInit();
ledLibInit();
pwmLedInit();

int msgTOUCH = msgget(MESSAGE_TO, IPC_CREAT|0666);
int msgBUTTON = msgget(MESSAGE_BU, IPC_CREAT|0666);

TOUCH_MSG_T recvTO;
BUTTON_MSG_T recvBU;

int start = 0;
int menu = 0;
int count = 0;
int class = 0;
int lcount = 0;
int cout = 0;
int B,G,R;
char mode;
//테스트용 명령어들 끝나면 다음 주석까지 제거
lcdtextwrite ("reboot","embedded",0);
fnd(count,MODE_STATIC_DIS);
ledread ("0x00");
pwmSetPercent(0,0);
pwmSetPercent(0,1);
pwmSetPercent(0,2);
////////////////////////
bitmainfunc("bbam600.bmp"); //메인화면 띄우기

while(1) //start 버튼을 누르기 전까지 대기
    {
    msgrcv(msgTOUCH, &recvTO, sizeof(recvTO)-sizeof(long int),0,0); //터치 값 받기
    switch (recvTO.keyInput)
        {
        case 999:	
        if (recvTO.pressed==0)
            {
            if (recvTO.x>350&&recvTO.x<550&&recvTO.y>280&&recvTO.y<320)
                {
                printf("embedded start!!");
                start =1;
                }
            break;
            }
        else
            {
            printf("wait");
            }
        }
    if(start==1)
        break;
    }
touchExit();
bitmainfunc("1.bmp");
sleep(1);
//라면선택 창
touchInit();
while(1)
{
msgrcv(msgTOUCH, &recvTO, sizeof(recvTO)-sizeof(long int),0,0); //터치 값 받기

if (recvTO.pressed==0)
    {  
    if (recvTO.x <= 612)
    menu = 1;
    else if (recvTO.x >612)
    menu = 2;     
break;
    }
}

if (menu == 1) //1번메뉴 신라면일때
{
bitmainfunc("2.bmp");
lcdtextwrite ("press the HOME","how to use button",0);
count = 210;
while(1)
    {
    fnd(count,MODE_STATIC_DIS);
    msgrcv(msgBUTTON, &recvBU, sizeof(unsigned short) * 2 + sizeof(int), 0, 0);
    
    if(recvBU.type == EV_KEY)
        {
        if (recvBU.pressed==0)
            {
            switch(recvBU.keyInput)
                {
                case KEY_HOME:
                    {
                    if (class == 0)
                        {
                        lcdtextwrite ("HOME: how to use","BACK: start timer",0);
                        class++;
                        }
                    else if (class== 1)
                        {
                        lcdtextwrite ("SERCH: Up 1m","MENU: down 1m",0);
                        class++;
                        }
                    else if (class == 2)
                        {
                        lcdtextwrite ("V.UP: Up 10s","V.DOWN: down 10s",0);
                        class = 0;
                        }
                            break;  
                     }
                case KEY_BACK: 
                        { 
                        start = 2;
                        break;
                        }   
                case KEY_SEARCH:
                        {
                        count = count + 60;
                        break;
                        }
                case KEY_MENU: 
                        {
                        count= count-60;
                        break;
                        }
                case KEY_VOLUMEUP:
                        {
                        count= count +10;
                        break;
                        }
                case KEY_VOLUMEDOWN:
                        {
                        count = count-10;
                        break;
                        }
                }
            }
        }
    if(start==2)
    break;            
    }
lcount = count/8;
int percent =0 ;
while(1)
{
fnd(count,MODE_STATIC_DIS);
percent = (cout/lcount);
if(percent==1) ledread("0x01");
else if(percent==2) ledread("0x03");
else if(percent==3) ledread("0x07");
else if(percent==4) ledread("0x0F");
else if(percent==5) ledread("0x1F");
else if(percent==6) ledread("0x3F");
else if(percent==7) ledread("0x7F");
else if(percent==8) ledread("0xFF");

sleep(1);
if (count==0)
break;
count--;
cout++;
}
buzzerPlaySong(musicScale[0]);
pwmSetPercent(0,0);
pwmSetPercent(0,1);
pwmSetPercent(100,2);

sleep(1);
buzzerPlaySong(musicScale[2]);
pwmSetPercent(0,0);
pwmSetPercent(100,1);
pwmSetPercent(0,2);

sleep(1);
buzzerPlaySong(musicScale[4]);
pwmSetPercent(100,0);
pwmSetPercent(0,1);
pwmSetPercent(0,2);

sleep(1);
buzzerPlaySong(musicScale[7]);
pwmSetPercent(100,0);
pwmSetPercent(100,1);
pwmSetPercent(100,2);

sleep(1);
buzzerStopSong();
}





if (menu == 2) //2번메뉴 짜파게티일때
{
bitmainfunc("3.bmp");
lcdtextwrite ("press the HOME","how to use button",0);
count = 300;
while(1)
    {
    fnd(count,MODE_STATIC_DIS);
    msgrcv(msgBUTTON, &recvBU, sizeof(unsigned short) * 2 + sizeof(int), 0, 0);
    
    if(recvBU.type == EV_KEY)
        {
        if (recvBU.pressed==0)
            {
            switch(recvBU.keyInput)
                {
                case KEY_HOME:
                    {
                    if (class == 0)
                        {
                        lcdtextwrite ("HOME: how to use","BACK: start timer",0);
                        class++;
                        }
                    else if (class== 1)
                        {
                        lcdtextwrite ("SERCH: Up 1m","MENU: down 1m",0);
                        class++;
                        }
                    else if (class == 2)
                        {
                        lcdtextwrite ("V.UP: Up 10s","V.DOWN: down 10s",0);
                        class = 0;
                        }
                            break;  
                     }
                case KEY_BACK: 
                        { 
                        start = 2;
                        break;
                        }   
                case KEY_SEARCH:
                        {
                        count = count + 60;
                        break;
                        }
                case KEY_MENU: 
                        {
                        count= count-60;
                        break;
                        }
                case KEY_VOLUMEUP:
                        {
                        count= count +10;
                        break;
                        }
                case KEY_VOLUMEDOWN:
                        {
                        count = count-10;
                        break;
                        }
                }
            }
        }
    if(start==2)
    break;            
    }
lcount = count/8;
int percent =0 ;
while(1)
{
fnd(count,MODE_STATIC_DIS);
percent = (cout/lcount);
if(percent==1) ledread("0x01");
else if(percent==2) ledread("0x03");
else if(percent==3) ledread("0x07");
else if(percent==4) ledread("0x0F");
else if(percent==5) ledread("0x1F");
else if(percent==6) ledread("0x3F");
else if(percent==7) ledread("0x7F");
else if(percent==8) ledread("0xFF");

sleep(1);
if (count==0)
break;
count--;
cout++;
}

buzzerPlaySong(musicScale[0]);
pwmSetPercent(0,0);
pwmSetPercent(0,1);
pwmSetPercent(100,2);

sleep(1);
buzzerPlaySong(musicScale[2]);
pwmSetPercent(0,0);
pwmSetPercent(100,1);
pwmSetPercent(0,2);

sleep(1);
buzzerPlaySong(musicScale[4]);
pwmSetPercent(100,0);
pwmSetPercent(0,1);
pwmSetPercent(0,2);

sleep(1);
buzzerPlaySong(musicScale[7]);
pwmSetPercent(100,0);
pwmSetPercent(100,1);
pwmSetPercent(100,2);

sleep(1);
buzzerStopSong();
}

lcdtextwrite ("Press any button","TO exit",0);

while(1)
{
msgrcv(msgBUTTON, &recvBU, sizeof(unsigned short) * 2 + sizeof(int), 0, 0);
if(recvBU.type == EV_KEY)
    {
    if (recvBU.pressed==1)
        {
lcdtextwrite ("","",0);
fnd(count,MODE_STATIC_DIS);
ledread ("0x00");
pwmSetPercent(0,0);
pwmSetPercent(0,1);
pwmSetPercent(0,2);
bitmainfunc("0");
break;
}}}

touchExit();
buttonExit();
buzzerExit();
ledLibExit();
pwmInactiveAll();
	
return 0;
}


