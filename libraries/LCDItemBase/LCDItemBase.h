#ifndef _LCDITEMBASE_H
#define _LCDITEMBASE_H

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

#define MAX_LCD_COLUMNS 16

// Make it easier to support LCD modules from different manufacturers
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
typedef  Adafruit_RGBLCDShield shen_LCD;

extern PROGMEM const char* LCDStringTable[];

class LCDItemBase
{
protected:
	bool IsInEditMode;
	bool IsInErrorMode;
	
	// Number of rows and columns our display supports
	unsigned char RowsSupported, ColumnsSupported;
	
	// Offset into LCDStringTable of the label for this display item
	unsigned char LabelOffset;
	
	// Returns a flag which, when set, indicates that the results of an edit
	// are valid.
	virtual bool IsValid (void) = 0;
	
	// Pointer to the LCD display object
	shen_LCD* TheLCD;
	
	
	// Called when edit mode has been terminated and the results of the edit
	// have been determined to be valid. In most cases, this will be
	// overriden in derived classes to take some action based on the results
	// of the edit (like saving the new configuration). On its own, this
	// method restores the non-edit display and sets IsInEditMode to false.
	virtual void ApplyEdit (void);
	
	// Display the current value of this LCD item on the second line of
	// the display.
	virtual void DisplayCurrentValue (void) = 0;
	
	// Render the specified line of the display right-justified. Leading 
	// blanks are added as necessary. Parameter is a null-terminated 
	// string.Line number defaults to 1 (ie. the second line).
	// Returns the display column (starting at 0) of the first character in the string.
	int RenderLineRightJustified (char* theString, 
		                      unsigned short lineNum = 1);
	
	// Render the specified line of the display center-justified. Leading 
	// and trailing blanks are added as necessary. Parameter is a 
	// null-terminated string.Line number defaults to 0 (ie. the first line).
	// Returns the display column (starting at 0) of the first character in the string.
	int RenderLineCenterJustified (char* theString, 
	   	                       unsigned short lineNum = 0);

	// Render the line in LCDStringTable specified by the passed-in offset
	// to the display center-justified. Leading and trailing blanks are added as 
	// necessary. Line number defaults to 0 (ie. the first line).
	// Returns the display column (starting at 0) of the first character in the string.
	int RenderFlashStringCenterJustified (unsigned char stringIndex,
					      unsigned short lineNum = 0);
	
	// Display an error when editing fails. Ideally, derived classes will
	// override.
	virtual void DisplayError (void) {RenderLineRightJustified ("ERROR"); }
	
	// Fetch data that this object will display on the second line and load
	// object attributes accordingly. This is intended to be called from the
	// OnEntry() and Update() methods of derived classes. This method returns
	// a flag which, when set, indicates that the data value has changed since
	// the last time FetchData() was called. We do this because LCD updates 
	// are fairly time-consuming and we only want to do them when necessary.
	virtual int FetchData (void) = 0;
	
public:
	// Constructor
	LCDItemBase (shen_LCD*      theLCD,
		     unsigned char  theLabelOffset, // Offset of label in LCDStringTable 
		     unsigned char  rowsSupported = 2, 
		     unsigned char  columnsSupported = 16);
	
	// Handle a left button press. This should only be called when we are
	// in edit mode.
	virtual void OnLeft (void) = 0;
	
	// Handle a right button press. This should only be called when we are
	// in edit mode.
	virtual void OnRight (void) = 0;
	
	// Handle an up button press. This should only be called when we are
	// in edit mode.
	virtual void OnUp (void) = 0;
	
	// Handle a down button press. This should only be called when we are
	// in edit mode.
	virtual void OnDown (void) = 0;
	
	// Handle an Enter button press. This should only be called when we are
	// in edit mode.
	virtual void OnEnter (void);
	
	// Handle a cancel button press. This should only be called when we are
	// in edit mode.
	virtual void OnCancel (void);
	
	// Called when this item becomes active on the display. This method
	// displays the item title passed into the constructor on line 1 and
	// Calls DisplayCurrentValue() to display the item's value on line 2.
	virtual void OnEntry (void);
	
	// Returns a flag which, when set, indicates that this item is being
	// edited
	bool IsBeingEdited (void) { return (IsInEditMode); }
	
	// Must be defined in derived classes. Should change the display so that
	// there is a visual indication that value is being edited.
	virtual void OnEditModeEntry (void);
	
	// Called periodically by application to update the display (in case values
	// have been changed by the terminal interface or other system components.
	// In this default version, DisplayCurrentValue() is called as long as
	// we are not in error mode or edit mode.
	virtual void Update (void);
};

#endif

