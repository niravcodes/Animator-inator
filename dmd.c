#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "matrixdata.h"


#define HEIGHT 16 
#define WIDTH 64

#define SHIFTPORT PORTB
#define SHIFTPORTDDR DDRB

#define OUTPUTENABLE 6
#define SELA 1
#define SELB 2
#define CLK 3
#define LATCH 4
#define DATA 5

#define setpinhigh(pin) SHIFTPORT = SHIFTPORT | (1<<pin)
#define setpinlow(pin) SHIFTPORT = SHIFTPORT & ~(1<<pin)


unsigned char display[HEIGHT][WIDTH/8] = {
	0xff, 0xff, 0xff,0Xff, 0xff, 0xff, 0xff,0Xff,
	0xff, 0xff, 0xff,0Xff, 0xff, 0xff, 0xff,0Xff,
	0xff, 0xff, 0xff,0XFF, 0xff, 0xff, 0xff,0XFF,
	0xff, 0xff, 0xff,0XFF, 0xff, 0xff, 0xff,0XFF,
	0xff, 0xff, 0xff,0XFF, 0xff, 0xff, 0xff,0XFF,
	0xff, 0xff, 0xff,0XFF, 0xff, 0xff, 0xff,0XFF,
	0xff, 0xff, 0xff,0XFF, 0xff, 0xff, 0xff,0XFF,
	0xff, 0xff, 0xff,0XFF, 0xff, 0xff, 0xff,0XFF,
	0xff, 0xff, 0xff,0Xff, 0xff, 0xff, 0xff,0Xff,
	0xff, 0xff, 0xff,0Xff, 0xff, 0xff, 0xff,0Xff, 
	0xff, 0xff, 0xff,0XFF, 0xff, 0xff, 0xff,0XFF, 
	0xff, 0xff, 0xff,0XFF, 0xff, 0xff, 0xff,0XFF,
	0xff, 0xff, 0xff,0XFF, 0xff, 0xff, 0xff,0XFF,
	0xff, 0xff, 0xff,0XFF, 0xff, 0xff, 0xff,0XFF,
	0xff, 0xff, 0xff,0XFF, 0xff, 0xff, 0xff,0XFF,
	0xff, 0xff, 0xff,0XFF, 0xff, 0xff, 0xff,0XFF
};



void setline(char line, char bool){
	if (bool)
		setpinhigh(line);
	else
		setpinlow(line);
}


void disableoutput(){
	setline(OUTPUTENABLE, 0);
}

void enableoutput(){
	setline(OUTPUTENABLE, 1);	
}

/*
	Sends a high followed by a low in the pin specified by outpin
             	   ____
	___(low)__|high|__(low)___
*/
void sendpulse(char outpin){
	setpinhigh(outpin);
	setpinlow(outpin);
	
}

/*
	inputs one bit into the shift register
	1. first, data line is set to the bit
	2. second, the clock is pulsed 
*/
void clockbit(char bit){
	if (bit)
		setpinhigh(DATA);
	else
		setpinlow(DATA);

	sendpulse(CLK);
	setpinlow (DATA);
}

/*
	sends one byte of data to dot matrix
*/
void clockbyte(char byte){
	for (unsigned char i=0x80; i != 0; i = i >> 1){
		if (byte & i)
			setpinlow(DATA);
		else
			setpinhigh(DATA);
		sendpulse (CLK);
	}
	setpinlow(DATA);

}


//sets the selector pins A and B
void setselector(char select){
	//if (select > 3) return;
	setline(SELA, select & 0x01);
	setline(SELB, select & 0x02);
}



/*
	clocks in one full multiplex unit from the display
	sel is the multiplex selector(0,1,2,3)

	void clocksel(char sel){
		for (int i = 0 ; i < (WIDTH>>3);i++){		//4
			for (int j = (HEIGHT>>2) - 1; j >= 0; j--){			//4
				clockbyte(display[j<<2 + sel][ i ]);
		}
	}
}*/


void clocksel(char sel){
	disableoutput();
	for (int i = 0 ; i < (WIDTH/8); i++){		//4
		for (int j = 3; j >= 0; j--){			//4
			clockbyte(display[j*4 + sel ][ i ]);
		}
	}
	setselector(sel);
	sendpulse(LATCH);
	enableoutput();
}


//clears screen to black BY shifting in zero
//as opposed to simply turning on the OE line
//which achieves the same effect
void clearscreen(){
	int count = 0;
	setpinlow(DATA);
	while (count < (HEIGHT * WIDTH/4)){
		sendpulse(CLK);
		count++;
	}

}


void init(){
	//set ddr for PORT
	SHIFTPORTDDR = 0XFF;

	//set all pins to default states
	setpinhigh(OUTPUTENABLE);
	setpinlow(SELA);
	setpinlow(SELB);
	setpinlow(CLK);
	setpinlow(LATCH);
	setpinlow(DATA);
	
	//clear once 
	clearscreen();
}


/*
void assigndisplay(const unsigned char newdisp[HEIGHT][WIDTH/8]){
	unsigned char byte;
	for (unsigned char i = 0; i < HEIGHT; i++){
		for (unsigned char j = 0; j < WIDTH/8; j++){
			byte = pgm_read_byte(&(newdisp[i][j]));
			display[i][j] = byte; 
		}
	}
}
*/


// reads frame data from PROGMEM in the Header
// and puts it into RAM
void newassigndisplay(int frameno){
	unsigned char byte;
	for (unsigned char i = 0; i < HEIGHT; i++){
		for (unsigned char j = 0; j < WIDTH/8; j++){
			byte = pgm_read_byte(&(animation[frameno][i][j]));
			display[i][j] = byte; 
		}
	}
}

int main(){
	init();
	OCR0 = 0xff;
	TIMSK = 2;
	TCCR0 = 0X0b;
	sei();

	int frame = 0;
	while (1){
		cli();
		disableoutput();
		newassigndisplay(frame++);
		enableoutput();
		sei();
		if (frame >= framecount) frame = 0;
		_delay_ms(6);
	}

		
}

int rowcount_internal = 0;

ISR(TIMER0_COMP_vect ){
	clocksel(rowcount_internal++);
	if (rowcount_internal > 3){
		rowcount_internal = 0;
	}
}
