/*
 * user_input.h
 *
 * Created: 08/05/2018 01:19:14
 *  Author: Tom
 */ 


#ifndef USER_INPUT_H_
#define USER_INPUT_H_

#include "device_interface.h"




class UserInput
{
	public:
		UserInput(DeviceInterface* const pDeviceinterface);
		uint8_t getPortStatus(char port);
	
	private:
		// holds a pointer to a DeviceInterface object
		DeviceInterface* const m_pDeviceInterface;
	
};



#endif /* USER_INPUT_H_ */