
//********************************************************************************************************
//  Device Interface class
//
//  This device interface class enables control over the state of the various ports and pins on the micro controller
//	any class that requires interaction with the physical hardware needs access to this class.











#ifndef DEVICE_INTERFACE_H
#define DEVICE_INTERFACE_H

#include "stdint.h"
#include "state.h"

class DeviceInterface
{
	public:

			DeviceInterface();
			
			
			// allows direction of the port to be modified
			void setPortDirec(const char, const uint8_t);
				
			// allows the direction of the pins on the port to be modified
			void setPinDirec(const char, const uint8_t, const State);
				
				
			// allows the state of the port to be modified
			void setPortState(const char, const uint8_t);
				
			// allows the state of the pins on the port to be modified
			void setPinState(const char, const uint8_t, const State) ;
				
				
			// allows the state of a port to be read
			uint8_t getPortState(const char);
				
			// allows the state of a pin on a port to be read
			State getPinState(const char, const uint8_t);
		
};






#endif 