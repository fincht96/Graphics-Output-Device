;
; uC2_ATMEGA324A_CODE_rev_0.asm
;
; Created: 03/03/2018 23:48:14
; Author : Tom
;





; Description of operation:
; -------------------------
;
;	GENERAL
;	This program, to execute on an ATmega324A reads from either SRAM 1 or 2 and outputs the data in the memory chips to a display. Specifically this involves reading data from each address
;   and outputting the pixel data to the screen via the VGA protocol in the relevant region. i.e. RGB out.
;	
;	In conjunction to this main functionality, the device will also set the enable state of the relevant tristate bus devices. Allowing this microcontroller to read from one SRAM while the 
;	other microcontroller writes to the other SRAM. 
;	
;
;	SPECIFIC
;	During the vertical back porch of the VGA protocol the microcontroller will check the 'comms input pin', PORT B4, connected to the 'comms output pin', PORT B0, on microcontroller 1
;	if the comms input pin is 'low' then microcontroller 2 will swap which SRAM module to read from. Once the swap has completed, microcontroller 2 will drive it's 'comms output pin', PORT B3,
;   'low' for 20 clock cycles before being driven low again. This will indicate to microcontroller 1, it can continue writing to memory. Depending on which SRAM was being previously being written to, 
;	microcontroller 2 will swap to the other SRAM module. The operations to read from each SRAM module can be found below:	
;	
;
;
;	To read the data stored in SRAM 1 the following operation will occur:
;
;	1) PORT B0 will be set to 'LOW' output, this will enable TRI 2,3,10,11,12, SRAM 1 (OE), and SRAM 2 (WE)
;	2) PORT B2, connected to TRI 9, is driven 'HIGH', high impedance output, this disconnects the data lines of SRAM 2 from the DAC input
;	3) PORT B1, connected to TRI 4, is driven 'HIGH', high impedance output, until VGA RGB output region where the data lines of SRAM 1 are connected to the DAC input
;
;
;	To read the data stored in SRAM 2 the following operation will occur:
;
;	1) PORT B0 will be set to 'HIGH' output, this will enable TRI 1,5,6,7,8, SRAM 1 (WE), and SRAM 2 (OE)
;	2) PORT B1, connected to TRI 4, is driven 'HIGH', high impedance output, this disconnects the data lines of SRAM 1 from the DAC input
;	3) PORT B2, connected to TRI 9, is driven 'HIGH', high impedance output, until VGA RGB output region where the data lines of SRAM 2 are connected to the DAC input 




; Port/Pin configuraton:
; ----------------------
;
;	PORT:		PIN:		I/O:		DESCRIPTION:
;	=====		====		====		============
;	C			0			O			Addr 0 of SRAM 1/2 
;	C			1			O			Addr 1 of SRAM 1/2
;	C			2			O			Addr 2 of SRAM 1/2
;	C			3			O			Addr 3 of SRAM 1/2
;	C			4			O			Addr 4 of SRAM 1/2
;	C			5			O			Addr 5 of SRAM 1/2
;	C			6			O			Addr 6 of SRAM 1/2
;	
;	A			0			O			Addr 7 of SRAM 1/2
;	A			1			O			Addr 8 of SRAM 1/2
;	A			2			O			Addr 9 of SRAM 1/2
;	A			3			O			Addr 10 of SRAM 1/2
;	A			4			O			Addr 11 of SRAM 1/2
;	A			5			O			Addr 12 of SRAM 1/2
;	A			6			O			Addr 13 of SRAM 1/2
;	A			7			O			Addr 14 of SRAM 1/2
;
;	B			0			O			'NOT' gate input
;	B			1			O			Tri 4 enable pin
;	B			2			O			Tri 9 enable pin
;	B			3			O			transmit (YELLOW CABLE ON UC2)
;	B			4			I			receive (GREEN CABLE ON UC2)

;	D			0			O			H-Sync pin
;	D			1			O			V-Sync pin




