/*
 * user_input.cpp
 *
 * Created: 08/05/2018 01:22:18
 *  Author: Tom
 */ 


#include "user_input.h"
#include <avr/io.h>

UserInput::UserInput(DeviceInterface* const pDeviceinterface)
	: m_pDeviceInterface(pDeviceinterface)
{
	m_pDeviceInterface->setPortDirec('B', 0b00000010);		// set PB0 as input, 'comms input', set PB1 as an output, 'comms output'
	m_pDeviceInterface->setPortState('B', 0b00000011);		// PB0 and PB1 are by default set high, PB0 ,'comms input', is set 'high' with internal pull up
}

uint8_t UserInput::getPortStatus(char port)
{
	if(port == 'B')
	{
		return PINB;
	}

	else if(port == 'C')
	{
		return PINC;
	}
	
	return 0;
	
}