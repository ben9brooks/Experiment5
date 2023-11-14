/*
 * main.c
 *
 * Created: 8/24/2023 5:26:61 PM
 * Author : tony tim ben
 */ 

//#include <avr/io.h>
#include "board.h"
#include <util/delay.h>
#include "UART.h"
#include <stdio.h>
#include "SD.h"
#include "SPI.h"
#include "gpio_output.h"
#include "UART_Print.h"
#include "Long_Serial_In.h"
#include "sd_read.h"

int main(void)
{
	/**********************************
	*
	* Experiment 2 Initializations
	*
	***********************************/
	UART_init(UART0, BAUD_RATE);
	UART_init(UART1, BAUD_RATE);
	UART_init(UART2, BAUD_RATE);

	/**********************************
	*
	* Experiment 3 Initializations
	*
	***********************************/
	
	// 400KHz used for the first init, which is the max hz for now.
	SPI_master_init(SPI0, 400000U); // port 0 is used for SD card, OLED, MP3
	uint32_t mem_block_num;
	uint8_t mem_block[512];
	enum ErrorTypes typederror = 0;
	
	// debug strings
	char start[] = "Start\n";
	char stop[] = "Stop\n";
	
	// initialize SS AKA CS
	GPIO_Output_Init(PB, (1<<4));
	
	UART_transmit_string(UART1, start, 6);
	
	do
	{
		typederror = SD_init(SPI0);
		if (typederror != 0)
		{
			display_error(UART1, typederror);
		}
	}while(typederror != 0);
	
	//SPI can be reinitialized at a faster freq, now that the SD has been initialized.
	SPI_master_init(SPI0, 8000000U);
	
	UART_transmit_string(UART1, "SD initialized\n", 15);

	// to debug SPI_transmit on MSO: Trigger Menu: Type=Edge, Source=D0, Slope=All, Level=1.51, Normal
	while (1)
	{
		UART_transmit_string(UART1, "Input Block Number:\n", 21);
		mem_block_num = long_serial_input(UART1);
		UART_transmit_string(UART1, "Reading Block...\n", 18);
		//set SD low/active
		SD_CS_active(PB, (1<<4));
		// read block with CMD17
		typederror = read_sector(mem_block_num, 512, mem_block);
		//send_command(SPI0, CMD17, mem_block_num);
		//typederror = read_block(SPI0, 512, mem_block);
		if(typederror != 0)
		{
			display_error(UART1, typederror);
			break;
		}
		//print block
		print_memory(mem_block, 512);
		
	}
	
	UART_transmit_string(UART1, stop, 5);
	UART_transmit(UART1, '\n');
	return 0;
}
