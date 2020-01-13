
void Can1_InitAsTx (void);
void Can2_InitAsRx (unsigned int uiID);
unsigned char ucCan1_TxFree (void);
unsigned char ucCan2_RxReady (void);
void Can1_SendByte (unsigned int uiID, unsigned char ucByte);
unsigned char ucCan2_ReceiveByte (void);
