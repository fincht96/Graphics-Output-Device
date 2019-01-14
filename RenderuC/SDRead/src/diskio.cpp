/*
 * diskio.cpp
 *
 * Created: 21/04/2018 22:39:53
 *  Author: Tom
 */ 

#include "diskio.h"
#include "device_interface.h"


#ifndef F_CPU					// if F_CPU was not defined in Project -> Properties
#define F_CPU 20000000UL			// define it now as 1 MHz unsigned long
#endif

#include <avr/io.h>				// this is always included in AVR programs
#include <util/delay.h>
#include <avr/interrupt.h>


#define SCK PORTB7
#define MISO PORTB6
#define MOSI PORTB5
#define SS_COMP PORTB4


#define CMD_LENGTH_BYTES 4
#define SECTOR_SIZE 512


DeviceInterface pDev_spi;


// ENSURE OPERATING VOLTAGE OF BETWEEN 2.7V and 3.6V


// converts a uint32 into 4 individual uint8 bytes
void convert_sector_addr(DWORD addr, BYTE sd_cmd_sector_addr[])
{
	sd_cmd_sector_addr[3] = (BYTE)(addr >> 24);
	sd_cmd_sector_addr[2] = (BYTE)(addr >> 16);
	sd_cmd_sector_addr[1] = (BYTE)(addr >> 8);
	sd_cmd_sector_addr[0] = (BYTE)(addr);
}


// initializes SPI module on the micro controller
void SpiMasterInit(void)
{
		// sets SS as output
		pDev_spi.setPinDirec('B', 4, high);
		
		// sets MOSI as output
		pDev_spi.setPinDirec('B', 5, high);
		
		// sets SCK as output
		pDev_spi.setPinDirec('B', 7, high);
	
		// spi control register
		SPCR0 = (1<<SPE0)|(1<<MSTR0)|(3<<CPHA0) | (1 << SPI2X0) | (0 << SPR10) | (0 << SPR00); //SPI Master, F_CPU/256, mode#3
	
		// deselect SS as output
		pDev_spi.setPinState('B', 4, high);
	
		// sets MOSI high
		pDev_spi.setPinState('B', 5, high);
	
}


// sends an 8 bit SPI message
void SpiMasterTransmit(unsigned uData)
{
	// start transmission, writes to the data register
	SPDR0 = uData;
	
	// wait for transmission to complete, checks if SPSR is empty and if serial transmission flag is clear
	while(!(SPSR0 & (1<<SPIF0)));
}



// executes SD card initialization sequence
void sdCardInitialize()
{
	// SD INITIALIZATION BEGIN
	// =========================================================================
	
	// initializes SPI on Atmega328p @ 250 KHz
	SpiMasterInit();
	
	
	// deselect SS as output
	pDev_spi.setPinState('B', 4, high);
	
	
	// applies 80 clock pulses
	for(int i = 0; i < 10; i++)
	{
		SpiMasterTransmit(0xFF);
	}
	
	
	// select SS as output
	pDev_spi.setPinState('B', 4, low);
	
	
	// transmits COMMAND 0 to SD card until the response 0x01 is received (Card is in idle state)
	
	while(SPDR0 != 0x01)
	{
		SpiMasterTransmit(0x40);
		SpiMasterTransmit(0x00);
		SpiMasterTransmit(0x00);
		SpiMasterTransmit(0x00);
		SpiMasterTransmit(0x00);
		SpiMasterTransmit(0x95);
		
		// dummy bytes
		SpiMasterTransmit(0xFF);
		SpiMasterTransmit(0xFF);
	}
	
	
	// Command 8
	SpiMasterTransmit(0xFF);
	SpiMasterTransmit(0xFF);
	
	SpiMasterTransmit(0x48);
	SpiMasterTransmit(0x00);
	SpiMasterTransmit(0x00);
	SpiMasterTransmit(0x01);
	SpiMasterTransmit(0xAA);
	SpiMasterTransmit(0x87);
	
	// dummy bytes
	SpiMasterTransmit(0xFF);
	SpiMasterTransmit(0xFF);

	_delay_ms(10);

	for(int i=0; i <10; i++)
	{
		SpiMasterTransmit(0xFF);
	}
	
	while(SPDR0 != 0x01)
	{
		// Command 55	(expect a response of 0x01, SD is in idle state)
		SpiMasterTransmit(0x77);
		SpiMasterTransmit(0x00);
		SpiMasterTransmit(0x00);
		SpiMasterTransmit(0x00);
		SpiMasterTransmit(0x00);
		SpiMasterTransmit(0x95);
		
		// dummy bytes
		SpiMasterTransmit(0xFF);
		SpiMasterTransmit(0xFF);
	}
	
	while(SPDR0 != 0x00)
	{
		// deselect SS as output
		pDev_spi.setPinState('B', 4, high);
		
		// select SS as output
		pDev_spi.setPinState('B', 4, low);
		
		SpiMasterTransmit(0xFF);
		SpiMasterTransmit(0xFF);
		
		// Command 55	(expect a response of 0x01, SD is in idle state)
		SpiMasterTransmit(0x77);
		SpiMasterTransmit(0x00);
		SpiMasterTransmit(0x00);
		SpiMasterTransmit(0x00);
		SpiMasterTransmit(0x00);
		SpiMasterTransmit(0x95);
		
		// dummy bytes
		SpiMasterTransmit(0xFF);
		SpiMasterTransmit(0xFF);
		
		
		// deselect SS as output
		pDev_spi.setPinState('B', 4, high);
		
		// select SS as output
		pDev_spi.setPinState('B', 4, low);
		
		SpiMasterTransmit(0xFF);
		SpiMasterTransmit(0xFF);
		// ACMD41	(expect a response of 0x01, SD is in idle state)
		SpiMasterTransmit(0x69);
		SpiMasterTransmit(0x40);
		SpiMasterTransmit(0x00);
		SpiMasterTransmit(0x00);
		SpiMasterTransmit(0x00);
		SpiMasterTransmit(0x95);
		
		// dummy bytes
		SpiMasterTransmit(0xFF);
		SpiMasterTransmit(0xFF);
	}
	
	
	
	// CHNAGES BLOCK SIZE TO 512 BYTES
	while(SPDR0 != 0x00)
	{
		// deselect SS 
		pDev_spi.setPinState('B', 4, high);
			
		// select SS 
		pDev_spi.setPinState('B', 4, low);
	
	
		SpiMasterTransmit(0xFF);
		SpiMasterTransmit(0xFF);
	
	
		// Command 16	(expect a response of 0x00)
		SpiMasterTransmit(0x50);
		
		// 512
		SpiMasterTransmit(0x00);		//MSB
		SpiMasterTransmit(0x00);
		SpiMasterTransmit(0x02);
		SpiMasterTransmit(0x00);		//LSB
		
		// CRC
		SpiMasterTransmit(0x95);
		
		// dummy bytes
		SpiMasterTransmit(0xFF);
		SpiMasterTransmit(0xFF);
	
	}
	
	
	// deselect SS as output
	pDev_spi.setPinState('B', 4, high);
	
	// =========================================================================
	// SD INITIALIZATION COMPLETED
	

	
	
}





