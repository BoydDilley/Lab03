/*!****************************************************************************
 *
 * \file Lib_LCD.c
 *
 * \brief File for FreeRTOS LCD implementation
 *
 * \author  
 *
 * \details Contains all functions for two line LCD operation in FreeRTOS
 *			
 *			
 *
 * Modification History:
 * 11/18/2013 - Pulled all Functions in
 * 11/16/2013 - Original File
 *
 ******************************************************************************
 */
 
 
 /* #includes go here */
 
/*****************************************************************************/
/****************************/
/*Library LCD Initialization*/
/****************************/

/*!****************************************************************************
*
* \fn vLCD_INITIALIZATION(void)
*
* \brief Function to initialize the LDC 
*
* \details Function is called to initialize the LCD into either 
*		   4 bit or 8 bit mode off of the flowcharts on pages 26 and 27
*		   of the KS6600U datasheet 
*
* \params[in] none
*
* \returns nothing
*
* Modification History:
*
* 11/17/2013 - Original Function
*
*******************************************************************************/
void vLCD_INITIALIZATION(void)
{
	unsigned char Instructions = 0x00;
	
	/*! Initialize the Data Bit port for initially low output*/
	LDDR = 0xFF;
	LDP =  0x00;
	
	/*! Initialize the Control Bit port for initially low output*/
	LCDR = 0xFF;
	LCP =  0x00;	
	
	#ifdef BITMODE4
		/*! Delay more than 30ms after powering up*/
		_delay_ms(35);
		
		/***************************************************************************/
		/*! ###Function set###
		/***************************************************************************/
		
		/*!
		 * Set LCD in one or two line mode
		 * Set LCD Font type 5x11 dots or 5x8 dots
		 */
		
		Instructions = (1<<LCD_D5);
		
		vWRITE_COMMAND_TO_LCD(INSTR_WR, Instructions);
		vWRITE_COMMAND_TO_LCD(INSTR_WR, Instructions);
		
		Instructions = (TWO_LINE_MODE<<LCD_D7)|(DISPLAY_ON<<LCD_D6);
		
		vWRITE_COMMAND_TO_LCD(INSTR_WR, Instructions);
		
		/*! Delay more than 39us*/
		_delay_us(50);
		
		/***************************************************************************/
		/*! ###Display ON/OFF Control###
		/***************************************************************************/
		
		/*!
		 * Set LCD to have display on
		 * Set LCD Cursor on or off
		 * Set Cursor to blink or not
		 */
		
		Instructions = 0x00;
		
		vWRITE_COMMAND_TO_LCD(INSTR_WR, Instructions);
		
		Instructions = (1 << LCD_D7) | 
			  (DISPLAY_ON << LCD_D6) | 
			   (CURSOR_ON << LCD_D5) | 
		 (CURSOR_BLINK_ON << LCD_D4));
		 
		 vWRITE_COMMAND_TO_LCD(INSTR_WR, Instructions);
		 
		/*! Delay more than 39us*/
		_delay_us(50);
		
		/***************************************************************************/
		/*! ###Display Clear###
		/***************************************************************************/
		
		Instructions = 0x00;
		
		vWRITE_COMMAND_TO_LCD(INSTR_WR, Instructions);
		
		/*! Write Commands to clear the screen*/
		
		Instructions = (1<<LCD_D4);
		
		vWRITE_COMMAND_TO_LCD(INSTR_WR, Instructions);
		
		/*! Delay more than 1.53ms*/
		_delay_us(1600);
		
		/***************************************************************************/
		/*! ###Entry Mode set###
		/***************************************************************************/
		
		Instructions = 0x00;
		
		vWRITE_COMMAND_TO_LCD(INSTR_WR, Instructions);
		
		/*!
		 * Set LCD in Increment or decrement mode
		 * Set LCD to entire shift mode if desired
		 */
		
		Instructions =  (1 << LCD_D6) | 
		   (INCREMENT_MODE << LCD_D5) | 
	    (ENTIRE_SHIFT_MODE << LCD_D4);
		
		vWRITE_COMMAND_TO_LCD(INSTR_WR, Instructions);
	#endif;
	
	#ifdef BITMODE8
	
		/*! Delay  more than 30ms after powering up*/
		_delay_ms(35);
		
		/***************************************************************************/
		/*! ###Function set###
		/***************************************************************************/
		
		/*!
		 * Set LCD in one or two line mode
		 * Set LCD Font type 5x11 dots or 5x8 dots
		 */
		
		Instructions = (1 << LCD_D5) |
					   (1 << LCD_D4) | 
		   (TWO_LINE_MODE << LCD_D3) | 
		      (DISPLAY_ON << LCD_D2);	
			  
		vWRITE_COMMAND_TO_LCD(INSTR_WR, Instructions);
			  	
		/*! Delay more than 39us*/
		_delay_us(50);
		
		/***************************************************************************/
		/*! ###Display ON/OFF Control###
		/***************************************************************************/
		
		/*!
		 * Set LCD to have display on
		 * Set LCD Cursor on or off
		 * Set Cursor to blink or not
		 */
		
		Instructions = (1 << LCD_D3) | 
		      (DISPLAY_ON << LCD_D2) | 
			   (CURSOR_ON << LCD_D1) | 
		 (CURSOR_BLINK_ON << 0);
		 
		 vWRITE_COMMAND_TO_LCD(INSTR_WR, Instructions);
		 
		/*! Delay more than 39us*/
		_delay_us(50);
		
		/***************************************************************************/
		/*! ###Display Clear###
		/***************************************************************************/
		
		/*! Write Commands to clear the screen*/
		
		Instructions = 0x01;
		
		vWRITE_COMMAND_TO_LCD(INSTR_WR, Instructions);
		
		/*! Delay more than 1.53ms*/
		_delay_us(1600);
		
		/***************************************************************************/
		/*! ###Entry Mode set###
		/***************************************************************************/
		
		/*!
		 * Set LCD in Increment or decrement mode
		 * Set LCD to entire shift mode if desired
		 */
		
		Instructions =  (1 << LCD_D2) | 
		   (INCREMENT_MODE << LCD_D1) | 
		(ENTIRE_SHIFT_MODE << 0);
		
		vWRITE_COMMAND_TO_LCD(INSTR_WR, Instructions);
		CURSOR_X_POSITION = 0;
		
	#endif;
	
}