; Port/Pin State initial state configuration:
; -------------------------------------------
;
;	PORT:		PIN:		State:		DESCRIPTION:
;	=====		====		======		============
;	C			0			0			Addr 0 of SRAM 1/2 
;	C			1			0			Addr 1 of SRAM 1/2
;	C			2			0			Addr 2 of SRAM 1/2
;	C			3			0			Addr 3 of SRAM 1/2
;	C			4			0			Addr 4 of SRAM 1/2
;	C			5			0			Addr 5 of SRAM 1/2
;	C			6			0			Addr 6 of SRAM 1/2
;	
;	A			0			0			Addr 7 of SRAM 1/2
;	A			1			0			Addr 8 of SRAM 1/2
;	A			2			0			Addr 9 of SRAM 1/2
;	A			3			0			Addr 10 of SRAM 1/2
;	A			4			0			Addr 11 of SRAM 1/2
;	A			5			0			Addr 12 of SRAM 1/2
;	A			6			0			Addr 13 of SRAM 1/2
;	A			7			0			Addr 14 of SRAM 1/2
;
;	B			0			0			'NOT' gate input, default state SRAM 1 is being read by uC2 and SRAM 2 is being written to by uC1
;	B			1			1			Tri 4 (connects SRAM 1 data pins to DAC) by default tristate is disabled, will be re-enabled when outputting SRAM 1 to VGA
;	B			2			1			Tri 9 (connects SRAM 2 data pins to DAC) will remain disabled while reading from SRAM 1
;	B			3			1			transmit output pin, by default 'high', when communicating with uC1 then pin is pulled 'low'
;	B			4			1			receive input pin set with a "pull up" resistor, by default will read '1' 

;	D			0			1			by default H-Sync is high
;	D			1			1			by default V-Sync is high 











; COMPILER SETTINGS
.INCLUDE "M324ADEF.INC"


; INTERRUPT VECTORS
.org 0		; defines absolute address for interrupt vector




; ASSIGN LABELS

.def H_COUNT = r22			; label holds the horizontal pixel count
.def V_COUNT = r23			; label holds vertical line count

.def SRAM_SELECTED = r24	; label points to SRAM 1 or 2, (0b00000010 is SRAM 2)
.def DAC_OUT_TRI = r25		; label that points to either TRI 4 (PB1) or TRI 9 (PB2), by default is pointing at TRI 4 












; ****************************************************************************************
; **** STARTUP SEQUENCE
; ****************************************************************************************

; INITIALIZE STACK POINTER

	ldi r16,low(ramend)		; loads the lower byte of top stack address into register 16 
	out spl,r16				; stack pointer lower byte is set to lower byte of the top 
							; stack address stored in register 16
							
	ldi r16,high(ramend)	; loads the upper byte of top stack address into register 16
	out sph,r16				; stack pointer upper byte is set to upper byte of the top 
							; stack address stored in register 16






INIT_PORT_I_O:


;=======================================================
; **** PORT C I/O SETUP 
;=======================================================

	; ddrc pins configured as an 'output'

	sbi ddrc,0		;2 	
	sbi ddrc,1 		;2 
	sbi ddrc,2		;2 
	sbi ddrc,3		;2 
	sbi ddrc,4		;2 
	sbi ddrc,5		;2 
	sbi ddrc,6		;2 


;=======================================================
; **** PORT A I/O SETUP 
;=======================================================

	; ddra pins configured as an 'output'

	sbi ddra,0		;2  		
	sbi ddra,1 		;2 
	sbi ddra,2		;2 	
	sbi ddra,3		;2 
	sbi ddra,4		;2 
	sbi ddra,5		;2 
	sbi ddra,6		;2 
	sbi ddra,7		;2 




;=======================================================
; **** PORT B I/O SETUP 
;=======================================================

	; ddrb pins configured as 'output' and input'

	sbi ddrb,0		;2 
	sbi ddrb,1		;2 
	sbi ddrb,2		;2 
	sbi ddrb,3		;2 
	cbi ddrb,4		;2 





;=======================================================
; **** PORT D I/O SETUP 
;=======================================================

	; ddrb pins configured as 'output' and input'

	sbi ddrd,0		;2 
	sbi ddrd,1		;2 



INIT_PORT_STATE:

;=======================================================================
; PORT C 
;=======================================================================

	ldi r16, 0b00000000		;1
	out portc, r16			;1


;=======================================================================
; PORT A 
;=======================================================================

	ldi r16, 0b00000000		;1
	out porta, r16			;1




;=======================================================================
; PORT B 
;=======================================================================

	ldi r16, 0b01111110		;1 
	out portb, r16			;1 




;=======================================================================
; PORT D
;=======================================================================

	ldi r16, 0b00000011		;1 
	out portb, r16			;1 





