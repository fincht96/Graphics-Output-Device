/*
 * VideoMemory.h
 *
 * Created: 11/03/2018 17:19:53
 *  Author: Tom
 */ 


#ifndef VIDEO_MEMORY_H
#define VIDEO_MEMORY_H

#include "device_interface.h"
#include "memory.h"
#include "stdint.h"
#include "communication.h"

class VideoMemory : public Memory
{
	public:
		// constructor initializes the direction and state of the various ports and pins
		VideoMemory(DeviceInterface* const pDeviceInterface, Communication* const pCommunication);															
		
		// function will write 8 bit data to a specific location in external SRAM	
		void writeMemoryLocation(const uint8_t h_addr, const uint8_t v_addr, const uint8_t data) const;		
		
		// writes to all memory locations a set colour
		void writeAllMemoryLocations(uint8_t col);
		
		// function will write 'black' to all the memory locations in external SRAM	
		virtual void clearMemoryBuffer();									
	
	private:
		// holds a pointer to a DeviceInterface object
		DeviceInterface* const m_pDeviceInterface;
		
		Communication* const m_pCommunication;
	
};



#endif /* VIDEOMEMORY_H_ */