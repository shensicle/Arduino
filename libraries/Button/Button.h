#ifndef _Button_h
#define _Button_h

/*
Copyright © 2011 Scott Henwood/shensicle photographic. All Rights Reserved.

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

class Button
{
protected:
	
	// Digital I/O pin this button is connected to
	byte ThePin;
	unsigned long DebounceInterval;   // milliseconds
	unsigned long UpdateInterval;     // milliseconds
	
	// How long the current button press has been ongoing
	unsigned long ButtonPressTime;   // milliseconds
	
	// A flag which, when set, indicates that the current button press has
	// been delivered to the caller
	bool ButtonPressDelivered;
	
	// A flag which, whenn set, indicates that a button press is available
	// for delivery to the caller
	bool ButtonPressAvailable;
	
  public:
  
    Button (byte thePin, unsigned long debounceInterval, unsigned long updateInterval);
 
    // Called at UpdateInterval to service the button
    void Update (void);
 
    // Returns true once for each button press that has been debounced.
    bool WasPressed (void);
};


#endif