; *********************************************************************************************************************************************************************************************************
; INITIALIZE DEFAULT READING FROM SRAM 1
; *********************************************************************************************************************************************************************************************************

	; first set 'SRAM_SELECTED' to '0b00000001'
	ldi SRAM_SELECTED, 0b00000001	;1

	; allocate an indirect reference to 'TRI 4' to allow it to be toggled, this is so data output on SRAM 1 to DAC can be connected and disconnected during the appropriate region in the VGA protocol
	ldi DAC_OUT_TRI, 0b00000010	;1 loads PB1 bit into DAC_OUT_TRI


	; configure tristates to read from SRAM 1
	; ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

	;	1) PORT B0 will be set to 'LOW' output, this will enable TRI 2,3,10,11,12, SRAM 1 (OE), and SRAM 2 (WE)
	cbi portb,0				;2

	;	2) PORT B2, connected to 'TRI 9', is driven 'HIGH', high impedance output, this disconnects the data lines of SRAM 2 from the DAC input
	sbi portb,2				;2

	;	3) PORT B1, connected to 'TRI 4', is driven 'HIGH', high impedance output, until VGA RGB output region where the data lines of SRAM 1 are connected to the DAC input
	in r16, portb			;1 
	or r16, DAC_OUT_TRI		;1 
	out portb, r16			;1


	; communicates to uC1 indicating it can begin writing the next frame
	; ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

	cbi portb, 3			;2 pulls the transmit line 'low', indicating to uC1 that the frame has been swapped and that it can begin writing the new frame

	; Delay 18 cycles
    ldi  r18, 6
LT: dec  r18
    brne LT
    

	sbi portb, 3			;2 pulls the transmit 'high'







; main program loop
main:



V_LOOP:

; ****************************************************************************************
; **** VERTICAL LOOP - BEGIN
; ****************************************************************************************

	; **NOTE: 7 clks have already been used up, 5 by leaving the V_front porch region and 2 for jumping
	; to beginning of V_loop.





; **** V-SYNC DRIVE LOW (2 lines, 1,270 cycles)


	ldi r20, 0x00	; 1 loads 0b00000000 into r20
	out portd,r20 	; 1 drives v-sync and h-sync active low 

	
; ========================================================================================
	; Delay 1268 cycles
	; -----------------
  
	; delay 1266
    ldi  r18, 2
    ldi  r19, 164
L1: dec  r19
    brne L1
    dec  r18
    brne L1
    nop
 
  
	; delay 2
  	ldi r16,low(33)			;1 holds LSB of loop value
	ldi r17,high(33)		;1 hold MSB of loop value
; ========================================================================================
	
	ldi r20, 0b00000011	; 1 loads 0b11000000 into r20
	out portd,r20 		; 1 drives v-sync and h-sync high 
	

	
	
	
	
	
	
	
	
	

; **** VERTICAL BACK PORCH (33 lines, 20,955 cycles)

	
V_BACK_PORCH:





  ; **** Delay (521 cycles)

  
; ========================================================================================	
	; DELAY 519
	
	
	
	
	; Delay 473 cycles
	; ----------------
    ldi  r18, 157
L_V_BACK_PORCH_1: dec  r18
    brne L_V_BACK_PORCH_1
    rjmp PC+1









//==========================================================================================================================================================================================================
// COUNTER RESET, COMMS CHECK AND SRAM SWAP
//==========================================================================================================================================================================================================
// TOTAL CLK CYCLES: 46








//**********************************************************************************************************************************************************************************************************
; total clk cycles: 2

RESET_VERT_HORIZ_COUNTER:

; Description:
; -------------
;
; This subroutine resets the horizontal and vertical counter.
;

	ldi H_COUNT, 0b00000000		;1
	ldi V_COUNT, 0b00000000		;1

//**********************************************************************************************************************************************************************************************************





//**********************************************************************************************************************************************************************************************************
; total clk cycles: 5 (skip) / 7 (branch to READ_SRAM_2_INIT)/ 9 (branch to READ_SRAM_1_INIT)

; NOTE:
;		- in order to mitigate the difference in time calling the SRAM swap functions READ_SRAM_2_INIT will have an extra two clock cycles added
;		- in order to mitigate the time difference when not doing anything 


CHECK_COMMS_STATE:

; Description:
; -------------
;
; This subroutine will check the pin state of PB4 (comms input), if the pin is '0' then the SRAM module being read from will be swapped else leave this sub routine.
;



	; if comms input is '1' then skip i.e. microcontroller 1 is not indicating it has finished drawing frame yet
	; ----------------------------------------------------------------------------------------------------------

	in r20, PINB			;1 reads the input status of pins connected to port b
	andi r20, 0b00010000	;1 isolates state of bit PB4 on portb input register

	cpi r20, 0b00010000		;1 compares if r16 is equal to immediate value

	breq delay_and_skip				;2/1 2 if true, 1 if false, skips SRAM swap if comms line is 'high'


	; if comms input is '0' and 'SRAM_SELECTED' is '0b00000001' then swap to SRAM 2
	; -----------------------------------------------------------------------------
	cpi SRAM_SELECTED, 0b00000001	;1
	breq READ_SRAM_2_INIT			;2/1


	; if comms input is '0' and 'SRAM_SELECTED' is '0b00000010' then swap to SRAM 1 
	; -----------------------------------------------------------------------------
	cpi SRAM_SELECTED, 0b00000010	;1
	breq READ_SRAM_1_INIT			;2/1



//**********************************************************************************************************************************************************************************************************



//**********************************************************************************************************************************************************************************************************
; total clk cycles: 35

READ_SRAM_1_INIT:

	; first set 'SRAM_SELECTED' to '0b00000001'
	ldi SRAM_SELECTED, 0b00000001	;1

	; allocate an indirect reference to 'TRI 4' to allow it to be toggled, this is so data output on SRAM 1 to DAC can be connected and disconnected during the appropriate region in the VGA protocol
	ldi DAC_OUT_TRI, 0b00000010	;1 loads PB1 bit into DAC_OUT_TRI


	; configure tristates to read from SRAM 1
	; ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

	;	1) PORT B0 will be set to 'LOW' output, this will enable TRI 2,3,10,11,12, SRAM 1 (OE), and SRAM 2 (WE)
	cbi portb,0				;2

	;	2) PORT B2, connected to 'TRI 9', is driven 'HIGH', high impedance output, this disconnects the data lines of SRAM 2 from the DAC input
	sbi portb,2				;2

	;	3) PORT B1, connected to 'TRI 4', is driven 'HIGH', high impedance output, until VGA RGB output region where the data lines of SRAM 1 are connected to the DAC input
	in r20, portb			;1 
	or r20, DAC_OUT_TRI		;1 
	out portb, r20			;1


	; communicates to uC1 indicating it can continue writing the next frame
	; ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

	cbi portb, 3			;2 pulls the transmit line 'low', indicating to uC1 that the frame has been swapped and that it can begin writing the new frame

	; Delay 18 cycles
    ldi  r18, 6
LT_DELAY_1: dec  r18
    brne LT_DELAY_1
    

	sbi portb, 3			;2 pulls the transmit 'high'



	jmp H_SYNC_LOW			;4

//**********************************************************************************************************************************************************************************************************







//**********************************************************************************************************************************************************************************************************
; total clk cycles: 37


READ_SRAM_2_INIT:
	nop		;1
	nop		;1

	; first set 'SRAM_SELECTED' to '0b00000010'
	ldi SRAM_SELECTED, 0b00000010	;1

	; allocate an indirect reference to 'TRI 9' to allow it to be toggled, this is so data output on SRAM 2 to DAC can be connected and disconnected during the appropriate region in the VGA protocol
	ldi DAC_OUT_TRI, 0b00000100	;1 loads PB2 bit into DAC_OUT_TRI


	; configure tristates to read from SRAM 2
	; ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

	;	1) PORT B0 will be set to 'HIGH' output, this will enable TRI 1,5,6,7,8, SRAM 1 (WE), and SRAM 2 (OE)
	sbi portb,0			;2

	;	2) PORT B1, connected to TRI 4, is driven 'HIGH', high impedance output, this disconnects the data lines of SRAM 1 from the DAC input
	sbi portb,1			;2

	;	3) PORT B2, connected to TRI 9, is driven 'HIGH', high impedance output, until VGA RGB output region where the data lines of SRAM 2 are connected to the DAC input 
	in r20, portb			;1 
	or r20, DAC_OUT_TRI		;1 
	out portb, r20			;1


	; communicates to uC1 indicating it can continue writing the next frame
	; ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

	cbi portb, 3			;2 pulls the transmit line 'low', indicating to uC1 that the frame has been swapped and that it can begin writing the new frame

	; Delay 18 cycles
    ldi  r18, 6
LT_DELAY_2: dec  r18
    brne LT_DELAY_2
    

	sbi portb, 3			;2 pulls the transmit 'high'



	jmp H_SYNC_LOW		;4

//**********************************************************************************************************************************************************************************************************


delay_and_skip:

	; Delay 35 cycles
	ldi  r18, 11
LD_S: dec  r18
    brne LD_S
    nop

	jmp H_SYNC_LOW		;4


