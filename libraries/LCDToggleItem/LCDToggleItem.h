#ifndef _LCDTOGGLEITEM_H
#define _LCDTOGGLEITEM_H

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

// This class implements and LCD control whose state is toggled between 2 values
// by pressing the Enter key while the item is active. There is no edit mode and
// the change is immediate.
class LCDToggleItem : public LCDItemBase
{
protected:

	// Pointer to an array of strings containing all of our possible
	// list items.
	char** TheList;
	
	// Current position within the list
	unsigned short CurrentItem;
	
	// The number of items on the list. Leaving this as a variable in case
	// we decide to support more than 2 states in this class later.
	unsigned short NumItems;
	
	// Returns a flag which, when set, indicates that the results of an edit
	// are valid. Since we are processing a known list, our edits are always
	// valid.
	virtual bool IsValid (void) { return (true); }
	
	// Called when edit mode has been terminated and the results of the edit
	// have been determined to be valid. Since we don't stay in edit mode, this
	// is called directly by OnEditModeEntry(). Derived classes need to define
	// this method to update the rest of the system.
	virtual void ApplyEdit (void) = 0;
	
	// Display the current value of this LCD item on the second line of
	// the display.
	virtual void DisplayCurrentValue (void);
	
public:
	// Constructor
	LCDToggleItem (shen_LCD*      theLCD, 
		       unsigned char  theLabelOffset, // Offset of label in LCDStringTable 
		       char**         theList);
	
	// Handle button presses in edit mode. As this class never enters edit mode,
	// these are just placeholders.
	virtual void OnLeft (void) {};
	virtual void OnRight (void) {};
	virtual void OnUp (void) {};
	virtual void OnDown (void) {};
	
	// Called from the LCD controller when we have to go into edit mode. In the case
	// of this class, we don't stay in edit mode so this method changes the toggle and
	// updates other system objects directly.
	virtual void OnEditModeEntry (void);
		
};

#endif

