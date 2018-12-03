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

#include <LCDApp.h>
#include <EEPROM.h>

#include <LCDBacklightColourItem.h>
	
// --------------------------------------------------------------------------- 
//Set our default configuration in case EEPROM is un-initialized or
// corrupted.
void LCDApplication::SetDefaultConfiguration (void)
{
	  LCDAppConfig.VersionNumber = 1;
	  LCDAppConfig.BacklightColour = WHITE;
	  LCDAppConfig.LowPowerMode = false; 
	  
	  LCDAppConfig.Spare = 0;
}
	
// --------------------------------------------------------------------------- 
// Constructor
LCDApplication::LCDApplication (
	unsigned short configOffset,
	unsigned short supportedProfiles,
	ProfileBase**  programList,
	shen_LCD*      theLCD
	) : ApplicationBase (configOffset, 
                             (char*)&LCDAppConfig,
                             sizeof(LCDAppConfig),
                             supportedProfiles, 
                             programList)
{
	  // Read application configuration settings from EEPROM  
	  if (ReadConfiguration() == -1)
	  {
	  	  // Something went wrong - revert to defaults
	  	  SetDefaultConfiguration();
	  	  WriteConfiguration();
	  }
	  	  	  
	  // Save a pointer to the LCD display
	  TheLCD = theLCD;
}

// --------------------------------------------------------------------------- 
void LCDApplication::SetBacklightColour (char theColour)
{
	LCDAppConfig.BacklightColour = theColour;
	WriteConfiguration();
}

	
// --------------------------------------------------------------------------- 
char LCDApplication::GetBacklightColour (void)
{
	return (LCDAppConfig.BacklightColour);
}

// --------------------------------------------------------------------------- 
// Set low power mode
void LCDApplication::SetLowPowerMode (void)
{
	// Change configuration and save it to EEPROM
	LCDAppConfig.LowPowerMode = true;
        WriteConfiguration();
}

// --------------------------------------------------------------------------- 
// Clear low power mode
void LCDApplication::SetNormalPowerMode (void)
{
	// Change configuration and save it to EEPROM
	LCDAppConfig.LowPowerMode = false;
        WriteConfiguration();
}

// --------------------------------------------------------------------------- 
// Run the current profile. If it is already running, it is stoppped and
// restarted
void LCDApplication::Run (void)
{
	ApplicationBase::Run();
	
	// If we're in low power mode, turn off backlight while running.
	if (IsInLowPowerMode())
	{
		TheLCD->setBacklight(OFF);
	}
}
	
// --------------------------------------------------------------------------- 
// Stop running the current profile. If it's not running, nothing is
// done.
void LCDApplication::Stop (void)
{
	ApplicationBase::Stop();
	
	// If we're in low power mode, turn the backlight back on
	if (IsInLowPowerMode())
	{
		TheLCD->setBacklight(LCDAppConfig.BacklightColour);
	}
}
	
// --------------------------------------------------------------------------- 
// Called by main loop to update us
void LCDApplication::Update (void)
{
	// If we're running, update the current program
	if (IsRunning)
	{
		IsRunning = ProfileList[CurrentProfile]->Update();
	}
	
	// If running has now been terminated, restore the backlight if
	// we're in low power mode.
	if (!IsRunning && LCDAppConfig.LowPowerMode)
	{
		if (LCDAppConfig.BacklightColour != OFF)
		{
			TheLCD->setBacklight (LCDAppConfig.BacklightColour);
		}
	}
}
