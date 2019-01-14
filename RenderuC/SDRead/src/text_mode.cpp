/*
 * text_mode.cpp
 *
 * Created: 06/05/2018 15:46:45
 *  Author: Tom
 */ 


#include "text_mode.h"


TextMode::TextMode(uint8_t* pBuffer, VideoMemory* const pVideoMemory)
	: m_pBuffer(pBuffer), m_pVideoMemory(pVideoMemory)
{
		
}

// clears all data in internal memory buffer, must be called before writing chars to the internal memory buffer
void TextMode::clearBuffer()
{
	for(int i = 0; i < BUFFER_SIZE; i++)
	{
		m_pBuffer[i] = 0;
	}
}


bool TextMode::writeChar(uint16_t x, uint16_t y, uint8_t letter)
{
	// if location is out of bounds
	if
	(	
		(x < 0) || 
		(x >= TEXTMODE_WIDTH_RES) || 
		(y < 0) ||
		(y >= TEXTMODE_HEIGHT_RES) 
	)
	{
		return false;
	}
	
	
	// converts the 2D address into 1D
	uint16_t loc = (y*TEXTMODE_WIDTH_RES) + x;
	
	// writes the character into the internal memory buffer location
	m_pBuffer[loc] = (uint8_t)letter;
	
	

	return true;
}

uint8_t TextMode::readChar(uint16_t x, uint16_t y)
{
	uint8_t data = 0;	// holds data read from internal memory buffer
	uint16_t loc = (y*TEXTMODE_WIDTH_RES) + x;	// calculates location in 512 array
	
	data = (uint8_t)m_pBuffer[loc];	// fetches data from array
	
	return data;
}

void TextMode::renderFrame()
{
	
	uint8_t val;	// holds value read from the internal memory buffer
	const unsigned char* pCharSprite;		// holds pointer to sprite character
	uint16_t x_vmem_loc = 0;			// holds the x location in video memory
	uint16_t y_vmem_loc = 0;			// holds the y location in video memory
	uint8_t sprite_data_addr = 0;		// holds address of data within sprite
	
	
	
	// clears the entire memory buffer, makes it go black
	m_pVideoMemory->clearMemoryBuffer();
	
	
	
	// Iterates through all addresses in TextMode internal memory buffer
	for(int y_text = 0; y_text < TEXTMODE_HEIGHT_RES;  y_text++)
	{
		for(int x_text = 0; x_text < TEXTMODE_WIDTH_RES; x_text++)
		{
			// reads character from internal memory buffer as ASCII value
			val = readChar(x_text,y_text);
			
			// finds sprite pointer based upon ASCII value
			switch(val)
			{
				// blank space
				case 32:
				pCharSprite = &sprite_blnk[0];
				break;
				
				// *
				case 42:
				pCharSprite = &sprite_star[0];
				break;
				
				// fill
				case 219:
				pCharSprite = &sprite_fill[0];
				break;
				

				// ->
				case 128:
				pCharSprite = &sprite_arrow_right[0];
				break;
				
				// diamond
				case 129:
				pCharSprite = &sprite_diamond[0];
				break;
				
				
				
				// F
				case 70:
				pCharSprite = &sprite_F[0];
				break;
				
				// I
				case 73:
				pCharSprite = &sprite_I[0];
				break;
				
				// L
				case 76:
				pCharSprite = &sprite_L[0];
				break;
				
				// E
				case 69:
				pCharSprite = &sprite_E[0];
				break;
				
				// 0
				case 48:
				pCharSprite = &sprite_0[0];
				break;
				
				// 1
				case 49:
				pCharSprite = &sprite_1[0];
				break;
				
				// 2
				case 50:
				pCharSprite = &sprite_2[0];
				break;
				
				
				// 3
				case 51:
				pCharSprite = &sprite_3[0];
				break;
				
				
				// 4
				case 52:
				pCharSprite = &sprite_4[0];
				break;
				
				// 5
				case 53:
				pCharSprite = &sprite_5[0];
				break;
				
				// 6
				case 54:
				pCharSprite = &sprite_6[0];
				break;
				
				// 7
				case 55:
				pCharSprite = &sprite_7[0];
				break;
				
				// 8
				case 56:
				pCharSprite = &sprite_8[0];
				break;
				
				// 9
				case 57:
				pCharSprite = &sprite_9[0];
				break;
				
				default:
				pCharSprite = &sprite_blnk[0];
				break;
			}
			
		
		
		
			// calculates the initial starting location to begin writing to in video memory
			x_vmem_loc = (x_text * 7) + 8 ;
			y_vmem_loc = (y_text * 8);
		

		
			// increments through all locations within the sprite data
			for(int y_sprite = 0; y_sprite < 8; y_sprite++)
			{
				for(int x_sprite = 0; x_sprite < 7; x_sprite++)
				{
					// converts 2D sprite address into 1D
					sprite_data_addr  = (y_sprite*7) + x_sprite;
				
					// writes each data location within sprite to the video memory 
					m_pVideoMemory->writeMemoryLocation(x_vmem_loc,y_vmem_loc, pCharSprite[sprite_data_addr]);
				
					// increments x location in video memory, will just keep incrementing
					++x_vmem_loc;
					
				}
				
				// resets  the x_vmem_loc
				x_vmem_loc = (x_text * 7) + 8;
				
				// increments y location in video memory
				++y_vmem_loc;
			}	
		
			
			}	
		
		
		}
	
	
	
	
}