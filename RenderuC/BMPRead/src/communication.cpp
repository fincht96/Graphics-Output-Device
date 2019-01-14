

// I THINK COMMS INTERFACE TOO SLOW, HENCE LAG, REMEMBER ONLY ON FOR 20 clk cylces on uC2 probabably frequnectly misses it!

// MISSSES THE ACKNOWLEDGE RESPONSE!


#include "communication.h"
#include "state.h"


Communication::Communication(DeviceInterface* const pDeviceinterface)
	: m_pDeviceInterface(pDeviceinterface)
{
		m_pDeviceInterface->setPortDirec('B', 0b00000010);		// set PB0 as input, 'comms input', set PB1 as an output, 'comms output'	
		m_pDeviceInterface->setPortState('B', 0b00000011);		// PB0 and PB1 are by default set high, PB0 ,'comms input', is set 'high' with internal pull up
}




void Communication::transmitFrameComplete()
{
		// make the transmit pin go low
		m_pDeviceInterface->setPinState('B',1,low);
	
		// while receive pin is high
		while((m_pDeviceInterface->getPinState('B',0)) != low)
		{
			// do nothing.....
		}
		
		// make the transmit pin go high
		m_pDeviceInterface->setPinState('B',1,high);	// sets the 'comms output' pin high*/
}


