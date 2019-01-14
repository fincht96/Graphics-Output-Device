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
#include "image_data.h"
#include "petitfs_interface.h"
#include "text_mode.h"
#include "user_input.h"



	






int main(void)
{
	
	uint8_t buffer[256];
	uint8_t text_buffer[BUFFER_SIZE];		// used exclusively for text mode
	
	DeviceInterface dev;

	FATFS file_system;
		
		
	uint8_t port_state_B = 0;
	uint8_t port_state_C = 0;
	UserInput o_user_input(&dev);
	Communication o_communication(&dev);
	VideoMemory o_video_memory(&dev, &o_communication);
	TextMode o_text(&text_buffer[0], &o_video_memory);
	PetitFsInterface o_petit_fs(&file_system);
	DataRead o_data_read(&o_petit_fs);
	
	
	
	_delay_ms(100);
	
	


	FRESULT sd_init = o_petit_fs.initSD();
	FRESULT open_file = o_petit_fs.openFile();


	o_video_memory.clearMemoryBuffer();
	_delay_ms(10);
	o_communication.transmitFrameComplete();
	
	_delay_ms(1000);
	
	uint8_t arrow_loc = 1;		// holds V location of arrow

	for(int i =0; i < 3; i++)
	{
		// renders new text frame
		o_text.renderFrame();
		
		// tells uC2 to update frame
		o_communication.transmitFrameComplete();
	}
	
	

	while(1)
	{	
		
		
		
		
		port_state_B = o_user_input.getPortStatus('B');
		port_state_C = o_user_input.getPortStatus('C');
		
		// if up button is pressed 
		if((port_state_B & 0b00001000) == 0b00001000)
		{
			
			// decrements vertical position
			--arrow_loc;
			
			// bounds check
			if(arrow_loc <= 0)
			{
				arrow_loc = 8;
			}
			
			
			
			for(int i =0; i < 3; i++)
			{
				// renders new text frame
				o_text.renderFrame();
							
				// tells uC2 to update frame
				o_communication.transmitFrameComplete();
			}
		}
		
		
		// if down button is pressed 
		if((port_state_B & 0b00000100) == 0b00000100)
		{

			// decrements vertical position
			++arrow_loc;
			
			// bounds check
			if(arrow_loc > 8)
			{
				arrow_loc = 1;
			}
			
			
			
			for(int i =0; i < 3; i++)
			{
				// renders new text frame
				o_text.renderFrame();
				
				// tells uC2 to update frame
				o_communication.transmitFrameComplete();
			}
		}
		
		
		
		// if select button is pressed 
		if((port_state_C & 0b10000000) == 0b10000000)
		{
			uint8_t x = 14;
			uint8_t y = 0;
			uint8_t symbol = 0;
			uint8_t col = 0;
			
			
			// looks for arrow
			for(y = 1; y < 9; y++)
			{
				symbol = o_text.readChar(x,y);
				
				if(symbol == 128)	
					break;
			}
			
			// once found, reads file number
			uint8_t file_num = o_text.readChar(19,y);
			
			switch(file_num)
			{
					// 1
					case 49:
						if((sd_init == FR_OK) && (open_file == FR_OK))
						{
							col = 0b00000011;
						}
						else 
						{
							col = 0b00000000;
						}
						o_video_memory.writeAllMemoryLocations(col);
						o_communication.transmitFrameComplete();
						break;
				
					// 2
					case 50:
						o_video_memory.writeAllMemoryLocations(buffer[0]);
						o_communication.transmitFrameComplete();
						break;
				
				
					// 3
					case 51:
						col = 0b00001111;
						o_video_memory.writeAllMemoryLocations(col);
						o_communication.transmitFrameComplete();
						break;
				
				
					// 4
					case 52:
						col = 0b00111111;
						o_video_memory.writeAllMemoryLocations(col);
						o_communication.transmitFrameComplete();
						break;
				
					// 5
					case 53:
						col = 0b00110011;
						o_video_memory.writeAllMemoryLocations(col);
						o_communication.transmitFrameComplete();
						break;
				
					// 6
					case 54:
						col = 0b00100001;
						o_video_memory.writeAllMemoryLocations(col);
						o_communication.transmitFrameComplete();
						break;
				
					// 7
					case 55:
						col = 0b00101001;
						o_video_memory.writeAllMemoryLocations(col);
						o_communication.transmitFrameComplete();
						break;
				
					// 8
					case 56:
						col = 0b00101011;
						o_video_memory.writeAllMemoryLocations(col);
						o_communication.transmitFrameComplete();
						
						break;
				
					// 9
					case 57:
						col = 0b00111011;
						o_video_memory.writeAllMemoryLocations(col);
						o_communication.transmitFrameComplete();
						break;
				
					default:
						break;
				
			}
			
			

		}
		
		
	// clears internal memory buffer
	o_text.clearBuffer();
	

	for(int y = 1; y < 9; y++)
	{
		// character 'F'
		o_text.writeChar(15,y,70);

		// character 'I'
		o_text.writeChar(16,y,73);

		// character 'L'
		o_text.writeChar(17,y,76);

		// character 'E'
		o_text.writeChar(18,y,69);

		// character ' '
		o_text.writeChar(19,y,32);

		// character 'FILE_NUMBER'
		o_text.writeChar(19,y,48+y);
	}




	// writes -> starting at (14, 1)
	// ------------------------------------
	
	// character '->'
	o_text.writeChar(14,arrow_loc,128);
		
		
		
		
		
		
		
	}
		
		
		
	
	
	
	return 0;
}



