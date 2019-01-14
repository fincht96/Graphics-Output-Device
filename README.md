# Graphics-Output-Device

##**Project description:**

The creation of a microprocessor (such as ARM, AVR, PIC, etc.)  controlled device capable of reading image files from a connected digital storage medium (such as SD card, Micro-SD card, Compact flash card, etc.) commonly implemented in digital cameras. The device should also allow, through a graphical user interface, the user to select and output specific image files to a monitor via a display interface (such as VGA, HDMI, Composite Video, etc). 

**## Project Objectives:** 

- The device will be capable of reading image files from a digital storage medium commonly implemented in digital cameras. 
- The device will be able to output image files stored on the storage medium, through a standard display interface (such as VGA, HDMI, Composite Video, etc). 
- The device will incorporate graphical user interface software, allowing through user interaction, the navigation and selection of the image files on the storage medium. 
- The device will be portable and will not require an external power supply for operation.  


**## Project Deliverables:** 

The project can be broken down into the following key deliverables, where each key deliverable consists of sub-deliverables: 
- Digital storage medium interface 
- Display device interface 
- User interface   
- Data conversion interface 
- System management controller


Together the deliverables meet the overall system implementation. Further detail into these deliverables can be found below.

**### Digital storage medium interface:**

- *Hardware interface*, the appropriate hardware requirements for the communication protocol between the microprocessor and digital storage medium. E.g. operating voltage, physical connections between microprocessor and digital storage medium. 
 
- *Software interface*, the appropriate implementation of the software protocol for communication between the microprocessor and the digital storage medium. E.g. configuring microcontroller internally for communication interfacing, sending data between the microcontroller and digital storage medium in the correct data packet format. 

- *File system management software*, the implementation of software that allows the microprocessor to interface and access files on the storage medium device that is formatted to a specific file system by the digital camera. E.g. allowing the microcontroller to read a file from a FAT file system implemented on a digital storage medium. 



 
