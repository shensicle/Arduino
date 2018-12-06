#include <Personality.h>
#include <EEPROM.h>

// Constructor - allocate EEPROM space
PersonalityClass::PersonalityClass (int personalitySize)
{
	PersonalitySize = personalitySize;

	// Add 1 for the checksum
	EEPROM.begin (PersonalitySize+1);
}
		


// Write personality information to EEPROM, adding a checksum
void PersonalityClass::Write (char* thePersonality)
{
}
		
// Read personality information from EEPROM and validate the checksum
// Returns true if personality is valid and false otherwise
bool PersonalityClass::Read(char* thePersonality)
{
}

