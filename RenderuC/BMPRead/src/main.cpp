/*
 * uC1_ATMEGA324A_CODE_CPP_rev_1.cpp
 *
 * Created: 10/03/2018 22:26:31
 * Author : Tom
 */ 

#ifndef F_CPU
#define F_CPU 20000000UL		// define clock frequency of device as 20 MHz
#endif

#include "frame_constants.h"

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <math.h>

#include "image_process.h"
#include "data_read.h"
#include "device_interface.h"
#include "communication.h"
#include "state.h"
#include "video_memory.h"
#include "internal_memory.h"
#include "render.h"
#include "image_data.h"







	

	






int main(void)
{
	// creates an object of type DeviceInterface
	DeviceInterface o_dev_interface;
	
	
	_delay_ms(400);


		
		
		

		
		
	// creates an object of type Communication
	Communication o_communication(&o_dev_interface);
	
	// creates an object of type VideoMemory
	VideoMemory o_video_memory(&o_dev_interface, &o_communication);
	
	// creates an object of type InternalMemory
	InternalMemory o_internal_memory;
		
	// creates an object of type DataRead
	DataRead o_data_read(&o_internal_memory);
	
	
				

	
	
	// creates an object of type ImageProcess, this will automatically read BMP data into VRAM through its constructor 
	ImageProcess o_image_process(&o_video_memory, &o_internal_memory, &o_data_read);
	
	o_video_memory.clearMemoryBuffer();
	o_communication.transmitFrameComplete();
	_delay_ms(20);



	
	for(int y = 0; y < FRAME_HEIGHT; y++)
	{
		for(int x = 0; x < FRAME_WIDTH; x++)
			o_video_memory.writeMemoryLocation(x,y,0x32);
	}
	
	o_communication.transmitFrameComplete();
	_delay_ms(20);
	
	
	for(int i= 0; i < 3; i++)
	{
		o_image_process.processImage();
		o_communication.transmitFrameComplete();
		_delay_ms(20);
	}
	
	
	
	
    while (1)				// infinite loop
    {

			
		
		
			
    }
	
	
	return 0;
}

