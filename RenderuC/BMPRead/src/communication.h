
//********************************************************************************************************
//  Communication class
//
//  Used to communicate with uC1, specifically to indicate when the frame draw has been completed. It will wait until confirmation from uC1 the 
//  that the tristate buses have finished being setup before function has finished executing.


#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "device_interface.h"

class Communication 
{
	public:
		// constructor initializes the direction and state of the various ports and pins
		Communication(DeviceInterface* const pDeviceinterface);
		
		// function sends communication to uC1 indicating frame write to video memory has completed
		void transmitFrameComplete();
	
	private:
		// holds a pointer to a DeviceInterface object
		DeviceInterface* const m_pDeviceInterface;
		
	
};





#endif 