/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2014      */
/*-----------------------------------------------------------------------*/



#include "diskio.h"


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (void)
{
	DSTATUS stat;

	sdCardInitialize();

	stat = RES_OK;
	
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Partial Sector                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_readp (
BYTE* buff,		/* Pointer to the destination object */
DWORD sector,	/* Sector number (LBA) */
UINT offset,	/* Offset in the sector */
UINT count		/* Byte count (bit15:destination) */
)
{
	
	DRESULT res;
	
	
	// converts sector address to byte aligned
	sector = sector * 512;
	
	// holds the number of bytes to skip after relevant bytes have been read from the SD card sector
	UINT bytes_to_skip;

	// holds the sd_cmd_sector_addr
	BYTE sd_cmd_sector_addr[4];
	
	// converts the address into 4 8-bit array
	convert_sector_addr(sector, &sd_cmd_sector_addr[0]);
	
	// calculate number of trailing bytes + crc to skip following data read
	bytes_to_skip = 512 + 2 - offset - count;

	
	
	

	// READ BLOCK BEGIN
	// =========================================================================

	for(int i = 0; i < 100; i++)
	{
		SpiMasterTransmit(0xFF);
	}


	// select SS as output
	pDev_spi.setPinState('B', 4, low);






	// sends CMD 17 (Read a block, specifically block 0)
	
	SpiMasterTransmit(0xFF);
	SpiMasterTransmit(0xFF);
	
	// CMD17
	SpiMasterTransmit(0x51);

	// block address
	SpiMasterTransmit(sd_cmd_sector_addr[3]); // MSB
	SpiMasterTransmit(sd_cmd_sector_addr[2]);
	SpiMasterTransmit(sd_cmd_sector_addr[1]);
	SpiMasterTransmit(sd_cmd_sector_addr[0]); // LSB
	
	// CRC bytes
	SpiMasterTransmit(0xFF);
	SpiMasterTransmit(0xFF);

	// waits until valid response is received
	while(SPDR0 != 0x00)
	{
		SpiMasterTransmit(0xFF);
	}


	// looks for data token response 0xFE
	while(SPDR0 != 0xFE)
	{
		SpiMasterTransmit(0xFF);
	}


	// move to offset position in the sector
	for(unsigned i = 0; i < offset; i++)
	{
		SpiMasterTransmit(0xFF);
	}


	// read data from offset location
	for(unsigned i = 0; i < count; i++)
	{
		// communications is full duplex
		SpiMasterTransmit(0xFF);
		
		// reads each specified data byte into the internal memory buffer
		*buff++ = SPDR0;
	}


		
	// skips remaining bytes
	for(unsigned  i = 0; i < bytes_to_skip; i++)
	{
		SpiMasterTransmit(0xFF);
	}


	SpiMasterTransmit(0xFF);
	SpiMasterTransmit(0xFF);
	
	
	// =========================================================================
	// READ BLOCK END
	
	// select SS as output
	pDev_spi.setPinState('B', 4, high);
	
	res = RES_OK;
	
	

	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Partial Sector                                                  */
/*-----------------------------------------------------------------------*/

//DRESULT disk_writep (
//BYTE* buff,		/* Pointer to the data to be written, NULL:Initiate/Finalize write operation */
//DWORD sc		/* Sector number (LBA) or Number of bytes to send */
/*)
{
	// NOT REQUIRED FOR IMPLEMENTATION....
	
	
	
	DRESULT res;


	if (!buff) {
		if (sc) {

			// Initiate write process

			} else {

			// Finalize write process

		}
		} else {

		// Send data to the disk

	}

	return res;
}
*/