/*****************************************************************************/
/*************************************/
/*Library LCD Write and Write Command*/
/*************************************/

/*!****************************************************************************
*
* \fn vWRITE_COMMAND_TO_LCD(void)
*
* \brief Function to write commands to the LCD
*
* \details Function is write commands to the LCD using the read/write 
*		   commands and the register select 
*
* \params[in] RS, data
*
* \returns nothing
*
* Modification History:
*
* 11/17/2013 - Original Function
*
*******************************************************************************/
void vWRITE_COMMAND_TO_LCD(char RS, char data)
{
	/*! Set DB7 pin to read*/
	LDDR = LDDR & 0x7F;
	
	/*! Wait for Busy flag to clear*/
	while(LDP & 0x80);
	
	/*! Set LCD_D7 pin for output*/
	LDDR = LDDR | (1 << LCD_D7);
	
	if(RS == DATA_WR) LCP = 1 << LCD_RS;	/*Set RS high to write data*/ 
	else LCP = 0x00;	/*Set RS low to write instructions*/
	
	/*! Enable display for use*/
	LCP = LCP | 1<<LCD_E; 
	
	/* Write Data*/
	LDP = data;
	
	/*! Wait for Busy flag to clear*/
	while(LDP & 0x80); 
	
	/*Set Read/Write low*/
	LCP = LCP & 1<<LCD_RS;
}


/*!****************************************************************************
*
* \fn vLCD_WRITE_STRING(char *str_ptr)
*
* \brief Function to write strings to the LCD
*
* \details Function is used to take in a string of characters and display 
*		   the string to the LCD
*
* \params[in] *str_ptr
*
* \returns nothing
*
* Modification History:
*
* 11/17/2013 - Original Function
* 11/23/2013 - Added Code
* 11/30/2013 - Limit bottom line to prevent rollover
*
******************************************************************************
*/
void vLCD_WRITE_STRING(char *str_ptr)
{	
	uint8_t character;
	while(*str_ptr != '\0')		//move through the string until the end is reached
	{
		/*! If text wrap is enabled */
		#ifdef configTEXT_WRAP
			#if configTEXT_WRAP == 1
		
				/*! If the LCD is at the end of the top line */
				if( xLCD_Get_Length()<= 0 && CURSOR_Y_POSITION == 0)
					vLCD_HOME_BOTTOM_LINE(); //Continue on to the bottom line
		
			#endif
		#endif
		/*! Limit the length of the bottom line to prevent rollover */
		if (!(xLCD_Get_Length() <= 0 && CURSOR_Y_POSITION == 1))
		{
			character = *str_ptr++; //increment pointer
			vWRITE_COMMAND_TO_LCD(DATA_WR, character); //print character
		}			
	}	
}

