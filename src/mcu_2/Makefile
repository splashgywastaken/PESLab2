MCU=atmega8
F_CPU=1200000
CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-std=c99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
TARGET=../../build/atmega8_2/atmega8_2_build
SRCS=main.c

all:
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex

# Примеры для написания своих команд сборки
# build_b3:
# 	${CC} ${CFLAGS} -DLED1=PB1 -DLED2=PB2 -DBUTTON1=PB3 -o ${TARGET}.bin ${SRCS}

# build_b4:
# 	${CC} ${CFLAGS} -DBUTTON1=PB4 -o ${TARGET}.bin ${SRCS}

# objcoy:
# 	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex

clean:
	rm -f ../../build/atmega8_2/*.bin ../../build/atmega8_2/*.hex