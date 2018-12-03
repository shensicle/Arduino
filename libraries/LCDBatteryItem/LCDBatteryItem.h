#ifndef _LCDBATTERYITEM_H
#define _LCDBATTERYITEM_H

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

#include <LCDItemBase.h>
#include <VCC.h>

extern PROGMEM const char* LCDStringTable[];

class LCDBatteryItem : public LCDItemBase
{
protected:
	// Pointer to the object from which we retrieve the battery voltage
	Vcc* VoltageMonitor;
	
	// Millivolt reading from the power management circuitry
	long Millivolts;
	
	// Returns a flag which, when set, indicates that the results of an edit
	// are valid.
	virtual bool IsValid (void) { return (true); }
	
	
	// Called when edit mode has been terminated and the results of the edit
	// have been determined to be valid. Since for this class, we don't allow
	// editing, we don't need to do anything here.
	virtual void ApplyEdit (void) {}
	
	// Display the current value of this LCD item on the second line of
	// the display.
	virtual void DisplayCurrentValue (void);
	
	// Fetch data that this object will display on the second line and load
	// object attributes accordingly. This is intended to be called from the
	// OnEntry() and Update() methods of derived classes. This method returns
	// a flag which, when set, indicates that the data value has changed since
	// the last time FetchData() was called. We do this because LCD updates 
	// are fairly time-consuming and we only want to do them when necessary.
	virtual int FetchData (void);
	
public:
	// Constructor
	LCDBatteryItem (shen_LCD*      theLCD,
		        unsigned char  theLabelOffset, 
		        Vcc*           voltageMonitor);
	
	// Handle a left button press. This should only be called when we are
	// in edit mode.
	virtual void OnLeft (void) {}
	
	// Handle a right button press. This should only be called when we are
	// in edit mode.
	virtual void OnRight (void) {}
	
	// Handle an up button press. This should only be called when we are
	// in edit mode.
	virtual void OnUp (void) {}
	
	// Handle a down button press. This should only be called when we are
	// in edit mode.
	virtual void OnDown (void) {}
	
	// Must be defined in derived classes. Should change the display so that
	// there is a visual indication that value is being edited. We don't need
	// to do anything here as this object can't be edited, but we do have to
	// override the base class functionality, which does support editing.
	virtual void OnEditModeEntry (void) {}
	
	// Called periodically by application to update the display (in case values
	// have been changed by the terminal interface or other system components.
	// In this default version, DisplayCurrentValue() is called as long as
	// we are not in error mode or edit mode.
	virtual void Update (void);
};

#endif

