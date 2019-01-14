

#include "device.h"
#include <avr/io.h>


Device::Device()
{
	
	// initializes the various port directions
	this->setPortDirec('C', 0b01111111);	// set PC0-PC6 as outputs, connected to address A0-A6 on SRAM
	this->setPortDirec('A', 0b11111111);	// set PA0-PA7 as outputs, connected to address A7-A14 on SRAM
	this->setPortDirec('B', 0b00000010);	// set PB0 as input, 'comms input', set PB1 as an output, 'comms output'
	this->setPortDirec('D', 0b11111111);	// set PD0-PD7 as outputs, connected to data I/O pins on SRAM
	
	
	// initializes the various port states
	this->setPortState('C', 0b0000000);		// set PC0-PC6 to 0
	this->setPortState('A', 0b0000000);		// set PA0-PA7 to 0
	this->setPortState('B', 0b0000011);		// PB0 and PB1 are by default set high, PB0 ,'comms input', is set 'high' with internal pull up
											// PB1 is toggled low when communicating with uC2.									
	this->setPortState('D', 0b0000000);		// outputs '0' on all PORTD pins
	
}
	
	

void Device::setPortDirec(const char port, const uint8_t val) const
{
	switch(port)
	{
		case 'A':
			DDRA = val;
			break;
		
		case 'B':
			DDRB = val;
			break;
		
		case 'C':
			DDRC = val;
			break;
			
		case 'D':
			DDRD = val;
			break;
			
		default:
			break;
	}
}


void Device::setPinDirec(const char port, const uint8_t pin_number, const State pin_state) const
{
		// if setting pin high
		if(pin_state == high)
		{
			switch(port)
			{
				case 'A':
					DDRA |= (1<<pin_number);
					break;
				
				case 'B':
					DDRB |= (1<<pin_number);
					break;
				
				case 'C':
					DDRC |= (1<<pin_number);
					break;
				
				case 'D':
					DDRD |= (1<<pin_number);
					break;
				
				default:
					break;
			}
		}
		
		
		// else if setting a pin low
		else if (pin_state == low)
		{
			switch(port)
			{
				case 'A':
					DDRA &= !(1<<pin_number);
					break;
				
				case 'B':
					DDRB &= !(1<<pin_number);
					break;
				
				case 'C':
					DDRC &= !(1<<pin_number);
					break;
				
				case 'D':
					DDRD &= !(1<<pin_number);
					break;
				
				default:
					break;
			}
		}
			
	
	
	
}


void Device::setPortState(const char port, const uint8_t val) const
{
	switch(port)
	{
		case 'A':
			PORTA = val;
			break;
		
		case 'B':
			PORTB = val;
			break;
		
		case 'C':
			PORTC = val;
			break;
		
		case 'D':
			PORTD = val;
			break;
		
		default:
			break;
	}
}


void Device::setPinState(const char port, const uint8_t pin_number, const State pin_state) const
{
	// if setting pin high
	if(pin_state == high)
	{
			switch(port)
			{
				case 'A':
					PORTA |= (1<<pin_number);
					break;
						
				case 'B':
					PORTB |= (1<<pin_number);
					break;
					
				case 'C':
					PORTC |= (1<<pin_number);
					break;
					
				case 'D':
					PORTD |= (1<<pin_number);
					break;
					
				default:
					break;
			}
	}
	
	
	// else if setting a pin low
	else if (pin_state == low)
	{
			switch(port)
			{
				case 'A':
					PORTA &= !(1<<pin_number);
					break;
						
				case 'B':
					PORTB &= !(1<<pin_number);
					break;
						
				case 'C':
					PORTC &= !(1<<pin_number);
					break;
						
				case 'D':
					PORTD &= !(1<<pin_number);
					break;
						
				default:
					break;
			}
		
		
		
	}
	
	
	
	
	

	
	
	
}


uint8_t Device::getPortState(const char port) const
{
	switch(port)
	{
		case 'A':
			return PINA;
		
		case 'B':
			return PINB;
		
		case 'C':
			return PINC;
		
		case 'D':
			return PIND;
		
		default:
			return 1;
	}
}


State Device::getPinState(const char port, const uint8_t pin_number) const
{
	switch(pin_number)
	{
		case 'A':
			if((PINA & (1<<pin_number)) == 0)
				return low;
			else
				return high;
			
		case 'B':
			if((PINB & (1<<pin_number)) == 0)
				return low;
			else
				return high;
			
		case 'C':
			if((PINC & (1<<pin_number)) == 0)
				return low;
			else
				return high;
			
		case 'D':
			if((PIND & (1<<pin_number)) == 0)
				return low;
			else
				return high;		
	
		default:
			return low;
	}
	
}