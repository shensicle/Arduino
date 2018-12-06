#include <Personality.h>
#include <EEPROM.h>

// ----------------------------------------------------------------------
// Return the one's complement checksum of the personality structure
unsigned char PersonalityClass::CalculateChecksum (personality_t* thePersonality)
{
	unsigned char* personalityBytes = (unsigned char*)thePersonality;
	
	unsigned char returnValue = 0;
	
	for (int i = 0; i < sizeof(personality_t); i++)
	{
		returnValue += *personalityBytes++;
	}
	
	return (returnValue);
}

// ----------------------------------------------------------------------
// Constructor - allocate EEPROM space
PersonalityClass::PersonalityClass (void)
{
	// Add 1 for the checksum
	EEPROM.begin (sizeof(personality_t+1);
}
		

// ----------------------------------------------------------------------
// Write personality information to EEPROM, adding a checksum
void PersonalityClass::Write (char* thePersonality)
{
}
		

// ----------------------------------------------------------------------
// Read personality information from EEPROM and validate the checksum
// Returns true if personality is valid and false otherwise
bool PersonalityClass::Read(char* thePersonality)
{
}

