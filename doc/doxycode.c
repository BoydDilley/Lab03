/*! \page Usage LCD Library Usage

	\tableofcontents
	
	The purpose of this page is to highlight the functions that can be called
	from the library and how they are used. This library is used to drive an
	LCD using the 
	
	\section Defines Defines
	\subsection Wiring LCD Wiring
	The LCD needs wired so that there are 8 data pins and 3 "command" pins. The
	3 command pins are Register Select (RS), Read/Not Write (R/!W), and Enable
	(E). The 8 data pins are always plugged into their own port, with the data
	pins 0-7 being port pins 0-7. The 3 command pins are plugged into a port,
	with RS being pin 0, RW being pin 1, and E being pin 2. The ports default
	to PORTK for the data pins and PORTJ for the command pins. These may be
	changed via the defines LDP and LCP in the header file. The Data Definition
	Registers LDDR and LCDR should also being changed according to the assigned
	ports.
	
	\subsection wrap Text Wrapping
	The "configTEXT_WRAP" setting is used to define if text wrapping should be
	enabled when calling the "vLCD_WRITE_STRING" command. This automatically
	moves to the second line of the display when the first line is full when set
	to 1.
	
	\subsection bitmode 4-bit/8-bit Mode
	The BITMODE definition is used to define if the display is to be used
	in 8-bit mode or 4-bit mode. A define of "BITMODE8" will set the display
	to use 8-bit mode, where a define of "BITMODE4" will set the display to
	use 4-bit mode.
	
	\subsection Cursor Cursor Blink and Show
	The configuration of the display can be changed so that the cursor is shown,
	or is set to blink. This is done by changing two configuration options.
	configCURSOR_SHOW will show the cursor if set to 1, and hide it if set to 0.
	configCURSOR_BLINK will blink the cursor if set to 1, and hide it if set to
	0.
	
	\subsection twoline	Two Line/One Line Mode
	The number of lines enabled on the display can be either both lines or the
	top line. To enable just the top line, set "TWO_LINE_MODE" to 0. To enable
	both lines, set "TWO_LINE_MODE" to 1.
	
	\subsection font Font Size
	Two font sizes exist for the display, both 5x11 dots and 5x8 dots. When
	"FONT_TYPE" is set to 0, the 5x8 dot format is used. When "FONT_TYPE" is set
	to 1, the 5x11 dot format is used.
	
	\subsection Mode Increment and Shift Mode
	The configuration of the shift mode can be changed to either increment or
	entire shift. Setting the define "INCREMENT_MODE" to 1 will set the display
	to increment, where setting the define "ENTIRE_SHIFT_MODE" to 1 will set the
	display to shift on data write.
	
	\section functions Functions
	\subsection Initialization vLCD_INITIALIZATION()
	The initialization function is used to initially set up the display. This
	function configures the ports used by the display, sets the display's entry
	mode and text size, turns on the display while setting the cursor settings,
	then clears the display. The display should be on, the text cleared, and the
	cursor at the home position.
	
	\subsection write_command vWRITE_COMMAND_TO_LCD(RS,data)
	Writes instructions or characters to the LCD. The input
	parameters for the display are the Register Select state and the data
	to be written. The Register Select state is '0' to write a command and '1'
	to write data. The data input is the value to be put on the data ports of
	the LCD.
	\note This command is not intended to be used directly by the user, but may
	be called if the user wishes to use a command not available through function
	calls contained in this library.
	
	\subsection write_string vLCD_WRITE_STRING(string)
	Writes an input string to the LCD at the cursor's current
	position. This will overwrite and current characters on the display. If text
	wrapping is enabled, the text will automatically move from line 1 to line 2
	if overflow happens.
	
	\subsection clear vLCD_CLEAR()
	Clears both lines of the display and returns the cursor to the
	home position.
	
	\subsection clear_top vLCD_CLEAR_TOP()
	Sets the cursor for the home position of the top row, writes
	a string of 24 spaces, then returns the cursor to the home position of the
	top row.
	
	\subsection clear_bottom vLCD_CLEAR_BOTTOM()
	Sets the cursor for the home position of the bottom row, writes
	a string of 24 spaces, then returns the cursor to the home position of the
	bottom row.
	
	\subsection onoff vLCD_ON_OFF()
	Toggles the on/off status of the LCD. If called, the LCD will
	turn off if it is on, and turn on if it is off.
	\note Using vWRITE_COMMAND_TO_LCD to turn the display on and off will not
	change the variable used to track the display, which may cause undesirable
	results when calling this function.
	
	\subsection length xLCD_Get_Length()
	Returns the number of available characters remaining on the
	LCD as an 8-bit unsigned integer. The value is calculated from the position
	of the cursor.
	
	\subsection goto vLCD_GO_TO_POSITION(x,y)
	Moves the cursor to a position on the LCD. X is the horizontal position
	0-23, and y is the vertical position 0-1.
	
	\subsection tophome vLCD_HOME_TOP_LINE()
	Moves the cursor to the home position of the top line.
	
	\subsection bottomhome vLCD_HOME_BOTTOM_LINE()
	Moves the cursor to the home position of the bottom line.
*/

