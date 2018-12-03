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

#include <LCDListItem.h>

#include <Arduino.h>

	
// --------------------------------------------------------------------------- 
// Called when edit mode has been terminated and the results of the edit
// have been determined to be valid. In most cases, this will be
// overriden in derived classes to take some action based on the results
// of the edit (like saving the new configuration). On its own, this
// method restores the non-edit display and sets IsInEditMode to false.
void LCDListItem::ApplyEdit (void)
{
	// Call base class. Derived classes should to more
	LCDItemBase::ApplyEdit ();
}
	
// --------------------------------------------------------------------------- 
// Display the current value of this LCD item on the second line of
// the display.
void LCDListItem::DisplayCurrentValue (void)
{
	// Temporary storage for our string. The "+1" provides space for the
	// null terminator.
	char outputString[ColumnsSupported+1];
	
	// String index
	int index = 0;
	
	// If we are in edit mode, display the value with square brackets around
	// it; otherwise display normally.
	if (IsInEditMode)
		outputString[index++] = '[';
	
	
	// Next, copy in the string corresponding to the current item
	strncpy (&outputString[index], TheList[CurrentItem], ColumnsSupported-index);
	
	// Just in case ...
	outputString[ColumnsSupported] = 0x00;
	
	// Make sure we haven't gone too long
	index = strlen (outputString);

	if ((IsInEditMode == true) && (index >= (ColumnsSupported - 1)))
	{
		index = ColumnsSupported - 1;
		outputString[index++] = ']';
	}
	else if (IsInEditMode == true)
	{
		outputString[index++] = ']';
	}
	else if ((IsInEditMode == false) &&(index >= (ColumnsSupported)))
		index = ColumnsSupported;
		
	outputString[index] = 0x00;
	
	RenderLineRightJustified (outputString);
}

// --------------------------------------------------------------------------- 
// Constructor
LCDListItem::LCDListItem (shen_LCD* theLCD, unsigned char theLabelOffset, char** theList, unsigned short numItems) :
   LCDItemBase (theLCD, theLabelOffset)
{
	TheList = theList;
	CurrentItem = 0;
	NumItems = numItems;
}
	
// --------------------------------------------------------------------------- 
// Handle an up button press. This should only be called when we are
// in edit mode and it moves to the next value in our list.
void LCDListItem::OnUp (void)
{
	if (++CurrentItem >= NumItems)
		CurrentItem = 0;
	DisplayCurrentValue();
}
	
// --------------------------------------------------------------------------- 
// Handle a down button press. This should only be called when we are
// in edit mode and it moves to the previous value in our list
void LCDListItem::OnDown (void)
{
	if (CurrentItem == 0)
		CurrentItem = NumItems - 1;
	else
		CurrentItem --;

	DisplayCurrentValue();	
}
		

