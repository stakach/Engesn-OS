

****:Engesn OS/1 Function and Command Information:****


1. Console Commands and their parameters.

	*ver:
	-----
	Displays OS version

	- Parameter 'build'
		Displays version build


	*mem:
	-----
	Displays memory statistics.


	*cls:
	-----
	Clears the screen.


	*time:
	------
	Displays system time.

	- Parameter '24'
		Displays 24 hour time.


	*date:
	------
	Displays current date.

	- Parameter 'long'
		Displays a long date format.


	*play:
	------
	Plays a user defined tune.

	- Usage:
		'play ABC;' <-- examples
		'play o2w6ABC;'
		'play ABCp2CD;'
		'play Ap2BC#Do2F#CDw4E;'

		Optional Commands:
		------------------
		* The 'o' stands for Octave, and is followed by the octave number, 1 or 2.
		  (optional command) [Default Octave is 'o1']

		* The 'w' is the length/width of the note(s) to be played. Accepts values from 0 to 9.
		  (optional command) [Default Width is 'w3']

		* The 'p' creates a pause. Values from 0 to 9 determine the length of the pause.
		  (optional command)

		* The 'A' 'B' 'C' 'D' 'E' 'F' 'G' characters are the possible notes that can be played.
		  (These notes can be made sharp by placing the '#' character after the note.)
		   --> The notes 'B' and 'F' cannot be set sharp


	*kbrate:
	--------
	Changes the typeamatic rate and delay.
	
	- Usage:
		'kbrate [Rate value]'

		Rate values:
		*'high' or 'h' sets a high rate and small delay.
		*'medium'or 'm' sets a medium rate and delay.
		*'low' or 'l' sets a low rate and a large delay.
		*'default' or 'd' is the default typeamatic rate.

	
	*screensav:
	-----------
	Turns off the monitor and waits for user input before turning the monitor back on.


	*end:
	-----
	Halts the cpu, ready for the computer to be turned off.


	*restart:
	---------
	Restarts the computer after user input

	- Parameter 'now'
		This causes the computer to restart instantly (no input required)
	
	- Parameter 'cold'
		This forces the computer to redo diagnostics on reboot 
		



2. Function misc information:


	Function Restart:
	-----------------
		*Values for BiosMem*
			0x0000 - Cold boot } ____| Only these options
			0x1234 - Warm boot }     | are used.
			0x4321 - PS/2
			0x5678 - Suspend system
			0x9abc - Manufacturing test mode
			0xabcd - POST loop mode
			0x0064 - Burn-in mode
	-----------------
