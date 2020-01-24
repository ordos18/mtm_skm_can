// Doprowadzic do dzialania ponizszy kod
// Wyjsc z programu Can Basic (czyli praca bez Acceptance Filter)
// Pozostac przy wysyBaniu 1 bajt/message
// Przeprowadzic równiez testy ngatywne czyli "zepsuc na chwile kod nadajnika/odbiornika"
// UWAGA: funkcja do odczytu statusu odbiornika can powinna wygladac nast.
//
// unsigned char ucCan2_RxReady(void){
//	 unsigned char ucResult=ucRxReady;
//	 if (ucResult == 1) ucRxReady=0;
//	 return ucResult;
// };
//
// zastanowic sie dlaczego nie tak
//
// unsigned char ucCan2_RxReady(void){
//	 unsigned char ucResult=ucRxReady;
//	 ucRxReady=0;
//	 return ucResult;
// };

#include "led.h"
#include "can.h"
#include "string.h"

char acTxStr[30],acRxStr[30];

unsigned char acc = 0;

int main(){
	Led_Init();
	
	Can1_InitAsTX(acTxStr);
	Can2_InitAsRX(acRxStr);
	
	// pierwsza transmisja
	CopyString("12347",acTxStr);
	Can1_SendString(); 
	while(ucCan2_RxReady()==0){}
		
		//while(1){Led_ShowByte(0x08);}
	if (eCompareString(acTxStr,acRxStr)==EQUAL) Led_ShowByte(0xF0);else Led_ShowByte(0x55);
		//while(1){}
	// to samo tylko z innym lancuchem	
	CopyString("bcdef",acTxStr);	
	Can1_SendString();
	while(ucCan2_RxReady()==0){}
		//while(1){Led_ShowByte(acRxStr[0]);}
	if (eCompareString(acTxStr,acRxStr)==EQUAL) Led_ShowByte(0xF0);else Led_ShowByte(acRxStr[0]);
	
	while(1){}	
}
