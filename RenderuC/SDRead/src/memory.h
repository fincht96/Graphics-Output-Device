
//********************************************************************************************************
//  Memory class
//
//  Base Memory class, used to allocate data to specifed memory locations, 
//  


#ifndef MEMORY_H
#define MEMORY_H



class Memory 
{
	public:
		// pure virtual function to be overwritten, will clear the data in the entire memory buffer region
		virtual void clearMemoryBuffer() = 0;							
};


#endif 