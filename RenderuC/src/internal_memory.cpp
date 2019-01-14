

#include "internal_memory.h"
#include "frame_constants.h"


InternalMemory::InternalMemory()
{
	
	
	
}



void InternalMemory::writeMemoryLocation(uint16_t location, uint8_t data) 
{
		// writes to specific location in memory buffer
		m_buffer[location] = data;


}


uint8_t InternalMemory::readMemoryLocation(uint16_t location) const
{
	// reads from specific location in memory buffer
	return m_buffer[location];
}



void InternalMemory::clearMemoryBuffer() 
{
	for(int i = 0; i < BUFFER_SIZE; i++)
		m_buffer[i] = 0x00;
}