/*
 * GccApplication4.c
 *
 * Created: 10/29/2022 9:22:37 PM
 * Author : Arosha
 */

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU/16/BAUD)-1)
#define BSIZE 128
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#include "lcd.c"
#include "utils.h"
#include "spi.h"
#include "spi.c"
#include "mfrc522.h"
#include "mfrc522.c"

volatile int rxFlag=0;
void action(void);
void door_action(void);
void door_action_error(void);
uint8_t SelfTestBuffer[64];
int Database_Status =0;

void initUSART(void)
{
	UBRR0H = (unsigned char)(UBRR_VALUE>>8);
	UBRR0L = (unsigned char)(UBRR_VALUE);
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
	
	sei();
}


void transmitUSART(unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

unsigned char receiveUSART(void)
{
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;	
}
 
int main()
{
	_delay_ms(3000);
	LCDInit(LS_BLINK);
	LCDWriteStringXY(2,0,"Starting...");
	_delay_ms(3000);
	EICRA |= 0x03;
	EIMSK |= 0x01;
	sei();

	initUSART();
	volatile unsigned rxData;

	DDRL |= 0B00000011;
	PORTL |= 0B00000001;
	uint8_t byte;
	uint8_t str[MAX_LEN];
	
	LCDWriteStringXY(2,0,"RFID Reader");
	LCDWriteStringXY(5,1,VERSION_STR);
	
	
	spi_init();
	_delay_ms(1000);
	LCDClear();
	
	//init reader
	mfrc522_init();
	
	//check version of the reader
	byte = mfrc522_read(VersionReg);
	if(byte == 0x92)
	{
		LCDWriteStringXY(1,0,"MIFARE RC522v2");
		LCDWriteStringXY(4,1,"Detected");
		LCDCur();
	}else if(byte == 0x91 || byte==0x90)
	{
		LCDWriteStringXY(1,0,"MIFARE RC522v1");
		LCDWriteStringXY(4,1,"Detected");
		LCDCur();
	}else
	{
		LCDWriteStringXY(0,0,"No reader found");
	}

	_delay_ms(1500);
	LCDClear();
	
	while(1){
		
		if (Database_Status==1)
		{
			LCDClear();
			LCDWriteStringXY(1,0,"Show Your Card ");
			LCDCur();   //stop cursor blinking
			byte = mfrc522_read(ComIEnReg);
			mfrc522_write(ComIEnReg,byte|0x20);
			byte = mfrc522_read(DivIEnReg);
			mfrc522_write(DivIEnReg,byte|0x80);
			byte = mfrc522_request(PICC_REQALL,str);
			if(byte == CARD_FOUND)
			{
				byte = mfrc522_get_card_serial(str);
				if(byte == CARD_FOUND)
				{
					_delay_ms(10);
					for(int i=0;i<4;i++)
					{
						rxData =str[i];
						transmitUSART(rxData);
					}
				}
				else
				{
					LCDClear();
					LCDWriteStringXY(5,0,"Error");
				}
			}
		}
		else
		{
			_delay_ms(1000);
			LCDClear();
			LCDWriteStringXY(2,0,"Database Not");
			LCDWriteStringXY(5,1,"Found");
			LCDCur();
		}	
	}
	return 0;
}
void door_action(void)
{
	LCDWriteStringXY(0,1,"Opening Door ...");
	PORTL |= 0B00000010;
	_delay_ms(200);
	PORTL &= 0B00000001;
	_delay_ms(200);
	PORTL |= 0B00000010;
	_delay_ms(400);
	PORTL &= 0B00000000;  //Relay
	_delay_ms(4000);
	LCDWriteStringXY(0,1,"Closing Door ...");
	LCDCur();
	_delay_ms(1000);
	PORTL |= 0B00000001;
}
void door_action_error(void)
{
	LCDWriteStringXY(0,0,"Access denied !");
	LCDWriteStringXY(3,1,"Locked ...   ");
	LCDCur();
	PORTL |= 0B00000010;  //Buzzer
	_delay_ms(300);
	PORTL &= 0B00000001;
	_delay_ms(200);
	PORTL |= 0B00000010;
	_delay_ms(300);
	PORTL &= 0B00000001;
	_delay_ms(200);
	PORTL |= 0B00000010;
	_delay_ms(300);
	PORTL &= 0B00000001;
	_delay_ms(200);
	PORTL |= 0B00000010;
	_delay_ms(300);
	PORTL &= 0B00000001;
}
void action(void)
{
	volatile unsigned rxData2;
	rxData2 = receiveUSART();
	
	if(rxData2 == 51)
	{
		Database_Status = 1;
	}
	else
	{
		if(rxData2 == 49)
		{
			LCDClear();
			LCDWriteStringXY(4,0,"UNLOCKED");
			door_action();
		}
		else
		{
			if (rxData2 == 50)
			{
				LCDClear();
				LCDWriteStringXY(3,0,"LOCKED");
				door_action_error();
			}
			else
			{
				LCDClear();
				Database_Status = 1;
			}
			
		}
	}
}
ISR (USART0_RX_vect)
{
	action();
}