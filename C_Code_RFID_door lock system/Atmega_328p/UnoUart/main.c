/*
 * Test1_Slave.c
 *
 * Created: 10/31/2022 12:13:02 AM
 * Author : Manoj Dissanayake
 */ 

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU/16/BAUD)-1)
#define BSIZE 128

#define LOCKED 2
#define UNLOCKED 3
#define rowCount 4

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

volatile int rxFlag=0;

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


int main(void)
{
	EICRA |= 0x03;
	EIMSK |= 0x01;
	sei();

	initUSART();
	unsigned char rxByte;
	uint8_t door_status = 0;
	unsigned char arrNew[1][4];
	unsigned char arr[rowCount][4] = {{0xCA,0x57,0x00,0x78},{0x01,0x02,0x03,0x04},{0x03,0x5C,0x34,0x3F},{0x87,0x05,0xAB,0xB4}};
	int i=0;
	int byte=0;
	int x=0;
	_delay_ms(3000);
	transmitUSART('3');
	while (1)
	{
		
		if(rxFlag)
		{	
			rxFlag = 0;
			rxByte=receiveUSART();
			arrNew[0][i]=rxByte;
			i++;	
		}
		
		if(i==4)
		{
			door_status = 0;
			for(x=0;x<rowCount;x++)
			{
				for(byte=0;byte<4;byte++)
				{
					if(arr[x][byte] != arrNew[0][byte])
					{
						byte=0;
						break;
					}				
				}
				if(byte == 4)
				break;
			}
			if(byte == 4)
			door_status = UNLOCKED;
			switch(door_status)
			{
				case UNLOCKED :
				{
					rxByte = '1';
					transmitUSART(rxByte);
					break;
				}
				default:
				{
					rxByte = '2';
					transmitUSART(rxByte);
					break;
				}
			}
			i=0;
		}
	}
	return 0;
}


ISR (USART_RX_vect)
{
	rxFlag =1;
}