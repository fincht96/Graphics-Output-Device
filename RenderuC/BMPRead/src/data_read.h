/*
 * image_read.h
 *
 *	A utility class, of which an instance cannot be made, reads data from internal or external storage into the internal memory buffer. 
 *
 * Created: 19/03/2018 09:34:53
 *  Author: Tom
 */ 


#ifndef DATA_READ_H
#define DATA_READ_H

#include "internal_memory.h"
#include "stdint.h"

class DataRead
{
	public:
	
		DataRead(InternalMemory* const pInternalMemory);
	
		// function reads 512 bytes of data from the internal storage into internal memory buffer
		void readInternalStorage(const uint16_t addr_offset) const;	
		
		// function reads 512 bytes of data from the external storage into internal memory buffer
		void readExternalStorage(const uint32_t addr_offset) const;
	
	private:
		// private constructor 
		DataRead();
		
		// holds a pointer to an InternalMemory object
		InternalMemory* const m_pInternalMemory;
	
};



#endif 