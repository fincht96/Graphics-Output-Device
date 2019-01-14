/*
 * registers.h
 *
 * use of enum provides control over which object can use this function, as only objects with the right permissions
 * have access to this header file
 *
 * Created: 23/03/2018 11:55:02
 *  Author: Tom
 */ 


#ifndef REGISTERS_H_
#define REGISTERS_H_


enum Reg
{
	// SPI Control Register 0
	SPCR0_REG = 0,
	
	// SPI Status Register 0
	SPSR0_REG,
	
	// SPI Data Register 0
	SPDR0_REG
	
};






#endif /* REGISTERS_H_ */