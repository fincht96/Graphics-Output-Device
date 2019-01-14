
#include "image_data.h"
#include "data_read.h"
#include "memory_constants.h"


DataRead::DataRead(PetitFsInterface* const pFs)
	: m_pFs(pFs)
{
	m_pFs->initSD();
	m_pFs->openFile();
	
	
}



void DataRead::readInternalStorage(const uint16_t addr_offset) const
{
	// reads data from each pointer address, stores in buffer and increments pointer
	for(int i = 0; i < BUFFER_SIZE; i++)
	{
		//m_pBuffer[i] = pgm_read_byte(&rawData[i + addr_offset]);
	}
}

void DataRead::readExternalStorage(uint8_t* pBuffer, uint32_t size, uint32_t addr_offset) const
{
	// To do, some jazz to read from SD card into the internal memory buffer..........
	m_pFs->readFile(pBuffer, size, addr_offset);
}