
void Can1_InitAsTX (char pcString[]);
void Can2_InitAsRX (char pcString[]);
unsigned char ucCan1_TxFree (void);
unsigned char ucCan2_RxReady (void);
void Can1_SendByte (unsigned char Byte);
void Can1_SendString (void);
unsigned char ucCan2_ReceiveByte (void);
