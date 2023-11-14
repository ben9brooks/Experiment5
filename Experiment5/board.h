/*
 * board.h
 *
 * Created: 9/4/2020 11:57:19 AM
 *  Author: tony tim ben
 */

#ifndef BOARD_H_
#define BOARD_H_

#define F_CPU (16000000UL)

#include <avr/io.h>

#define OSC_DIV (1)


//struct to simplify the offsets of port registers. Pointer to the struct is needed for proper use.
typedef struct port_regs
{
    volatile uint8_t PIN_REG;
    volatile uint8_t DDR_REG;
    volatile uint8_t PORT_REG;
} GPIO_port_t;

// structs defined for each port:
#define PA ( ( volatile GPIO_port_t * ) 0x20 )
#define PB ( ( volatile GPIO_port_t * ) 0x23 )
#define PC ( ( volatile GPIO_port_t * ) 0x26 )
#define PD ( ( volatile GPIO_port_t * ) 0x29 )
#define PE ( ( volatile GPIO_port_t * ) 0x2C )

#define DELAY 10000 

//SPI

typedef struct SPI_regs
{
    volatile uint8_t control_reg;
    volatile uint8_t status_reg;
    volatile uint8_t data_reg;
} SPI_t;

#define SPI0 ( ( volatile SPI_t * ) 0x4C)
#define SPI1 ( ( volatile SPI_t * ) 0xAC)

enum ErrorTypes{
	ERROR_TIMEOUT = 0xFF,
	ERROR_SPI = 0xFE,
	ERROR_SD = 0xFD,
	ERROR_VOLTAGE = 0xFC,
	ERROR_CMD0 = 0xFB,
	ERROR_CMD8 = 0xFA,
	ERROR_CMD58 = 0xF9,
	ERROR_CMD55 = 0xF8,
	ERROR_CMD41 = 0xF7,
	ERROR_CMD41_TIMEOUT = 0xF6,
	ERROR_CMD16 = 0xF5,
	ERROR_TOKEN = 0xF4,
	NO_ERROR = 0x00
	};

#endif