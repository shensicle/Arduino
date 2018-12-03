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

#include <Button.h>

  
// ---------------------------------------------------------------------------
Button::Button (byte thePin, unsigned long debounceInterval, unsigned long updateInterval)
{
	// Save the configuration values
	ThePin = thePin;
	DebounceInterval = debounceInterval;
	UpdateInterval = updateInterval;
	
	// Push buttons will make input high, so disable input pull-up resistors
	digitalWrite (thePin, LOW);

	// Program the DIO line used for the push button
	pinMode (thePin, INPUT);  


	// Can't have a debounce interval less than the update interval. If we
	// do, correct it.
	if (DebounceInterval < UpdateInterval)
		DebounceInterval = UpdateInterval;
	
	
}
 
// --------------------------------------------------------------------------- 
void Button::Update (void)
{
	int val = digitalRead (ThePin);

	// If a button press is underway
	if (val == HIGH)
	{
		// For how long ?
		ButtonPressTime += UpdateInterval;
		
		// If the button has been pressed long enough ...
		if ((ButtonPressTime >= DebounceInterval) && (ButtonPressDelivered == false))
		{
			ButtonPressAvailable = true;
		}
		
	}
	else // button is not being pressed
	{
		ButtonPressTime = 0;
		ButtonPressAvailable = false;
		ButtonPressDelivered = false;
	}	
}
 
// ---------------------------------------------------------------------------
bool Button::WasPressed (void)
{
	bool returnValue = false;
	
	if (ButtonPressAvailable)
	{
		returnValue = true;
		ButtonPressAvailable = false;
		ButtonPressDelivered = true;
	}
	
	return (returnValue);
}

