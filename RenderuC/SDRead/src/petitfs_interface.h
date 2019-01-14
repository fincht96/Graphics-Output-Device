/*
 * petitfs_interface.h
 *
 * Created: 22/03/2018 23:10:06
 *  Author: Tom
 */ 





#ifndef PETITFS_INTERFACE_H
#define PETITFS_INTERFACE_H



#include "buffer_config.h"

#include "device_interface.h"

#include "pff.h"
#include "diskio.h"


class PetitFsInterface
{
	public:
		// constructor takes address of FATFS variable  
		PetitFsInterface(FATFS* pFile_system);
	
		// function initialises SD card
		FRESULT initSD();
		
		// opens a file on SD card
		FRESULT openFile();
		
		// reads a file 
		void readFile(uint8_t* pBuffer, uint16_t buffer, uint32_t offset);
	
		// opens a directory 
		FRESULT openDir();
		
		// reads directory items
		void readDir();
		
		// moves the file pointer to specific location within file
		void movFilePointer(uint32_t _offset);
	

	
	private:

		// holds pointer to FATFS variable
		FATFS* m_pFile_system;
		
    
	
};







	
	
	


#endif /* PETITFS_INTERFACE_H_ */