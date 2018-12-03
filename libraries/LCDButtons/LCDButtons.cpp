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

#include <LCDButtons.h>

//const int LCDButtons::KeyValues [NUM_KEYS] = {30, 150, 360, 535, 760 };
const int LCDButtons::KeyValues [NUM_KEYS] = {180, 200, 300, 460, 800 };

  
// ---------------------------------------------------------------------------
LCDButtons::LCDButtons (unsigned long updateInterval, unsigned long debounceInterval, byte analogPin)
{
	// Save the configuration values
	AnalogPin = analogPin;
	DebounceInterval = debounceInterval;
	UpdateInterval = updateInterval;
	
	LastKeyPress = NO_KEY;
	

	// Can't have a debounce interval less than the update interval. If we
	// do, correct it.
	if (DebounceInterval < UpdateInterval)
		DebounceInterval = UpdateInterval;
	
	
}
 
// --------------------------------------------------------------------------- 
void LCDButtons::Update (void)
{
	int val = analogRead (AnalogPin);

	// Determine what key was pressed, if any
	int currKey = NO_KEY;
    
	for (int key = 0; (key < NUM_KEYS) && (currKey == NO_KEY); key++)
	{
		if (val < KeyValues[key])
		{
			currKey = key;
		}
	}
    

	// If we have a key press
	if (currKey != NO_KEY)
	{
		// If this button was pressed the last time we were called ...
		if (currKey == LastKeyPress)
		{	
			// Update the indicator of how long it has been pressed
			ButtonPressTime += UpdateInterval;
		
			// If the button has been pressed long enough ...
			if ((ButtonPressTime >= DebounceInterval) && (ButtonPressDelivered == false))
			{
				ButtonPressAvailable = true;
			}
		}
		else
		{
			// This is a brand new key press
			LastKeyPress = currKey;
			ButtonPressTime = 0;
			ButtonPressAvailable = false;
			ButtonPressDelivered = false;
		}
		
	}
	else // button is not being pressed
	{
		LastKeyPress = NO_KEY;
		ButtonPressTime = 0;
		ButtonPressAvailable = false;
		ButtonPressDelivered = false;
	}	
}
 
// ---------------------------------------------------------------------------
// Returns the number of the last button pressed and successfully debounced.
// Returns -1 if no buttons pressed.
int LCDButtons::GetPressed (void)
{
	int returnValue = NO_KEY;
	
	if (ButtonPressAvailable)
	{
		ButtonPressAvailable = false;
		ButtonPressDelivered = true;
		returnValue = LastKeyPress;
	}
	
	return (returnValue);
}

