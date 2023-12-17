#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/msg.h>
#include <pthread.h>
#include "touch.h"

#define INPUT_DEVICE_LIST "/dev/input/event"
#define PROBE_FILE "/proc/bus/input/devices"

int probeTouchPath(char *newPath)
{
	int returnValue = 0;
	int number1 = 0;
#define HAVE_TO_FIND_1 "N: Name=\"WaveShare WaveShare Touchscreen\"\n"
#define HAVE_TO_FIND_2 "H: Handlers=mouse0 event"
	
	FILE *fp = fopen(PROBE_FILE, "rt");
	while(!feof(fp))
	{
		char tmpStr[200];
		fgets(tmpStr,200,fp);
		if(strcmp(tmpStr,HAVE_TO_FIND_1) ==0)
		{
			printf("yes! i found!:%s\r\n",tmpStr);
			returnValue =1;
		}
		if(
				(returnValue ==1)&&
				(strncasecmp(tmpStr,HAVE_TO_FIND_2,strlen(HAVE_TO_FIND_2))==0)
		  )
		{
			printf("-->%s",tmpStr);
			printf("\t%c\r\n",tmpStr[strlen(tmpStr)-3]);
			number1 = tmpStr[strlen(tmpStr)-3]-'0';
			break;
		}
	}
	fclose(fp);
	if (returnValue == 1)
		sprintf (newPath,"%s%d",INPUT_DEVICE_LIST,number1);
	return returnValue;
}

static pthread_t touchTh_id;
static int fd = 0;
static int msgID;

void *touchThFunc(void* args)
{
	int x =0;
	int y =0;
	struct input_event stEvent;
	TOUCH_MSG_T sendMsg;
	sendMsg.messageNum = 1;
	sendMsg.keyInput = 999;
	printf ("Touch Thread Readdy \r\n");
	while (1)
	{
		read(fd,&stEvent,sizeof(stEvent));
		if (stEvent.type == EV_ABS)
		{
			//뭔가 좌표값이 들어온다
			if (stEvent.code == ABS_MT_POSITION_X)
			{
				x = stEvent.value;
				printf("you touch X: %d\r\n",stEvent.value);
			}
			else if (stEvent.code == ABS_MT_POSITION_Y)
			{
				y = stEvent.value;
				printf("you touch Y: %d\r\n",stEvent.value);
			}
		}
		else if( (stEvent.type == EV_KEY) && (stEvent.code == BTN_TOUCH))
		{
			  
			//좌표 입력이 끝났다.
			if (stEvent.value == 0)
			{
				sendMsg.pressed = 0;
                sendMsg.x=x;
			      sendMsg.y=y;  
				printf("you finished touch\r\n");
			}
			else if (stEvent.value ==1)
			{
				sendMsg.pressed = 1;
				printf("you touch now\r\n");
			}
			msgsnd(msgID,&sendMsg, sizeof(TOUCH_MSG_T) - sizeof(long int),0);
		}
		


/*
		if ((stEvent.value ==0)&&(stEvent.type == EV_KEY) )
		{
			TOUCH_MSG_T newMsg;
			newMsg.messageNum =1;
			newMsg.keyInput = stEvent.code;
			msgsnd(msgID, &newMsg, sizeof(TOUCH_MSG_T) - sizeof(long int), 0);
		}
*/
	}
}

int touchInit(void)
{
	char touchPath[1024];
	if (probeTouchPath(touchPath) == 0)
	{
		printf("Device Touch Open Failed\r\n");
		return 0;
	}
	fd=open (touchPath, O_RDONLY);
	msgID = msgget (MESSAGE_TO, IPC_CREAT|0666);
	TOUCH_MSG_T trashCan;
	while (msgrcv (msgID, &trashCan, sizeof(TOUCH_MSG_T)-sizeof(long int), 0, IPC_NOWAIT) >=0)
	{
		printf("Cleaning Meassages...\r\n");
	}
	pthread_create(&touchTh_id,NULL,touchThFunc,NULL);
	return 1;
}

int touchExit(void)
{
	pthread_cancel(touchTh_id);
}


