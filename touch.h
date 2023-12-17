#ifndef _TOUCH_H_
#define _TOUCH_H_
#define MESSAGE_TO 1123
typedef struct
{
    long int messageNum;
    int keyInput;
    int pressed;
    int x;
    int y;
} TOUCH_MSG_T;

int touchInit(void);
int touchExit(void);

#endif

