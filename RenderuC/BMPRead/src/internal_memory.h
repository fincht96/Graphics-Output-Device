/*
 * internal_memory.h
 *
 * Created: 11/03/2018 19:32:31
 *  Author: Tom
 */ 


#ifndef INTERNAL_MEMORY_H
#define INTERNAL_MEMORY_H

#include "stdint.h"
#include "memory_constants.h"
#include "memory.h"

class InternalMemory : public Memory
{
	public:
		// constructor takes pointer to a DeviceInterface as a parameter
		InternalMemory();
	
	
		// function will write 8 bit data to a specific location in static uint8_t array
		void writeMemoryLocation(uint16_t location, uint8_t data);		
		
		// function will write 'black' to all the memory locations in memory buffer
		virtual void clearMemoryBuffer();							
		
		// function will read data from specific location in static uint8_t array
		uint8_t readMemoryLocation(uint16_t) const;			
					
	
	private:
		// memory buffer, 512 bytes to hold data
		uint8_t m_buffer[BUFFER_SIZE];

	


	
	
	
};






#endif /* INTERNAL_MEMORY_H_ */