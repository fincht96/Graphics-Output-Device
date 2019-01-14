/*
 * text_mode.h
 *
 * Created: 06/05/2018 15:46:35
 *  Author: Tom
 */ 


#ifndef TEXT_MODE_H_
#define TEXT_MODE_H_

#include "sprites_data.h"
#include "video_memory.h"


#define TEXTMODE_WIDTH_RES 34
#define TEXTMODE_HEIGHT_RES 15 

#include "buffer_config.h"


class TextMode
{
	public:
		TextMode(uint8_t* pBuffer, VideoMemory* const pVideoMemory);
		
		// clears data currently in the 512 byte internal memory buffer
		void clearBuffer();
		
		// x(between 0 and 33), y(between 0 and 14), letter(ASCII HEX value)
		bool writeChar(uint16_t x, uint16_t y, uint8_t letter);
		
		
		// returns the character at specific location in the internal memory
		uint8_t readChar(uint16_t x, uint16_t y);
		
		
		// reads all data from internal memory buffer and writes to the video memory
		void renderFrame();
		
	private:
	
		uint8_t* m_pBuffer;
		VideoMemory* const m_pVideoMemory;
	
	

};





#endif /* TEXT_MODE_H_ */