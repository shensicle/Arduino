/*
Copyright � 2012 Scott Henwood/shensicle photographic. All Rights Reserved.

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

#include <LCDProfileStartupDelayItem.h>

// ---------------------------------------------------------------------------
// Called when edit mode has been terminated and the results of the edit
// have been determined to be valid. Adjust the profile we're representing
// accordingly.
void LCDProfileStartupDelayItem::ApplyEdit (void)
{
	LCDTimeItem::ApplyEdit();
	TheProfile->SetStartupDelay (TheValue);
}

// ---------------------------------------------------------------------------
// Fetch data that this object will display on the second line and load
// object attributes accordingly.
int LCDProfileStartupDelayItem::FetchData (void)
{
    int returnValue = 0;
    
    int oldValue = TheValue;
	TheValue = TheProfile->GetStartupDelay();
    
    if (oldValue != TheValue)
        returnValue = 1;
    
    return (returnValue);
}



