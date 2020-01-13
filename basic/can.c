#include <LPC21xx.H>
#include "can.h"

// PINSEL0 Pin Connect Block
#define mCAN_RD2		(1 << 14)
#define mCAN_TD2		(1 << 16)
#define mCAN_RD1		(1 << 18)

#define mLEDs			(0xFF << 16)

// W funkcjach do inicjalizacji CAN: 
// - wykorzystac rejestry PINSEL, MOD, BTR, TFI1, AFMR, CMR.
// - uzyc bittimingu z przykladu hitexa
// - wylaczyc filtrowanie komunikatów („Acceptance filter” powinien "przepuszczac" wszystkie komunkiat)
// - ustawic odpowiednio funkcje pinów
//
// W pozostalych funkcjach do CAN: 
// - wykorzystac rejestry SR, TID1, TDA1, CMR, RDA.

void Led_Init (void) {
	
	IO1DIR = IO1DIR | mLEDs;
}

void Can1_InitAsTX (void) {
	
	C1MOD	= 0x00000001;
	PINSEL1 = PINSEL1 | mCAN_RD1;
	
	AFMR	= 0x00000002;
	C1BTR	= 0x001C001D;
	C1TFI1	= 0x00010000;
	C1CMR	= 0x00000020;
	
	C1MOD	= 0x00000000;
}

void Can2_InitAsRX (void) {
	
	C2MOD	= 0x00000001;
	
	PINSEL1 = PINSEL1 | (mCAN_RD2 | mCAN_TD2);
	C2BTR 	= 0x001C001D;
	
	C2MOD 	= 0x00000000;
}

unsigned char ucCan1_TxFree (void) {
	
	return (C1SR & 0x00000004);
}

unsigned char ucCan2_RxReady (void) {
	
	return (C2SR & 0x00000001);
}

void Can1_SendByte (unsigned char ucData) {
	
	C1TID1 = 0x00000022;
	C1TDA1 = ucData;
	C1CMR = 0x00000001;
}

unsigned char ucCan2_ReceiveByte (void) {
	
	unsigned char ucReceivedByte;
	
	ucReceivedByte = C2RDA & 0x000000FF;
	C2CMR = 0x00000004;
	
	return ucReceivedByte;
}

void Led_ShowByte (unsigned char Byte) {
	
	IO1CLR = mLEDs;
	IO1SET = Byte << 16;
}


