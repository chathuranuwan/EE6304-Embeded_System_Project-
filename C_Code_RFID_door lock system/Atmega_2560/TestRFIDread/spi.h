
#ifndef SPI_H
#define SPI_H

#include "spi_config.h"
#include "stdint.h"
//#include "spi2.h"

void spi_init();
uint8_t spi_transmit(uint8_t data);

#define ENABLE_CHIP() (SPI_PORT &= (~(1<<SPI_SS)))
#define DISABLE_CHIP() (SPI_PORT |= (1<<SPI_SS))


#endif
