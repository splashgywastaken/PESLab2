include config.txt

CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS_1=-std=c99 -Wall -g -Os -mmcu=${MCU_1} -I.
CFLAGS_2=-std=c99 -Wall -g -Os -mmcu=${MCU_2} -DF_CPU=${MCU_2_F_CPU} -I.	
TARGET_1=./build/mcu_1/mcu_1_build
TARGET_2=./build/mcu_2/mcu_2_build

SRCS_1=./src/mcu_1/main.c
SRCS_2=./src/mcu_2/main.c

all:
	${CC} -DF_CPU=${MCU_1_F_CPU} -DSERIAL_INPUT=${SERIAL_INPUT} -DONOFFBUTTON=${ONOFFBUTTON} -DRBUTTON=${RBUTTON} -DFRELAY=${FRELAY} -DRRELAY=${RRELAY} -DONOFFRELAY=${ONOFFRELAY} ${CFLAGS_1} -o ${TARGET_1}.bin ${SRCS_1}
	${CC} -DF_CPU=${MCU_2_F_CPU} -DSEG_A=${SEG_A} -DSEG_B=${SEG_B} -DSEG_C=${SEG_C} -DSEG_D=${SEG_D} -DSEG_E=${SEG_E} -DSEG_F=${SEG_F} -DSEG_G=${SEG_G} -DDIG1=${DIG1} -DDIG2=${DIG2} -DDIG3=${DIG3} -DINPORT50=${INPORT50} -DINPORT75=${INPORT75} ${CFLAGS_2} -o ${TARGET_2}.bin ${SRCS_2}
	
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET_1}.bin ${TARGET_1}.hex
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET_2}.bin ${TARGET_2}.hex



clean:
	rm -f ../build/mcu_1/*.bin ../build/mcu_1/*.hex
	rm -f ../build/mcu_2/*.bin ../build/mcu_2/*.hex