/*! \page Flowcharts LCD Library Function Flowcharts

	\tableofcontents
	
	\section initialization vLCD_INITIALIZATION()
	\image html "vLCD_INITIALIZATION_FLOWCHART.PNG" "Figure 1 - LCD Initialization Function Flowchart"
	
	\section write vWRITE_COMMAND_TO_LCD()
	\image html "vWRITE_COMMAND_TO_LCD_FLOWCHART.PNG" "Figure 2 - LCD Write Function Flowchart"
	
	\section string vLCD_WRITE_STRING(string)
	\image html "vLCD_WRITE_STRING_FLOWCHART.PNG" "Figure 3 - LCD Write String Function Flowchart"
	
	\section onoff vLCD_ON_OFF()
	\image html "vLCD_ON_OFF_FLOWCHART.PNG" "Figure 4 - LCD On/Off Function Flowchart"
	
	\section clear vLCD_CLEAR()
	\image html "vLCD_CLEAR_FLOWCHART.PNG" "Figure 5 - LCD Clear Function Flowchart"
	
	\section clear_top vLCD_CLEAR_TOP()
	\image html "vLCD_CLEAR_TOP_FLOWCHART.PNG" "Figure 6 - LCD Clear Top Line Function Flowchart"
	
	\section clear_bottom vLCD_CLEAR_BOTTOM()
	\image html "vLCD_CLEAR_BOTTOM_FLOWCHART.PNG" "Figure 7 - LCD Clear Bottom Line Function Flowchart"
	
	\section position vLCD_GO_TO_POSITION(x,y)
	\image html "vLCD_GO_TO_POSITION_FLOWCHART.PNG" "Figure 8 - LCD Go To Position Function Flowchart"
	
	\section tophome vLCD_HOME_TOP_LINE()
	\image html "vLCD_HOME_TOP_LINE_FLOWCHART.PNG" "Figure 9 - LCD Home Top Line Function Flowchart"
	
	\section bottomhome vLCD_HOME_BOTTOM_LINE()
	\image html "vLCD_HOME_BOTTOM_LINE_FLOWCHART.PNG" "Figure 10 - LCD Home Bottom Line Function Flowchart"
	
	\section gettop vLCD_GET_TOP_LENGTH()
	\image html "vLCD_GET_TOP_LENGTH_FLOWCHART.PNG" "Figure 11 - LCD Get Top Length Function Flowchart"
	
	\section getbottom vLCD_GET_BOTTOM_LENGTH()
	\image html "vLCD_GET_BOTTOM_LENGTH_FLOWCHART.PNG" "Figure 12 - LCD Get Bottom Length Function Flowchart"
	
	\section getlength xLCD_GET_LENGTH()
	\image html "xLCD_GET_LENGTH_FLOWCHART.PNG" "Figure 13 - LCD Get Length Function Flowchart"
*/