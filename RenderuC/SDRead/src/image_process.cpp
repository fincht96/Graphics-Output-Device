/*
 * image_process.cpp
 *
 * Created: 19/03/2018 12:57:26
 *  Author: Tom
 */ 


#include <stdlib.h>
#include "frame_constants.h"
#include "image_process.h"


ImageProcess::ImageProcess(VideoMemory* const pVideoMemory, DataRead* const pDataRead, uint8_t* pBuffer)
	:	m_pVideoMemory(pVideoMemory), m_pDataRead(pDataRead), m_pBuffer(pBuffer)
{
	
	m_raw_image_size = 0;
	m_image_raw_data_offset = 0;
	m_image_width = 0;
	m_image_height = 0;
	m_horizontal_scale = 0.0;
	m_vertical_scale = 0.0;
	
	m_page_number = 0;
	m_page_address = 0;
	

}




void ImageProcess::processImage()
{

	// reads and stores various BMP header information
	readImageConfigData();
	
	// calculate scaling values for both vertical and horizontal values
	calcScaling();

	// writes appropriate data from dataArray into each video memory address location
	for(int vertical_addr_vram = 0; vertical_addr_vram < FRAME_HEIGHT; vertical_addr_vram++)
		for(int horizontal_addr_vram = 0; horizontal_addr_vram < FRAME_WIDTH; horizontal_addr_vram++)
		{
			// will write to each memory location in video memory based upon the VRAM address
			m_pVideoMemory->writeMemoryLocation(horizontal_addr_vram, vertical_addr_vram, getBufferData(horizontal_addr_vram,vertical_addr_vram));
		}

	
}


uint8_t ImageProcess::getBufferData(uint8_t horizontal_addr_vram, uint8_t vertical_addr_vram) 
{
	
	// TEMPORARY VARIABLES 
	//**************************************************************************
	
	// holds the x_scaled position in BMP file
	
				
	// holds the y_scaled position in BMP file

				
	// holds the specific location in the single dimension image data array

	
	// used to hold temporary page number

	
	// holds temporary page address on page

	
	//**************************************************************************
	
	
	
	
	
	// member function processes the VRAM location request and calculates the appropriate address to read from dataArray,
	// this is carried out by reading dataArray information into the internal memory buffer
	
		// calculates the scaled vertical address in BMP image, based upon vertical address in VRAM
		uint32_t temp_y_scaled = abs(((vertical_addr_vram - FRAME_HEIGHT_MAX) / m_vertical_scale));
		
		// calculates the scaled horizontal address in BMP image, based upon horizontal address in VRAM
		uint32_t temp_x_scaled = (horizontal_addr_vram/m_horizontal_scale);
		
		// converts the BMP 2 dimensional address into 1 dimensional address in the dataArray
		uint32_t temp_image_data_array_location = (((m_image_width*temp_y_scaled) + temp_y_scaled) + temp_x_scaled) + m_image_raw_data_offset;
	
		
		// calculates the page number required to read from 
		uint8_t temp_page_number = temp_image_data_array_location/BUFFER_SIZE;
	
	
		// only updates the page number if currently selected page number is not the same
		if(temp_page_number != m_page_number)
		{
			// page number pointed at is updated
			m_page_number = temp_page_number;
			
			// page number data pointed at is updated
			m_pDataRead->readExternalStorage(m_pBuffer, BUFFER_SIZE, (m_page_number*BUFFER_SIZE));
			
		}
		
		// calculates the address on page to read from
		uint16_t temp_page_address = (temp_image_data_array_location - (m_page_number * BUFFER_SIZE));
		
		// only updates the page number if currently selected page number is not the same
		if(temp_page_address != m_page_address)
		{
			// page address pointed at is updated
			m_page_address = temp_page_address;
		}
		
		
		
		
		// fetches data from address on specified page
		return m_pBuffer[m_page_address];
}





void ImageProcess::calcScaling()
{		
		// calculates horizontal scaling required for image
		m_horizontal_scale = FRAME_WIDTH/(float)m_image_width;
		
		// calculates the vertical scaling required for image
		m_vertical_scale = FRAME_HEIGHT/(float)m_image_height;
}


void ImageProcess::readImageConfigData()
{
	// BEAR IN MIND BITMAP FILE IS IN LITTLE ENDIAN FORMAT, AVR IS BIG ENDIAN, MUST BE CONVERTED!!! HENCE THE ORDER THE DATA IS READ IS IMPORTANT
	
	
	// function parses the bitmap data and gathers relevant information
	


	// BEAR IN MIND BITMAP FILE IS IN LITTLE ENDIAN FORMAT, AVR IS BIG ENDIAN, MUST BE CONVERTED!!! HENCE THE ORDER THE DATA IS READ IS IMPORTANT
	
	
	// function parses the bitmap data and gathers relevant information
	


	// fetches the first 512 bytes of bitmap file, various system config stuff
	m_pDataRead->readExternalStorage(m_pBuffer, BUFFER_SIZE, 0);
	

	
	// read offset 22h,23h,24h,25h in buffer into m_image_size
	m_raw_image_size |= ((uint32_t)m_pBuffer[0x25] << 24);
	m_raw_image_size |= ((uint32_t)m_pBuffer[0x24] << 16);
	m_raw_image_size |= ((uint32_t)m_pBuffer[0x23] << 8);
	m_raw_image_size |= ((uint32_t)m_pBuffer[0x22]);
	
	
	// read offset Ah,Bh,Ch,Dh in buffer into m_image_raw_data_offset
	m_image_raw_data_offset |= ((uint32_t)m_pBuffer[0x0D] << 24);
	m_image_raw_data_offset |= ((uint32_t)m_pBuffer[0x0C] << 16);
	m_image_raw_data_offset |= ((uint32_t)m_pBuffer[0x0B] << 8);
	m_image_raw_data_offset |= ((uint32_t)m_pBuffer[0x0A]);
	

	// read offset 12h, 13h, 14h, 15h into m_image_width
	m_image_width |= ((uint32_t)m_pBuffer[0x15] << 24);
	m_image_width |= ((uint32_t)m_pBuffer[0x14] << 16);
	m_image_width |= ((uint32_t)m_pBuffer[0x13] << 8);
	m_image_width |= ((uint32_t)m_pBuffer[0x12]);
	
	
	// read offset 16h, 17h, 18h, 19h into m_image_height
	m_image_height |= ((uint32_t)m_pBuffer[0x19] << 24);
	m_image_height |= ((uint32_t)m_pBuffer[0x18] << 16);
	m_image_height |= ((uint32_t)m_pBuffer[0x17] << 8);
	m_image_height |= ((uint32_t)m_pBuffer[0x16]);
	
	

	
}














