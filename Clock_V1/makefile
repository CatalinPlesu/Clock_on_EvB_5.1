MCU = atmega32
PROGRAMMER = usbasp 
PORT = usb
	# PROGRAMMER = avrisp
	# PORT = /dev/ttyUSB0
CC = avr-gcc 
CFLAGS = -Wall -Os -mmcu=$(MCU) -funsigned-char -funsigned-bitfields -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -std=gnu99 
# CFLAGS = -Wall -Os -mmcu=$(MCU) 

SRC = main
FOLDERS={I2c,Ds18b20,Adc,Rtc,Timer,Led,SevSeg,Button}
DEPS = $(FOLDERS)/*.c
# DEPS =
INCLUDE = -I$(FOLDERS)
# INCLUDE = 

all: compile install clean

compile:
	$(CC) $(CFLAGS) $(INCLUDE) -o $(SRC).bin $(SRC).c $(DEPS)
	avr-objcopy -j .text -j .data -O ihex $(SRC).bin $(SRC).hex

install: 
	# sudo avrdude -p atmega32 -c $(PROGRAMMER)  -U flash:w:$(SRC).hex:i -F -P usb
	sudo avrdude -p $(MCU) -P $(PORT) -c $(PROGRAMMER) -U flash:w:$(SRC).hex:i 

clean:
	rm *.bin *.hex
