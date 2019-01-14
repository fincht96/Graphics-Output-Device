# Graphics-Output-Device

**Project description:**

The creation of a microprocessor (such as ARM, AVR, PIC, etc.)  controlled device capable of reading image files from a connected digital storage medium (such as SD card, Micro-SD card, Compact flash card, etc.) commonly implemented in digital cameras. The device should also allow, through a graphical user interface, the user to select and output specific image files to a monitor via a display interface (such as VGA, HDMI, Composite Video, etc). 

<a href="https://ibb.co/3MTGvPb"><img src="https://i.ibb.co/3MTGvPb/apollo11.jpg" alt="apollo11" style="width:420px;height:420px;border=>0"</a> 



**Project Objectives:** 

- The device will be capable of reading image files from a digital storage medium commonly implemented in digital cameras. 
- The device will be able to output image files stored on the storage medium, through a standard display interface (such as VGA, HDMI, Composite Video, etc). 
- The device will incorporate graphical user interface software, allowing through user interaction, the navigation and selection of the image files on the storage medium. 
- The device will be portable and will not require an external power supply for operation.  


**Project Deliverables:** 

The project can be broken down into the following key deliverables: 
- Digital storage medium interface 
- Display device interface 
- User interface   
- Data conversion interface 
- System management controller


Further detail into these deliverables can be found below:

*Digital storage medium interface:*

- Hardware interface, the appropriate hardware requirements for the communication protocol between the microprocessor and digital storage medium. E.g. operating voltage, physical connections between microprocessor and digital storage medium. 
 
- Software interface, the appropriate implementation of the software protocol for communication between the microprocessor and the digital storage medium. E.g. configuring microcontroller internally for communication interfacing, sending data between the microcontroller and digital storage medium in the correct data packet format. 

- File system management software, the implementation of software that allows the microprocessor to interface and access files on the storage medium device that is formatted to a specific file system by the digital camera. E.g. allowing the microcontroller to read a file from a FAT file system implemented on a digital storage medium. 

*Display device interface:*

 - Hardware interface, the physical hardware implementation for the display interface between the microprocessor and the display device. E.g. the physical adapter for connection between the display and microcontroller, for a VGA implementation a D-Sub 15 in connector will be required.  
 
- Software interface, the implementation of software with the correct circuit timing and signal output characteristics allowing for image data to be sent, in real time, from the microcontroller to the connected display device. E.g. the VGA display standard signal times and the data packet format for data transfer between the microcontroller and the display device. 

*User interface:* 

- Hardware interface, the physical hardware implementation to allow the user to interact with and access the specific functionality of the device. E.g. a switch to allow the user to reset the device, providing buttons to allow user to select between image files on the digital storage medium. 
 
- Software interface, the graphical user interface software implementation that enables user interaction via the user hardware interface. This interface enables the user to view in real time, via the display, the effect of their input interaction. E.g. the user will be able to view, via the display, selected files on the digital storage medium.  
 
*Data conversion interface:*

- Hardware interface, the external hardware required for the storing of the converted image data ready for output to the display device interface. E.g. external SRAM memory chips with sufficient storage to act as a buffer to hold the image data ready for output to the display device interface. 
 
- Software interface, the software required for reading and converting the image file data read from the digital storage medium to a format that can be displayed to a display via the display device interface. E.g. software to convert an image file stored on the digital storage medium to a format that can be output to the display device interface by the microcontroller. 

*System management controller:*

- Software, the implementation of a management controller system that enables and manages the interactions between the user interface, digital storage medium interface, data conversion interface and the display device interface. 

