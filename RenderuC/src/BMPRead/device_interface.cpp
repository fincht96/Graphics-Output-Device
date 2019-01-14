


#include "device_interface.h"
#include <avr/io.h>

DeviceInterface::DeviceInterface()
{
		// does nothing.....
	
}





void DeviceInterface::setPortDirec(const char port, const uint8_t val) 
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


void DeviceInterface::setPinDirec(const char port, const uint8_t pin_number, const State pin_state) 
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


void DeviceInterface::setPortState(const char port, const uint8_t val) 
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


void DeviceInterface::setPinState(const char port, const uint8_t pin_number, const State pin_state) 
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


uint8_t DeviceInterface::getPortState(const char port) 
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


State DeviceInterface::getPinState(const char port, const uint8_t pin_number) 
{
	switch(port)
	{
		case 'A':
			if((PINA & (1<<pin_number)) == 1)
				return high;
				
			else if((PINA & (1<<pin_number)) == 0)
				return low;
		
			break;
		
		
		case 'B':
			if((PINB & (1<<pin_number)) == 1)
				return high;
				
			else if((PINB & (1<<pin_number)) == 0)
				return low;
				
			break;
		
		
		
		case 'C':
			if((PINC & (1<<pin_number)) == 1)
				return high;
				
			else if((PINC & (1<<pin_number)) == 0)
				return low;
				
			break;
		
		case 'D':
			if((PIND & (1<<pin_number)) == 1)
				return high;
			
			else if((PIND & (1<<pin_number)) == 0)
				return low;
			
			break;
		
		
		
		default:
			return low;
			break;
	}
	
	return low;
}