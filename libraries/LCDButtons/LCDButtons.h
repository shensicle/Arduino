
#ifndef _LCDBUTTONS_H
#define _LCDBUTTONS_H

/*
Copyright © 2012 Scott Henwood/shensicle photographic. All Rights Reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

3. The name of the author may not be used to endorse or promote products 
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Scott Henwood/shensicle photographic "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <arduino.h>



class LCDButtons
{
protected:
	
	//Analog input pin our key array is connected to
	byte AnalogPin;
	
	// From the manufacturer - analog readings associated with eack key
	const static int NUM_KEYS = 5;
	static const int KeyValues [NUM_KEYS];
	
	unsigned long DebounceInterval;   // milliseconds
	unsigned long UpdateInterval;     // milliseconds
	
	// How long the current button press has been ongoing
	unsigned long ButtonPressTime;   // milliseconds
	
	// Keeps track of the last key press we detected
	int LastKeyPress;
	
	// A flag which, when set, indicates that the current button press has
	// been delivered to the caller
	bool ButtonPressDelivered;
	
	// A flag which, when set, indicates that a button press is available
	// for delivery to the caller
	bool ButtonPressAvailable;
	
  public:
  	  
    // Value returned by GetPressed when no key press has been detected.
    const static int NO_KEY    = -1;
    const static int RIGHT_KEY = 0;
    const static int UP_KEY    = 1;
    const static int DOWN_KEY  = 2;
    const static int LEFT_KEY  = 3;
    const static int SEL_KEY   = 4;
  
    LCDButtons (unsigned long updateInterval, unsigned long debounceInterval = 100, byte analogPin = 0);
 
    // Called at UpdateInterval to service the button
    void Update (void);
 
    // Returns the number of the last button pressed and successfully debounced.
    // Returns -1 if no buttons pressed.
    int GetPressed (void);
};


#endif
