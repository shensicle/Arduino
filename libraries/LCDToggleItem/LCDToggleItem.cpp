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

#include <LCDToggleItem.h>

#include <Arduino.h>

		
// --------------------------------------------------------------------------- 
// Display the current value of this LCD item on the second line of
// the display.
void LCDToggleItem::DisplayCurrentValue (void)
{
	// Temporary storage for our string. The "+1" provides space for the
	// null terminator.
	char outputString[ColumnsSupported+1];
	
	// String index
	int index = 0;
		
	// Copy in the string corresponding to the current item
	strncpy (&outputString[index], TheList[CurrentItem], ColumnsSupported-index);
	
	// Make sure we haven't gone too long
	index = strlen (outputString);

	if (index > ColumnsSupported)
		index = ColumnsSupported;
		
	outputString[index] = 0x00;
	
	RenderLineRightJustified (outputString);
}

// --------------------------------------------------------------------------- 
// Constructor
LCDToggleItem::LCDToggleItem (shen_LCD* theLCD, unsigned char theLabelOffset, char** theList) :
   LCDItemBase (theLCD, theLabelOffset)
{
	TheList = theList;
	CurrentItem = 0;
	NumItems = 2;
}
	
// --------------------------------------------------------------------------- 
void LCDToggleItem::OnEditModeEntry (void)
{
	// Note that we don't set IsInEditMode to true here because we want
	// to apply the changes immediately.
	
	// First, toggle to the other item
	if (++CurrentItem >= NumItems)
		CurrentItem = 0;

	// Next, update the display value
	DisplayCurrentValue();
	
	// Finally, apply our instant edit to update the rest of the system.
	ApplyEdit();
	
}

