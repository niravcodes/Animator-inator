#! /bin/bash

avr-gcc $1 -Os -std=c99 -mmcu=atmega32a -o .phaseone
avr-objcopy -O ihex -j .text -j .data .phaseone .phasetwo

echo "Upload"
read a

if [ "$a" == "Y" ]
then
	avrdude -c avrisp -P /dev/ttyACM0 -b 19200 -p m32 -U flash:w:.phasetwo
fi 
