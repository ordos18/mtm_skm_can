#include <LPC21xx.H>
#include "can.h"

// PINSEL0 Pin Connect Block
#define mCAN_RD2		(1 << 14)
#define mCAN_TD2		(1 << 16)
#define mCAN_RD1		(1 << 18)

#define mLEDs			(0xFF << 16)

#define mAFMR_ACC_OFF	(1 << 0)
#define mAFMR_ACC_BP	(1 << 1)

#define mMOD_RM			(1 << 0)

#define mBITTIMING		0x001C001D

#define mSR_RBS			(1 << 0)
#define mSR_TBS1		(1 << 2)

#define mTFI1_DLC		(1 << 16)

#define mCMR_TR			(1 << 0)
#define mCMR_RRB		(1 << 2)

#define mTID1			0x00000022

void Led_Init (void) {
	
	IO1DIR = IO1DIR | mLEDs;
}

void Can1_InitAsTX (void) {
	
	C1MOD	= mMOD_RM;
	PINSEL1 = PINSEL1 | mCAN_RD1;
	
	AFMR	= mAFMR_ACC_BP;
	C1BTR	= mBITTIMING;
	C1TFI1	= mTFI1_DLC;
	
	C1MOD	= C1MOD & ~(mMOD_RM);
}

void Can2_InitAsRX (void) {
	
	C2MOD	= mMOD_RM;
	
	PINSEL1 = PINSEL1 | (mCAN_RD2 | mCAN_TD2);
	C2BTR 	= mBITTIMING;
	
	C2MOD 	= C2MOD & ~(mMOD_RM);
}

unsigned char ucCan1_TxFree (void) {
	
	return (C1SR & mSR_TBS1);
}

unsigned char ucCan2_RxReady (void) {
	
	return (C2SR & mSR_RBS);
}

void Can1_SendByte (unsigned char ucData) {
	
	C1TID1 = mTID1;
	C1TDA1 = ucData;
	C1CMR = mCMR_TR;
}

unsigned char ucCan2_ReceiveByte (void) {
	
	unsigned char ucReceivedByte;
	
	ucReceivedByte = C2RDA & 0x000000FF;
	C2CMR = mCMR_RRB;
	
	return ucReceivedByte;
}

void Led_ShowByte (unsigned char Byte) {
	
	IO1CLR = mLEDs;
	IO1SET = Byte << 16;
}