/*****************************************************************************/

/*****************************************************************************/
/*************************/
/*Library Clear Functions*/
/*************************/

/*!****************************************************************************
*
* \fn vLCD_CLEAR(void)
*
* \brief Function to clear the LCD Display
*
* \details Function is called to clear all data displayed on the LCD. Also
*		   returns the cursor to the home location on the top row. The command
*		   used to clear the display takes 1.53ms to run. Proper time needs
*		   allocated to ensure no commands are sent to the display for that time
*	
* \params[in] nothing
*	
* \returns nothing	
*
* Modification History:
*
* 11/17/2013 - Original Function
* 11/24/2013 - Added code to function
*
******************************************************************************
*/
void vLCD_CLEAR(void)
{
	/*! Call write command to send 0x01 command (clear) to the controller */
	vWRITE_COMMAND_TO_LCD(INSTR_WR, 1 << LCD_CLEAR_INSTRUCTION);
}

/*!****************************************************************************
*
* \fn vLCD_CLEAR_TOP(void)
*
* \brief Function to clear the top line of the LCD Display
*
* \details Calls the function to set the cursor to the home position of the top
*		   row, write 24 spaces as a string, then return to the home position
*		   of the top row.
*	
* \params[in] nothing
*	
* \returns nothing	
*
* Modification History:
*
* 11/17/2013 - Original Function
* 11/24/2013 - Added code to function
*
******************************************************************************
*/
void vLCD_CLEAR_TOP(void)
{
	/*! Call function to set cursor for top row's home position */
	vLCD_HOME_TOP_LINE();
	/*! Call function to write string of 24 spaces to clear the top row */
	vLCD_WRITE_STRING("                        ");
	/*! Call function to set cursor for top row's home position */
	vLCD_HOME_TOP_LINE();
}

/*!****************************************************************************
*
* \fn vLCD_CLEAR_BOTTOM(void)
*
* \brief Function to clear the bottom line of the LCD Display
*
* \details Calls the function to set the cursor to the home position of the
*		   bottom row, write 24 spaces as a string, then return to the home 
*		   position of the bottom row.
*	
* \params[in] nothing
*	
* \returns nothing	
*
* Modification History:
*
* 11/17/2013 - Original Function
* 11/24/2013 - Added code to function
*
******************************************************************************
*/
void vLCD_CLEAR_BOTTOM(void)
{
	/*! Call function to set cursor for bottom row's home position */
	vLCD_HOME_BOTTOM_LINE();
	/*! Call function to write string of 24 spaces to clear the bottom row */
	vLCD_WRITE_STRING("                        ");
	/*! Call function to set cursor for bottom row's home position */
	vLCD_HOME_BOTTOM_LINE();
}

/*!****************************************************************************
*
* \fn vLCD_ON_OFF(void)
*
* \brief Function to toggle LCD on and off
*
* \details Checks the current status of the LCD. If the LCD is on, toggle it
*		   so it's off. If the LCD is off, turn it on. Uses the vLCD_WRITE
*		   command to send instructions to the LCD.
*	
* \params[in] nothing
*	
* \returns nothing	
*
* Modification History:
*
* 11/18/2013 - Original Function
* 11/24/2013 - Added code to function
*
******************************************************************************
*/
void vLCD_ON_OFF(void)
{
	/*! Create command to toggle LCD Display */
	uint8_t LCD_Command = 
		(1 << LCD_ON_OFF_INSTRCUTION) |
		(OnOffStatus << LCD_ON_INSTRUCTION) |
		(configCURSOR_SHOW << LCD_CURSOR_SHOW_INSTRUCTION) |
		(configCURSOR_BLINK << LCD_CURSOR_BLINK_INSTRUCTION);
	
	/*! Toggle the OnOffStatus tracker variable */
	OnOffStatus = OnOffStatus ^ 0x01;
	
	/*! Toggle LCD */
	vWRITE_COMMAND_TO_LCD(0, LCD_Command);
}
/*****************************************************************************/

