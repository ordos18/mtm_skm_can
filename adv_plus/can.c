#include <LPC21xx.H>
#include "can.h"
#include "string.h"
#include "led.h"

#define NULL '\0'

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

#define mIER_RX			(1 << 0)
#define mIER_TX			(1 << 1)

#define VIC_CAN1_TX_CHANNEL_NR  20
#define VIC_CAN2_RX_CHANNEL_NR  27
#define mIRQ_SLOT_ENABLE    0x00000020

#define mTID1			0x00000022

unsigned char ucRxReady = 0, ucc = 0;
char *pcRx, *pcTx;
unsigned char ucOffsetRx = 0, ucOffsetTx = 0;


//----------------------TX-------------------------//
void Can1_SendString (void) {
	
	while(0 == ucCan1_TxFree()) {}
	ucOffsetTx = 0;
	Can1_SendByte(pcTx[0]);
}

void Can1_SendByte (unsigned char ucData) {
	
	C1TID1 = mTID1;
	C1TDA1 = ucData;
	C1CMR = mCMR_TR;
}

__irq void CAN1_TX_Interrupt (void) {
	
	if(NULL != pcTx[ucOffsetTx]) {
		ucOffsetTx++;
		Can1_SendByte(pcTx[ucOffsetTx]);
	}
	C1ICR;
	VICVectAddr = 0; // Acknowledge Interrupt
}



//----------------------RX-------------------------//

__irq void CAN2_RX_Interrupt (void) {
	
	pcRx[ucOffsetRx] = ucCan2_ReceiveByte();
	
	if(NULL == pcRx[ucOffsetRx]) {
		ucRxReady = 1;
		ucOffsetRx = 0;
	} else {
		ucRxReady = 0;
		ucOffsetRx++;
	}
	C2ICR;
	VICVectAddr = 0; // Acknowledge Interrupt
	Led_ShowByte(0x05);
}

void Can1_InitAsTX (char pcString[]) {
	
	C1MOD	= mMOD_RM;
	PINSEL1 = PINSEL1 | mCAN_RD1;
	C1BTR	= mBITTIMING;
	C1TFI1	= mTFI1_DLC;
	C1IER	= mIER_TX;
	
	VICVectAddr1  = (unsigned long) CAN1_TX_Interrupt;
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_CAN1_TX_CHANNEL_NR;
	VICIntEnable |= (0x1 << VIC_CAN1_TX_CHANNEL_NR);
	
	C1MOD	= C1MOD & ~(mMOD_RM);
	
	pcTx = pcString;
}

void Can2_InitAsRX (char pcString[]) {
	
	C2MOD	= mMOD_RM;
	
	AFMR	= mAFMR_ACC_BP;
	PINSEL1 = PINSEL1 | (mCAN_RD2 | mCAN_TD2);
	C2BTR 	= mBITTIMING;
	C2IER	= mIER_RX;
	
	VICVectAddr0  = (unsigned long) CAN2_RX_Interrupt;
	VICVectCntl0  = mIRQ_SLOT_ENABLE | VIC_CAN2_RX_CHANNEL_NR;
	VICIntEnable |= (0x1 << VIC_CAN2_RX_CHANNEL_NR);
	
	C2MOD 	= C2MOD & ~(mMOD_RM);
	
	pcRx = pcString;
}

unsigned char ucCan1_TxFree (void) {
	
	return (C1SR & mSR_TBS1);
}

unsigned char ucCan2_RxReady (void) {
	
	unsigned char ucResult=ucRxReady;
	if (ucResult == 1) ucRxReady=0;
	return ucResult;
}

unsigned char ucCan2_ReceiveByte (void) {
	
	unsigned char ucReceivedByte;
	
	ucReceivedByte = C2RDA & 0x000000FF;
	C2CMR = mCMR_RRB;
	
	return ucReceivedByte;
}
