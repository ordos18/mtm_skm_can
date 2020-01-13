// Doprowadzic do dzialania ponizszy program implementujac zawarte w nim funkcje.

// W funkcjach do inicjalizacji CAN: 
// - wykorzystac rejestry PINSEL, MOD, BTR, TFI1, AFMR, CMR.
// - uzyc bittimingu z przykladu hitexa
// - wylaczyc filtrowanie komunikat�w (�Acceptance filter� powinien "przepuszczac" wszystkie komunkiat)
// - ustawic odpowiednio funkcje pin�w
//
// W pozostalych funkcjach do CAN: 
// - wykorzystac rejestry SR, TID1, TDA1, CMR, RDA.

// UWAGI: 
// - nie modyfikowac zawartosci tego pliku
// - nie dawac komentarzy w kodzie 
// - nie uzywac przerwan
// - miec na uwadze fakt, ze nadajnik CAN sygnalizuje wyslanie komunikatu tylko wtedy kiedy kt�rys z odbiornik�w potwierdzil odbi�r
 
// Test1: Test przeprowadzic uzywajac jednego EVM-a.
//
// Test2: Test przeprowadzic uzywajac dw�ch EVM-�w.
//        - kt�re CAN-y ? 
//        - co i gdzie powinno sie wyswietlac ?
//        - co sie dzieje kiedy zatrzymamy nadajnik resetem ?
//
// Test3: Test przeprowadzic uzywajac trzech EVM-�w.

#include "can.h"
main (){
	unsigned char ucMsgCtr=0;
	Led_Init();
    Can1_InitAsTX();
    Can2_InitAsRX();
    while(1){
        if (ucCan1_TxFree())  Can1_SendByte(ucMsgCtr++);
        if (ucCan2_RxReady()) Led_ShowByte(ucCan2_ReceiveByte());
    }
}
