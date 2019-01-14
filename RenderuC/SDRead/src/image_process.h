/*
 * image_process.h
 *
 * This class will read
 *
 * Created: 19/03/2018 12:42:25
 *  Author: Tom
 */ 


#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H

#include "stdint.h"
#include "video_memory.h"
#include "data_read.h"
#include "buffer_config.h"

class ImageProcess
{
	public:
		ImageProcess(VideoMemory* const pVideoMemory, DataRead* const pDataRead, uint8_t* pBuffer);
		
		
		// system function, carries out entire task of reading image data from internal/external storage into the VRAM
		void processImage();
		
		
	private:

		
		// returns the appropriate information from dataArray, based upon VRAM address supplied
		uint8_t getBufferData(uint8_t horizontal_addr_vram, uint8_t vertical_addr_vram);
		
		// updates the vertical and horizontal scaling member variables
		void calcScaling();
		
		// reads BMP file header data, stores relevant data in the member variables
		void readImageConfigData();
		

		
	
	
		// OBJECT POINTER DATA
		//**********************************************************************************************
	
		// holds pointer to VideoMemory object
		VideoMemory* const m_pVideoMemory;
		
		// holds pointer to DataRead object
		DataRead* const m_pDataRead;
		
		// holds pointer to InternalMemory object
		uint8_t* m_pBuffer;
		
		
		//**********************************************************************************************

		
		// BITMAP HEADER DATA
		//**********************************************************************************************
		
		// holds size of image raw data in bytes
		uint32_t m_raw_image_size;
		
		// offset, where image raw data begins
		uint32_t m_image_raw_data_offset;
		
		// holds width of bitmap file
		uint32_t m_image_width;
		
		// holds height of bitmap file
		uint32_t m_image_height;
		
		//**********************************************************************************************
		
		
		
		 
		// RESOLUTION SCALING DATA
		//**********************************************************************************************
		
		// number of horizontal pixels in VRAM that are represented by a single from bitmap image to represent a single horizontal pixel 
		// i.e. horizontal_scale = 2, means that 2 horizontal pixels in VRAM represents 1 horizontal pixel in BMP
		// i.e. horizontal_scale = 0.5, means that 1 horizontal pixel in VRAM represents 2 horizontal pixel in in BMP
		float m_horizontal_scale;
		

		// number of vertical pixels from VRAM image to represent a single vertical pixel in BMP
		// i.e. vertical_scale = 2, means that 2 vertical pixels in VRAM represents 1 pixel vertical in BMP
		// i.e. vertical_scale = 0.5, means that 1 vertical pixel in VRAM represents 2 pixels in vertical in BMP
		float m_vertical_scale;

		//**********************************************************************************************
		 
		
		// BUFFER DATA
		//**********************************************************************************************
		
		// holds current page_number
		uint8_t m_page_number;
		
		// holds the current address on page
		uint16_t m_page_address;
		
		//**********************************************************************************************
		
	
};







#endif /* IMAGE_PROCESS_H_ */