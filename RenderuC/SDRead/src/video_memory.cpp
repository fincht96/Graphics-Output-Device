


#include "video_memory.h"
#include "frame_constants.h"




VideoMemory::VideoMemory(DeviceInterface* const pDeviceinterface, Communication *const pCommunication)
	: m_pDeviceInterface(pDeviceinterface) , m_pCommunication(pCommunication)
{
	
	m_pDeviceInterface->setPortDirec('C', 0b01111111);		// set PC0-PC6 as outputs, connected to address A0-A6 on SRAM
	m_pDeviceInterface->setPortDirec('A', 0b11111111);		// set PA0-PA7 as outputs, connected to address A7-A14 on SRAM
	m_pDeviceInterface->setPortDirec('D', 0b11111111);		// set PD0-PD7 as outputs, connected to data I/O pins on SRAM
	
	m_pDeviceInterface->setPortState('C', 0b0000000);		// set PC0-PC6 to 0
	m_pDeviceInterface->setPortState('A', 0b0000000);		// set PA0-PA7 to 0
	m_pDeviceInterface->setPortState('D', 0b0000000);		// outputs '0' on all PORTD pins
	
	
}


void VideoMemory::writeMemoryLocation(const uint8_t h_addr, const uint8_t v_addr, const uint8_t data) const
{
	// *NOTE: VERTICAL ADDRESS IS UPDATED FIRST, FOLLOWED BY HORIZONTAL ADDRESS
	

	
	// writes address lower output, lower 7 bits of address represent vertical address
	m_pDeviceInterface->setPortState('C', v_addr);
	
	// writes address upper output, upper 8 bits of address represent horizontal address
	m_pDeviceInterface->setPortState('A', h_addr);
	
	// writes data to data port
	m_pDeviceInterface->setPortState('D', data);
	

}


void VideoMemory::writeAllMemoryLocations(uint8_t col)
{
	
	
		for (int y = 0; y < FRAME_HEIGHT; y++)
		{
			// outputs the vertical address of SRAM to PORTC output
			m_pDeviceInterface->setPortState('C', y);
			
			
			for (int x = 0; x < FRAME_WIDTH; x++)
			{

				
				// outputs the horizontal address of SRAM to PORTA output
				m_pDeviceInterface->setPortState('A', x);
				
				// outputs white colour on the data pins to SRAM
				m_pDeviceInterface->setPortState('D', col);
			}
			
		}

	
	
}




void VideoMemory::clearMemoryBuffer() 
{
	for (int y = 0; y < FRAME_HEIGHT; y++)
	{
					
		for (int x = 0; x < FRAME_WIDTH; x++)
		{
			// outputs the vertical address of SRAM to PORTC output
			m_pDeviceInterface->setPortState('C', y);
			
			// outputs the horizontal address of SRAM to PORTA output
			m_pDeviceInterface->setPortState('A', x);	
			
			// outputs white colour on the data pins to SRAM				
			m_pDeviceInterface->setPortState('D', 0xFF);					
		}
		
	}
	
}