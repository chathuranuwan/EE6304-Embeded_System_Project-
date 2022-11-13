
#ifndef SPI_CONFIG_H
#define SPI_CONFIG_H

#include <avr/io.h>

/*
 * Set to 1, spi api will work in master mode
 * else in slave mode
 */
#define SPI_CONFIG_AS_MASTER 	1


/*
 * Config SPI pin diagram
 */
#define SPI_DDR		DDRB
#define SPI_PORT	PORTB
#define SPI_PIN		PINB
#define SPI_MOSI	PB2
#define SPI_MISO	PB3
#define SPI_SS		PB0
#define SPI_SCK		PB1

#endif
