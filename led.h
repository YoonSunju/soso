#ifndef _LED_H_
#define _LED_H_

int ledLibInit(void);
int ledOnOff (int ledNum, int onOff);
int ledStatus (void);
int ledLibExit(void);
int ledread(const char* data);
void doHelp(void);

#define LED_DRIVER_NAME "/dev/periled"

#endif //LED_H_

