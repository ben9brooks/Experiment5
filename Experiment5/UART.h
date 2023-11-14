/*
 * UART.h
 *
 * Created: 5/20/2010 11:57:19 AM
 * Author: tony tim ben
 */

#ifndef UART_H_
#define UART_H_

#include "board.h"

typedef struct UART
{
	volatile uint8_t UART_UCSRA;
	volatile uint8_t UART_UCSRB;
	volatile uint8_t UART_UCSRC;
	volatile uint8_t UART_UCSRD;
	volatile uint8_t UART_UBRRL;
	volatile uint8_t UART_UBRRH;
	volatile uint8_t UART_UDR;
} UART_t;

//uint32_t Calc_Baudrate(uint32_t baud_rate, uint8_t UtwoX);

void UART_init(volatile UART_t *UART_addr, uint16_t baud_rate);

void UART_transmit(volatile UART_t *UART_addr, uint8_t data);

char UART_receive(volatile UART_t * UART_addr);


#define BAUD_RATE (9600)

/***** UARTS ******/

#define UART0 ((volatile UART_t *) (0xC0U))
#define UART1 ((volatile UART_t *) (0xC8U))
#define UART2 ((volatile UART_t *) (0xD0U))

// UCSRA flags
#define RXCn 7  // RX Complete flag bit position in UCSRA

#define U2X_Val 0
//UBRR? mode and frame settings
#define async_mode_1 (0<<6)
#define async_mode_2 (0<<7)
#define sync_mode (1<<6)
#define SBI_mode (3<<6)
#define no_parity_1 (0<<4)
#define no_parity_2 (0<<5)
#define even_parity (2<<4)
#define odd_parity (3<<4)
#define one_stop_bit (0<<3)
#define two_stop_bits (1<<3)
#define eight_bit_data (3<<1)

//UCSRB
#define RXEN_en (1<<4)
#define RXEN_dis (0<<4)
#define TXEN_en (1<<3)
#define TXEN_dis (0<<3)
#define RXCIE_dis (0<<7)    
#define TXCIE_dis (0<<6)
#define UDRIE_dis (0<<5)


#endif