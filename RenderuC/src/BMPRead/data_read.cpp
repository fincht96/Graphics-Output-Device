
#include "image_data.h"
#include "data_read.h"
#include "memory_constants.h"


DataRead::DataRead(InternalMemory* const pInternalMemory)
	: m_pInternalMemory(pInternalMemory)
{
	
	
	
	
}



void DataRead::readInternalStorage(const uint16_t addr_offset) const
{
			// reads data from each pointer address, stores in buffer and increments pointer
			for(int i = 0; i < BUFFER_SIZE; i++)
			{
				m_pInternalMemory->writeMemoryLocation(i, pgm_read_byte(&(rawData[i + addr_offset])));
			}
}

void DataRead::readExternalStorage(const uint32_t addr_offset) const
{
	// To do, some jazz to read from SD card into the internal memory buffer..........
}