//==========================================================================================================================================================================================================
//==========================================================================================================================================================================================================


H_SYNC_LOW:


; ========================================================================================

	
	
	
; **** H_Sync_Active_low (76 cycles)
	
	ldi r20, 0b00000000		;1
	out portd, r20		;1 sets H_sync active low, V_sync high and RGB pins low

; ========================================================================================	
	; Delay 74 cycles
	; ---------------
	
    ldi  r18, 24
L3: dec  r18
    brne L3
    rjmp PC+1
; ========================================================================================
	

	ldi r20, 0b00000011			;1
	out portd, r20		;1 sets H_sync high, V_sync high and RGB pins low

	
	
; **** Delay (38 cycles)
	 
	 
	
; ========================================================================================	
	; Delay 22 cycles
    ldi  r18, 7
L_V_BACK_PORCH_2: dec  r18
    brne L_V_BACK_PORCH_2
    nop
; ========================================================================================
	
	
	
	

	ldi r18, low(1)			;1
	ldi r19, high(1)		;1
	

	sub r16,r18				;1
	sbc r17,r19				;1	
	
	
	brne V_BACK_PORCH_DELAY		; 2 cycles if true, 1 if false	
	rjmp H_LOOP_START			; 2 cycles 



V_BACK_PORCH_DELAY:
	nop						;1
	nop						;1
	nop						;1
	nop						;1
	nop						;1
	nop						;1
	jmp V_BACK_PORCH		;2
	
	
	
	; 4 cycles are already being used up by next section
	; this delay can only be 34 clk cycles
	
	
	
	
	
; ****************************************************************************************
; **** HORIZONTAL LOOP - BEGIN (LOOPS 480 times)
; ****************************************************************************************
H_LOOP_START:

	



	ldi r16,low(480)		;1 holds LSB of loop value
	ldi r17,high(480)		;1 hold MSB of loop value
	
	
	
	;31 (NEED 38)
	
H_LOOP:
	
	

//==========================================================================================================================================================================================================
// RGB OUT FROM SRAM 
//==========================================================================================================================================================================================================
// TOTAL CLK CYCLES: 518 (of which 7 are stolen from vertical back porch and 3 from the horizontal front porch)
	
	; 2 clk cycles to output addr + 5 clk cycles to make tristate active (CAN BE STOLEN FROM PREVIOUS REGION), 508 clk cycles, 3 clk cycles to make tristate inactive (STEAL FROM HORIZONTAL FRONT PORCH)

	; NOTE:
	;		- the vertical count needs to be incremented in the horizontal back porch. (REMEMBER IT GETS CLEARED AT THE BEGINNING OF EACH NEW FRAME DURING V_BACK_PORCH)
	;		- the horizontal count needs to be cleared in the horizontal back porch. (REMEMBER IT ALSO GETS CLEARED AT THE BEGINNING OF EACH NEW FRAME DURING V_BACK_PORCH)

	; OUTPUT LOWER 7 BIT OF ADDR
	out portc, V_COUNT		;1

	; OUTPUT UPPER 8 BIT OF ADDR
	out porta, H_COUNT		;1


	; SET TRISTATE LOW
	in r18, portb			;1 
	mov r19, DAC_OUT_TRI	;1 
	com r19					;1
	and r18,r19				;1
	out portb,r18			;1


		; !!!!!!!!!!!!RGB OUT SECTION BEGINS!!!!!!!!!!!!!!!!!



	; 508 CLOCK CYCLE LOOP
	;-----------------------------------------


	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 10



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1
	

	; 20



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 30



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 40



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 50



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1
	

	; 60



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 70



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 80



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 90



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1
	

	; 100



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 110



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 120



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 130



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1
	

	; 140



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 150



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 160

			

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 170



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 180



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 190



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1
	

	; 200



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 210



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 220



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 230



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1
	

	; 240



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 250



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 260



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 270



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1
	

	; 280



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 290



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 300



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 310



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1
	

	; 320



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 330



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 340



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 350



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1
	

	; 360



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 370



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 380



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 390



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1
	

	; 400



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 410



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 420

			

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 430



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 440



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 450



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1
	

	; 460



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 470



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 480



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1


	; 490



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1
	

	; 500



	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1

	inc H_COUNT				;1
	out porta, H_COUNT		;1



	; 508

	;-----------------------------------------


	; SET TRISTATE HIGH
	in r18, portb			;1 
	or r18, DAC_OUT_TRI		;1 
	out portb, r18			;1



























