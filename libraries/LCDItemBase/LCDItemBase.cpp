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
	
#include <Arduino.h>

// --------------------------------------------------------------
// Called when edit mode has been terminated and the results of the edit
// have been determined to be valid. In most cases, this will be
// overriden in derived classes to take some action based on the results
// of the edit (like saving the new configuration). On its own, this
// method restores the non-edit display and sets IsInEditMode to false.
void LCDItemBase::ApplyEdit (void)
{

	IsInEditMode = false;
	DisplayCurrentValue();
}

// --------------------------------------------------------------
// Render the second line of the display right-justified. Leading 
// blanks are added as necessary. Parameter is a null-terminated 
// string.
// Returns the display column (starting at 0) of the first character in the string.
int LCDItemBase::RenderLineRightJustified (char* theString, unsigned short lineNum)
{
	// Storage for the output string. The "+1" gives us a place to store
	// the null terminator.
	char outputString[ColumnsSupported+1];
	
	// Counter
	int i;
	
	// Avoids calculating the length of our input string more than once
	int theLength;
	
	// If the string is too long, truncate it to avoid a memory fault
	theLength = strlen (theString);
	if (theLength > ColumnsSupported)
	{
		theString[ColumnsSupported] = 0x00;
		theLength = ColumnsSupported;
	}
	
	// Start output string with the number of leading blanks we need
	for (i = 0; i < (ColumnsSupported - theLength); i++)
	{
		outputString[i] = ' ';
	}
	
	
	// And add the string passed into us
	strcpy (&outputString[i], theString);

	TheLCD->setCursor (0, lineNum);
	TheLCD->println (outputString);
	
	return(i);
}


// --------------------------------------------------------------
// Render the specified line of the display center-justified. Leading 
// and trailing blanks are added as necessary. Parameter is a 
// null-terminated string.Line number defaults to 0 (ie. the first line).
// Returns the display column (starting at 0) of the first character in the string.
int LCDItemBase::RenderLineCenterJustified (char* theString, 
	   	                             unsigned short lineNum)
{
	// Storage for the output string. The "+1" gives us a place to store
	// the null terminator.
	char outputString[ColumnsSupported+1];
	
	// Counters
	int i, j, k;
	
	int returnValue;
	
	j = strlen(theString);

	// Figure out how many leading spaces we need
	i = (ColumnsSupported - j)/2;
	
	returnValue = i;
	
	for (k = 0; k < i; k++)
	{
		outputString[k] = ' ';
	}
	
	memcpy (&outputString[i], theString, j);

	i += j;
	
	for (k = i; k < (ColumnsSupported); k++)
	{
		outputString[k] = ' ';
	}
	outputString[ColumnsSupported] = 0x00;

	TheLCD->setCursor (0, lineNum);
	TheLCD->println (outputString);
	
	return (returnValue);
}

// --------------------------------------------------------------
// Render the line in LCDStringTable specified by the passed-in offset
// to the display center-justified. Leading and trailing blanks are added as 
// necessary. Line number defaults to 0 (ie. the first line).
// Returns the display column (starting at 0) of the first character in the string.
int LCDItemBase::RenderFlashStringCenterJustified (unsigned char stringIndex,
					           unsigned short lineNum)
{
	// Buffer to store string once it is read from flash
	char buffer[ColumnsSupported+1];
	
	// Read the string into our buffer
	strcpy_P(buffer, (char*)pgm_read_word(&(LCDStringTable[LabelOffset]))); 

	return (RenderLineCenterJustified (buffer, 0));
}


// --------------------------------------------------------------
// Constructor
LCDItemBase::LCDItemBase (shen_LCD*      theLCD,
		          unsigned char  theLabelOffset, // Offset of label in LCDStringTable 
			  unsigned char  rowsSupported,
			  unsigned char  columnsSupported)
{
	
	IsInEditMode = IsInErrorMode = false;

	TheLCD = theLCD;
	LabelOffset = theLabelOffset;
	RowsSupported = rowsSupported;
	ColumnsSupported = columnsSupported;
}


// --------------------------------------------------------------
// Handle an Enter button press. This should only be called when we are
// in edit mode.
void LCDItemBase::OnEnter (void)
{
	// If we are in error mode, clear it and go back to edit mode
	if (IsInErrorMode == true)
	{
		IsInErrorMode = false;
		IsInEditMode = true;
		OnEditModeEntry();
	}
		
	// If the result of the edit is valid ...
	else 
	{
		if (IsValid())
		{
			// Go back to the non-editing state
			ApplyEdit();
		}
		else // bad edit - go to error mode
		{
			IsInEditMode = false;
			IsInErrorMode = true;
			DisplayError();
		}
	}
}


// --------------------------------------------------------------
// Handle a cancel button press. This should only be called when we are
// in edit mode.
void LCDItemBase::OnCancel (void)
{
	// If we are in error mode, clear it and go back to edit mode
	if (IsInErrorMode == true)
	{
		IsInErrorMode = false;
		IsInEditMode = true;
		OnEditModeEntry();
	}
	else
	{
		// Abort the current edit
		IsInEditMode = false;
		
		// And update the display accordingly
		DisplayCurrentValue();
	}
}

// --------------------------------------------------------------
// Called when this item becomes active on the display. This method
// displays the item title passed into the constructor on line 1 and
// Calls DisplayCurrentValue() to display the item's value on line 2.
void LCDItemBase::OnEntry (void)
{

	// Display the item title on line 1
	RenderFlashStringCenterJustified (LabelOffset);
	
	FetchData ();
	
	// Call derived classes' DisplayCurrentValue() to update line 2
	DisplayCurrentValue(); 

}

// --------------------------------------------------------------
// Called from the LCD controller when we have to go into edit mode. Change the
// display so that there is a visual indication that value is being edited.
void LCDItemBase::OnEditModeEntry (void)
{
	IsInEditMode = true;
	DisplayCurrentValue();
}


// --------------------------------------------------------------
// Called periodically by application to update the display (in case values
// have been changed by the terminal interface or other system components.
// In this default version, DisplayCurrentValue() is called as long as
// we are not in error mode or edit mode.
void LCDItemBase::Update (void)
{
	if (!IsInEditMode && !IsInErrorMode)
	{
		int isNewValue = FetchData();
        
		if (isNewValue)
			DisplayCurrentValue();
	}
}




