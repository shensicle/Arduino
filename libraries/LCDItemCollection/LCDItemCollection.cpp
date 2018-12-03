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

#include <LCDItemCollection.h>

#include <Arduino.h>

#define NO_ACTIVE_ITEM -1
	
// --------------------------------------------------------------
// Called when edit mode has been terminated and the results of the edit
// have been determined to be valid.
void LCDItemCollection::LCDItemCollection::ApplyEdit (void)
{
}
	
// --------------------------------------------------------------
// Display the current value of this LCD item on the second line of
// the display.
void LCDItemCollection::DisplayCurrentValue (void)
{
	RenderLineRightJustified ("Enter for more");
}
	
// --------------------------------------------------------------
// Constructor
LCDItemCollection::LCDItemCollection (
	           shen_LCD*      theLCD,
		   LCDItemBase**   theCollection,
	           unsigned short itemsInCollection,
		   unsigned char  theLabelOffset, // Offset of label in LCDStringTable 
		   bool		  displayOurself,
	           unsigned short rowsSupported, 
	           unsigned short columnsSupported)
: LCDItemBase (theLCD, theLabelOffset, rowsSupported, columnsSupported)
{
	TheCollection = theCollection;
	ItemsInCollection = itemsInCollection;
	DisplayOurself = displayOurself;

	if (displayOurself)
		ActiveItem = NO_ACTIVE_ITEM;
	else
		ActiveItem = 0;
}
	
// --------------------------------------------------------------
// Handle a left button press. This should only be called when we are
// in edit mode.
void LCDItemCollection::OnLeft (void)
{
	// If we have an item that is being edited, send the key there
	if (ActiveItem != NO_ACTIVE_ITEM)
	{
		if (TheCollection[ActiveItem]->IsBeingEdited())
		{
			TheCollection[ActiveItem]->OnLeft();
		}
		else
		{
			// If we're at the first item this would be the
			// time to display ourselves if that is enabled.
			if ((DisplayOurself == true) && (ActiveItem == 0))
			{
				ActiveItem = NO_ACTIVE_ITEM;
				IsInEditMode = false;
				DisplayCurrentValue();
			}
			else
			{
				// We have an active item that is not being
				// edited. In this case, the left key moves us
				// to the previous item and makes it active.
				ActiveItem --;
				if (ActiveItem < 0)
					ActiveItem = ItemsInCollection - 1;
			
				TheCollection[ActiveItem]->OnEntry();
			}
		}
	}
	
	// The else condition should never happen!
}
	
// --------------------------------------------------------------
// Handle a right button press. This should only be called when we are
// in edit mode.
void LCDItemCollection::OnRight (void)
{
	// If we have an item that is being edited, send the key there
	if (ActiveItem != NO_ACTIVE_ITEM)
	{
		if (TheCollection[ActiveItem]->IsBeingEdited())
		{
			TheCollection[ActiveItem]->OnRight();
		}
		else
		{
			// If we're at the last item this would be the
			// time to display ourselves if that is enabled.
			if ((DisplayOurself == true) && (ActiveItem ==  (ItemsInCollection-1)))
			{
				ActiveItem = NO_ACTIVE_ITEM;
				IsInEditMode = false;
				DisplayCurrentValue();
			}
			else
			{
				// We have an active item that is not being
				// edited. In this case, the right key moves us
				// to the next item and makes it active.
				ActiveItem ++;
				if (ActiveItem >= ItemsInCollection)
					ActiveItem = 0;
			
				TheCollection[ActiveItem]->OnEntry();
			}
		}
	}
	
	// The else condition should never happen!
}

// --------------------------------------------------------------
// Handle an up button press. This should only be called when we are
// in edit mode.
void LCDItemCollection::OnUp (void)
{
	// If we have an item that is being edited, send the key there
	if (ActiveItem != NO_ACTIVE_ITEM)
	{
		if (TheCollection[ActiveItem]->IsBeingEdited())
		{
			TheCollection[ActiveItem]->OnUp();
		}
	}

	// Otherwise, ignore the key
}
	
// --------------------------------------------------------------
// Handle a down button press. This should only be called when we are
// in edit mode.
void LCDItemCollection::OnDown (void)
{
	// If we have an item that is being edited, send the key there
	if (ActiveItem != NO_ACTIVE_ITEM)
	{
		if (TheCollection[ActiveItem]->IsBeingEdited())
		{
			TheCollection[ActiveItem]->OnDown();
		}
	}
	
	// Otherwise, ignore the key
}
	
// --------------------------------------------------------------
// Handle an Enter button press. This should only be called when we are
// in edit mode.
void LCDItemCollection::OnEnter (void)
{
	// It we have an active item, pass this key to it.	
	if (ActiveItem != NO_ACTIVE_ITEM)
	{
		if (TheCollection[ActiveItem]->IsBeingEdited())
		{
			TheCollection[ActiveItem]->OnEnter();
			
			// If our active item is no longer being edited, neither
			// are we.
			if (TheCollection[ActiveItem]->IsBeingEdited() == false)
				IsInEditMode = false;
		}
		else // Not currently being edited
		{
			TheCollection[ActiveItem]->OnEditModeEntry();
		}
	}
	
	// The else case should never happen.
}
	
// --------------------------------------------------------------
// Handle a cancel button press. This should only be called when we are
// in edit mode.
void LCDItemCollection::OnCancel (void)
{
	// It we have an active item that is being edited, pass this key to it.	
	if (ActiveItem != NO_ACTIVE_ITEM)
	{
		if (TheCollection[ActiveItem]->IsBeingEdited())
		{
			TheCollection[ActiveItem]->OnEnter();
		}
		else
		{
			// In this case, we have an active item that is not
			// being edited. If we're set to the mode where we
			// have to display ourself as part of the rotation, do so
			if (DisplayOurself)
			{
				ActiveItem = NO_ACTIVE_ITEM;
				IsInEditMode = false;
				DisplayCurrentValue();
			}	
		}			
	}

	// Otherwise, ignore the key
}
		

// --------------------------------------------------------------
// Must be defined in derived classes. Should change the display so that
// there is a visual indication that value is being edited.
void LCDItemCollection::OnEditModeEntry (void)
{
	
	// If there are any items in our collection, display the first one
	if (ItemsInCollection)
	{
		if (ActiveItem == NO_ACTIVE_ITEM)
		{
			ActiveItem = 0;
			TheCollection[ActiveItem]->OnEntry();
		}
		else
		{
			if (TheCollection[ActiveItem]->IsBeingEdited())
				TheCollection[ActiveItem]->OnEnter();
			else
				TheCollection[ActiveItem]->OnEditModeEntry();
		}
		IsInEditMode = true;
	}
}

// --------------------------------------------------------------
// Called when this item becomes active on the display. This method
// displays the item title passed into the constructor on line 1 and
// Calls DisplayCurrentValue() to display the item's value on line 2
void LCDItemCollection::OnEntry (void)
{
	if (ItemsInCollection)
	{
		ActiveItem = 0;
		TheCollection[ActiveItem]->OnEntry();
	}
}
	
// --------------------------------------------------------------
// Called periodically by application to update the display (in case values
// have been changed by the terminal interface or other system components.
// In this case, we call the update function of the active item, if there
// is one.
void LCDItemCollection::Update (void)
{
	if (ActiveItem != NO_ACTIVE_ITEM)
	{
		TheCollection[ActiveItem]->Update();
	}
}


