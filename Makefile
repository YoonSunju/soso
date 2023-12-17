CC= arm-linux-gnueabi-gcc
AR= arm-linux-gnueabi-ar

all: project

project: libMyPeri.a project.c
	$(CC) project.c -l MyPeri -L. -o project -lpthread
libMyPeri.a: bitmap.o touch.o button.o led.o textlcd.o fnd.o buzzer.o colorled.o
	$(AR) rc libMyPeri.a bitmap.o touch.o button.o led.o textlcd.o fnd.o buzzer.o colorled.o
bitmap.o: bitmap.c bitmap.h
	$(CC) bitmap.c -c -o bitmap.o
touch.o: touch.c touch.h
	$(CC) touch.c -c -o touch.o
button.o: button.c button.h
	$(CC) button.c -c -o button.o -lpthread
led.o: led.c led.h
	$(CC) led.c -c -o led.o
textlcd.o: textlcd.c textlcd.h
	$(CC) textlcd.c -c -o textlcd.o
fnd.o: fnd.c fnd.h
	$(CC) fnd.c -c -o fnd.o
buzzer.o: buzzer.c buzzer.h
	$(CC) buzzer.c -c -o buzzer.o
colorled.o: colorled.c colorled.h
	$(CC) colorled.c -c -o colorled.o
clean:
	-rm *.o *.a

