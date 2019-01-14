
#include <avr/pgmspace.h>
#include "render.h"



void Render::drawSquare(uint8_t h_addr, uint8_t v_addr, const uint8_t width, const uint8_t height, const uint8_t colour)
{
	// clears data in the video memory
	//clearMemoryBuffer();
	
	
	
	// loop through all the vertical addresses
	for(int y = 0; y < height; y++)
	{
		// loop through all the horizontal address
		for(int x = 0; x < height; x++)
		{
			// write each pixel to the memory location in video memory
			//writeMemoryLocation((h_addr+x),(v_addr+y),colour);
		}

	}
	

	
}
