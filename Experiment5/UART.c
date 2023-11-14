#include "UART.h"

/*
uint32_t Calc_Baudrate(uint32_t baud_rate, uint8_t UtwoX)
{
    //return (uint32_t)(((F_CPU/OSC_DIV)/(8*(2-U2X)*baud_rate))-1);

    return (uint32_t)((((F_CPU/OSC_DIV)+(4UL*(2-UtwoX)*baud_rate))/(8UL*(2-UtwoX)*baud_rate))-1);
}
*/

void UART_init(volatile UART_t *UART_addr, uint16_t baud_rate)
{
// Calculate the baud rate reload value using the constants F_CPU,
// OSC_DIV and U2X_bit. Write the values to UBRRH, UBRRL and set or
// clear the U2X bit in UCSRA.
    //uint16_t UBRR_value = Calc_Baudrate(baud_rate, U2X_Val); //Where do we get U2X
    //uint16_t UBRR_value = (uint32_t)((((F_CPU / OSC_DIV) + (4UL * (2 - U2X_Val) * baud_rate)) / (8UL * (2 - U2X_Val) * baud_rate)) - 1);
    
    uint16_t UBRR_value = 103;

    UART_addr->UART_UBRRH = UBRR_value / 256;
	UART_addr->UART_UBRRL = UBRR_value % 256; 
    UART_addr->UART_UCSRA = (U2X_Val<<U2X); //set u2x here. currently off (0)

// Determine the value for UCSRC using the settings for number of bits,
// parity and number of stop bits and write this value. In my solution, I
// defined constants for these settings to make the code more readable. You
// could also make sure your comments explain the value written to the
// register.
    UART_addr->UART_UCSRC = 0x00; // cleared 
	UART_addr->UART_UCSRC = (async_mode_1 | async_mode_2 | no_parity_1 | no_parity_2 | one_stop_bit | eight_bit_data);

// Enable the transmitter and receiver with interrupts disabled by writing the
// appropriate value to UCSRB 
	UART_addr->UART_UCSRB = 0x00; //cleared
    UART_addr->UART_UCSRB = (RXCIE_dis | TXCIE_dis | UDRIE_dis | RXEN_en | TXEN_en);
    return; 
}

void UART_transmit(volatile UART_t *UART_addr, uint8_t data)
{
	
	uint8_t status = 0 ;
	do 
	{
		status = (uint8_t)(UART_addr->UART_UCSRA & (1<<UDRE)); 
		
	} while (status != (1<<UDRE));
	
    //2: write val to UDR
    UART_addr->UART_UDR = data;
    return;
}


char UART_receive(volatile UART_t * UART_addr)
{
	uint8_t status;
	do 
	{
		status=(UART_addr->UART_UCSRA);
	} while ((status & (1 << RXC))!=(1 << RXC));
	return (UART_addr->UART_UDR);  // return the value in the receive buffer
}