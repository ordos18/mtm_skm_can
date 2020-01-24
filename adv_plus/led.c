#include <LPC21xx.H>
#include "led.h"

#define mLEDs			(0xFF << 16)

void Led_Init (void) {
	
	IO1DIR = IO1DIR | mLEDs;
}

void Led_ShowByte (unsigned char Byte) {
	
	IO1CLR = mLEDs;
	IO1SET = Byte << 16;
}