/*****************************************************************************/
/***************************/
/*Library Length Functions*/
/*************************/

/*!****************************************************************************
 *
 * \fn xGet_Top_Length(void)
 *
 * \brief Function to get the remaining characters on the LCD
 *
 * \details Function is called to calculate the amount of characters
 *			remaining on the current line
 *
 *			This allows for tracking the amount of line space available.
 *			
 * \params[in] 	nothing
 *			
 * \returns Number of characters remaining on the current line			
 *
 * Modification History:
 *
 * 11/17/2013 - Original Function
 * 11/23/2013 - Added Code
 *
 ******************************************************************************
 */
uint8_t xLCD_Get_Length(void)
{
	return = 24 - CURSOR_X_POSITION; //returns number of characters left in the line
}

/*****************************************************************************/

/*****************************************************************************/
/************************/
/*Library Home Functions*/
/************************/

/*!****************************************************************************
 *
 * \fn vLCD_GO_TO_Position(uint8_t, uint8_t)
 *
 * \brief Function to directly control LCD cursor Position
 *
 * \details Function is called to move the LCD cursor to a specific location 
 *				on the LCD.
 *
 *			Allows for direct control of where data can be written, rather
 *				than only from the beginning of a line
 *
 *			Saves current cursor position for text wrapping
 *			
 * \params[in] 	Character, Row 	
 *			
 * \returns nothing			
 *
 * Modification History:
 *
 * 11/15/2013 - Original Function
 *
 ******************************************************************************
 */
void vLCD_GO_TO_POSITION(uint8_t x, uint8_t y)
{
	//save the current position
	register uint8_t DDRAMAddr;
	
	// For each line o the LCD
	switch(y)
	{
		case 0: 
			//for the top line, set the DDRAM address 
			//move a certain number of characters to the right
			DDRAMAddr = LCD_LINE0_DDRAMADDR + x;
		break;
		
		case 1: 
			//for the bottom line, set the DDRAM address 
			//move a certain number of characters to the right
			DDRAMAddr = LCD_LINE1_DDRAMADDR + x;
		break;
		
		default: 
			//default to the top left of the LCD if nothing is specified
			DDRAMAddr = LCD_LINE0_DDRAMADDR+x;
	}
	
	//save current cursor position X
	CURSOR_X_POSITION = x;
	//save current cursor position Y
	CURSOR_Y_POSITION = y;
	
	// send a command to set the data address
	vWRITE_COMMAND_TO_LCD(INSTR_WR, 1 <<LCD_DDRAM | DDRAMAddr);
}

/*!****************************************************************************
 *
 * \fn vLCD_HOME_TOP_LINE(void)
 *
 * \brief Function to return to home position on the top line of the LCD
 *
 * \details Function is called to return the cursor to the first position
 *				on the top line of the LCD.
 *			
 *			This allows for the the top line to be ready to write 
 *				independently from the bottom line.
 *			
 * \params[in] none		
 *			
 * \returns nothing			
 *
 * Modification History:
 *
 * 11/15/2013 - Original Function
 *
 ******************************************************************************
 */
void vLCD_HOME_TOP_LINE(void)
{
	//move cursor to the top left position of the LCD
	vWRITE_COMMAND_TO_LCD(INSTR_WR, 1 << LCD_HOME_TOP_LINE);
}

/*!****************************************************************************
 *
 * \fn vLCD_HOME_BOTTOM_LINE(void)
 *
 * \brief Function to return to home position on the bottom line of the LCD
 *
 * \details Function is called to return the cursor to the first position
 *				on the bottom line of the LCD.
 *			
 *			This allows for the the bottom line to be ready to write 
 *				independently from the top line.
 *			
 * \params[in] none		
 *			
 * \returns nothing			
 *
 * Modification History:
 *
 * 11/15/2013 - Original Function
 *
 ******************************************************************************
 */
void vLCD_HOME_BOTTOM_LINE(void)
{
	//move the cursor to the bottom left position on the LCD
	vLCD_GO_TO_POSITION(0,1);
}

/*****************************************************************************/
 
 
