

#include <avr/io.h>

#include <util/delay.h>

#include "myutils.h"

#ifndef _LCD_H
#define _LCD_H



/*_________________________________________________________________________________________*/

/************************************************
	LCD CONNECTIONS
*************************************************/

#define LCD_DATA A			//Port PD0-PD3 are connected to D4-D7
#define LCD_DATA_POS 0	

#define LCD_E A 			//Enable/strobe signal
#define LCD_E_POS PA4		//Position of enable in above port

#define LCD_RS A			//RS SIGNAL
#define LCD_RS_POS PA5	

#define LCD_RW A			//RW SIGNAL
#define LCD_RW_POS PA6	

/***********************************************

LCD Type Selection
Uncomment Just one of them

************************************************/

//#define LCD_TYPE_202	//For 20 Chars by 2 lines

//#define LCD_TYPE_204	//For 20 Chars by 4 lines

#define LCD_TYPE_162	//For 16 Chars by 2 lines

//#define LCD_TYPE_164	//For 16 Chars by 4 lines


//************************************************

	


//************************************************

#define LS_BLINK 0x01
#define LS_ULINE 0x02
#define LS_NONE	 0B00000000



/***************************************************
			F U N C T I O N S
****************************************************/



void LCDInit(uint8_t style);
void LCDWriteString(const char *msg);
void LCDWriteInt(int val,unsigned int field_length);
void LCDGotoXY(uint8_t x,uint8_t y);
void LCDHexDumpXY(uint8_t x, uint8_t y,uint8_t d);

//Low level
void LCDByte(uint8_t,uint8_t);
#define LCDCmd(c) (LCDByte(c,0))
#define LCDData(d) (LCDByte(d,1))

void LCDBusyLoop();





/***************************************************
			F U N C T I O N S     E N D
****************************************************/


/***************************************************
	M A C R O S
***************************************************/
#define LCDClear() LCDCmd(0x01);
#define LCDHome() LCDCmd(0x02);
#define LCDCur() LCDCmd(0x0C);

#define LCDWriteStringXY(x,y,msg) {\
 LCDGotoXY(x,y);\
 LCDWriteString(msg);\
}

#define LCDWriteIntXY(x,y,val,fl) {\
 LCDGotoXY(x,y);\
 LCDWriteInt(val,fl);\
}
/***************************************************/




/*_________________________________________________________________________________________*/
#endif




