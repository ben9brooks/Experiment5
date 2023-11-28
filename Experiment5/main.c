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
	uint8_t numEntries = 0;
	uint32_t userDirNum = 0;
	uint32_t userClusNum = 0;
	uint32_t userSecNum = 0;
	uint32_t firstSecOfClus = 0;
	
	/**********************************
	*
	* Experiment 2 Initializations
	*
	***********************************/
	//UART_init(UART0, BAUD_RATE);
	UART_init(UART1, BAUD_RATE);
	//UART_init(UART2, BAUD_RATE);

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
	
	FS_values_t* accessor_fileSystem = export_drive_values();
	FS_values_t file_system;
	mount_drive(&file_system);
	
	*accessor_fileSystem = file_system;
	
	uint32_t FirstRootDirSector = first_sector(0);
	

	
	//SPI can be reinitialized at a faster freq, now that the SD has been initialized.
	SPI_master_init(SPI0, 8000000U);
	
	UART_transmit_string(UART1, "SD initialized\n", 15);

	// to debug SPI_transmit on MSO: Trigger Menu: Type=Edge, Source=D0, Slope=All, Level=1.51, Normal
	while (1)
	{
		//UART_transmit_string(UART1, "Input Block Number:\n", 21);
		//mem_block_num = long_serial_input(UART1);
		//UART_transmit_string(UART1, "Reading Block...\n", 18);
		//typederror = read_sector(mem_block_num, 512, mem_block);
		
		typederror = read_sector(FirstRootDirSector, 512, mem_block);
		
		if(typederror != 0)
		{
			display_error(UART1, typederror);
			break;
		}
		//print block
		
		//print_memory(mem_block, 512);
		
		//uint8_t temp8 = read_value_8(0, mem_block);
		//uint16_t temp16 = read_value_16(2, mem_block);
		//uint32_t temp32 = read_value_32(2, mem_block);
		//char str[64];
		//sprintf(str, "test: %lX\n", temp32);
		//UART_transmit_string(UART1, str, 64);
		userDirNum = FirstRootDirSector;
		while(1)
		{
			numEntries = print_directory(userDirNum, mem_block);
			UART_transmit_string(UART1, "Entry Number:\n", 14);
			userDirNum = long_serial_input(UART1);
			
			while(userDirNum > numEntries)
			{
				UART_transmit_string(UART1, "Invalid Entry Number. Provide a new one:\n", 41);
				userDirNum = long_serial_input(UART1);
			}
			
			userClusNum = read_dir_entry(FirstRootDirSector, userDirNum, mem_block);
			char str[64];
			sprintf(str, "test: %lX\n", userClusNum);
			UART_transmit_string(UART1, str, 64);
			//Directory:
			if((userClusNum & 0x10000000) != 0)
			{
				userClusNum &= 0x0FFFFFFF; //mask upper 4 off
				userDirNum = first_sector(userClusNum);
				//memset(mem_block,0, 512);
				//read_sector(userSecNum, 512, mem_block);
				//print_directory(userSecNum, mem_block);
			}
			//File
			else
			{
				
				//print_directory(userClusNum, mem_block);
				//read_sector(userClusNum, 512, mem_block);
				print_file(userClusNum, mem_block);
				
			}	
		}
	}
	
	UART_transmit_string(UART1, stop, 5);
	UART_transmit(UART1, '\n');
	return 0;
}
