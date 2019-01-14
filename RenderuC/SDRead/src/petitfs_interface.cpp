/*
 * petitfs_interface.cpp
 *
 * Created: 22/03/2018 23:13:32
 *  Author: Tom
 */ 


#include "petitfs_interface.h"


PetitFsInterface::PetitFsInterface(FATFS* pFile_system)
	: m_pFile_system(pFile_system)
{
	
	
}


FRESULT PetitFsInterface::initSD() 
{		
	// mounts the file system
	return pf_mount(m_pFile_system);
}



FRESULT PetitFsInterface::openFile()
{
	// opens TEST.TXT file
	return pf_open("moon.bmp");
}


void PetitFsInterface::readFile(uint8_t* pBuffer, uint16_t buffer, uint32_t offset)
{
	// bytes read
	UINT br = 0;
	
	// moves file pointer by offset
	pf_lseek(offset);
	
	pf_read(pBuffer, BUFFER_SIZE, &br);
	
}




FRESULT PetitFsInterface::openDir()
{
	return FR_OK;
	
	
}