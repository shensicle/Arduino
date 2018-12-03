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

#include <LCDBatteryItem.h>
	
#include <Arduino.h>


// --------------------------------------------------------------
// Constructor
LCDBatteryItem::LCDBatteryItem (shen_LCD*      theLCD,
		                unsigned char  theLabelOffset, 
		                Vcc*           voltageMonitor)
		: LCDItemBase (theLCD, theLabelOffset)
{
	
	VoltageMonitor = voltageMonitor;
	Millivolts = 0;
}



// --------------------------------------------------------------
// Called periodically by application to update the display (in case values
// have been changed by the terminal interface or other system components.
// In this default version, DisplayCurrentValue() is called as long as
// we are not in error mode or edit mode.
void LCDBatteryItem::Update (void)
{
	int isNewValue = FetchData();
        
	if (isNewValue)
		DisplayCurrentValue();
}


// --------------------------------------------------------------
// Display the current value of this LCD item on the second line of
// the display.
void LCDBatteryItem::DisplayCurrentValue (void)
{
	// Temporary storage for our string. The "+1" provides space for the
	// null terminator.
	char outputString[ColumnsSupported+1];
	
	unsigned long wholeVolts = Millivolts / 1000;
	unsigned long fractionalVolts = (Millivolts % 1000);

	// If the battery voltage is below 4 volts, start the display with
	// "Low ", otherwise start with "Ok  "
	if (wholeVolts < 4)
	{		
		snprintf (outputString, 13, "Low     %01lu.%01lu V", wholeVolts, fractionalVolts);
	}
	else
	{
		snprintf (outputString, 13, "Ok       %01lu.%01lu V", wholeVolts, fractionalVolts);
	}

	RenderLineRightJustified (outputString);
	
}
	
// --------------------------------------------------------------
// Fetch data that this object will display on the second line and load
// object attributes accordingly. This is intended to be called from the
// OnEntry() and Update() methods of derived classes. This method returns
// a flag which, when set, indicates that the data value has changed since
// the last time FetchData() was called. We do this because LCD updates 
// are fairly time-consuming and we only want to do them when necessary.
int LCDBatteryItem::FetchData (void)
{
    long oldMillivolts = Millivolts;
    
    // Check battery status
    Millivolts = VoltageMonitor->Get();

    // We only care about a resolution down to 1/10 of a volt for our
    // purposes, so zero out the 2 least significant digits.
    Millivolts = Millivolts / 100;
    Millivolts = Millivolts * 100;
    
    if (Millivolts == oldMillivolts)
    	    return (0);
    else
    	    return (1);
}