; **** HORIZONTAL FRONT PORCH (13 cycles)


	; **NOTE: Only 8 cycles are required as 2 are already used up for H_sync drive low
	;		  in the next region and 3 have been used up by the previous RGB region.
	
	
; ========================================================================================
	; delay 8 cycles
	; ---------------
    lpm
    lpm
    rjmp PC+1
; ========================================================================================
	
	
	
	
	
	
; **** H-SYNC DRIVE LOW (76 cycles)


	cbi portd,0		;2 drives h-sync active low 

; ========================================================================================
   	; Delay 74 cycles
	; ---------------
  
    ldi  r18, 24
L6: dec  r18
    brne L6
    rjmp PC+1
  
; ========================================================================================

	sbi portd,0		;2 drives h-sync high



	
	
	
; **** HORIZONTAL BACK PORCH (38 cycles)

  	; **NOTE: Only 29 cycles are required as 2 cycles are used to up jumping to beginning of H 
	;         loop and 7 are required to set up RGB in next region.
	
	
; ========================================================================================
	; Delay 29 cycles
	; ---------------
	
	ldi H_COUNT, 0		;1	resets the horizontal counter



	ldi r18, low(1)		;1
	ldi r19, high(1)	;1

	sub r16,r18			;1	
	sbc r17,r19			;1 decrements the 16 bit value vertical loop counter by 1




	mov r18, r16			;1 copies lower half of vertical loop counter into r18
	andi r18,3				;1 checks if vertical loop counter value is a factor of 4, if so, counter is incremented
	cpi r18, 0				;1 compares r18 with zero, if equal, number is a factor and is incremented, otherwise do nothing
	breq V_COUNT_INC		;2/1	2 cycles if true, 1 if false, 
	rjmp  SKIP_V_COUNT_INC	;2




V_COUNT_INC:
	inc V_COUNT				;1	increments V_COUNT by 1


SKIP_V_COUNT_INC:
	; 8 cycle delay
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop



	ldi r18, low(0)		;1
	ldi r19, high(0)	;1

	cp r16,r18			;1	
	cpc r17,r19			;1	checks if vertical loop counter is equal to zero
	
; ========================================================================================

	

	brne H_LOOP_JUMP		; 2 cycles if true, 1 if false	
	rjmp V_FRONT_PORCH			; 2 jumps to the beginning of V_F_PORCH




H_LOOP_JUMP:
	JMP H_LOOP				; 4 
	
	
	
	
	
	
; ****************************************************************************************
; **** HORIZONTAL LOOP - END
; ****************************************************************************************













V_FRONT_PORCH:


; **** VERTICAL FRONT PORCH (10 lines, 6350 cycles)


	; further 10 cycle delay required here, as only 26 of previos loop have been used up
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	
	




	ldi r16,low(10)			;1 holds LSB of loop value
	ldi r17,high(10)		;1 hold MSB of loop value
	


V_FRONT_PORCH_LOOP:


  ; **** Delay (521 cycles)

  ; NOTE: 2 have been used up by h-sync

  
  
; ========================================================================================	
	; Delay 519 cycles
	; ----------------
   
    ldi  r18, 173
L8: dec  r18
    brne L8
; ========================================================================================

	
	
	
; **** H_Sync_Active_low (76 cycles)
	
	cbi portd,0			;2 sets H_sync pin active low

; ========================================================================================	
	; Delay 74 cycles
	; ---------------
	
    ldi  r18, 24
L9: dec  r18
    brne L9
    rjmp PC+1
; ========================================================================================
	

	sbi portd,0			;2 sets H_sync pin high

	
	
; **** Delay (38 cycles)
	 
	 
	
; ========================================================================================	
	; Delay 32 cycles
   
   
	; delay 28
    ldi  r18, 9
L10: dec  r18
	brne L10
    nop
   
   
	; delay 4
   	ldi r18, low(1)			;1
	ldi r19, high(1)		;1

	sub r16,r18				;1
	sbc r17,r19				;1	
; ========================================================================================
	
	
	
	


	
	
	brne V_FRONT_PORCH_DELAY_AND_LOOP	; 2 cycles if true, 1 if false	
	
	
	
	

	nop				;1
    rjmp  V_LOOP	;2  relative jump to start of vertical loop
	

	
	
	
	
	
	
	
	
	
; ****************************************************************************************
; **** VERTICAL LOOP - END
; ****************************************************************************************



V_FRONT_PORCH_DELAY_AND_LOOP:
	nop						;1
	nop						;1
	RJMP V_FRONT_PORCH_LOOP		;2



