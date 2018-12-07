#include <Personality.h>
#include <EEPROM.h>

// ----------------------------------------------------------------------
// Return the one's complement checksum of the personality structure
unsigned char PersonalityClass::CalculateChecksum (personality_t* thePersonality)
{
	unsigned char* personalityBytes = (unsigned char*)thePersonality;
	
	unsigned char returnValue = 0;
	
	for (int i = 0; i < sizeof(*thePersonality); i++)
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
	EEPROM.begin (sizeof(personality_t)+1);
}
		

// ----------------------------------------------------------------------
// Write personality information to EEPROM, adding a checksum
void PersonalityClass::Write (personality_t* thePersonality)
{
	unsigned writeAddr = 0;
	
	unsigned char checksum = CalculateChecksum (thePersonality);
	
	// Write the data
	EEPROM.put (writeAddr, *thePersonality);
	
	// Write the checksum
	writeAddr += sizeof (*thePersonality);
	EEPROM.put (writeAddr, checksum);
	
	EEPROM.commit();
}
		

// ----------------------------------------------------------------------
// Read personality information from EEPROM and validate the checksum
// Returns true if personality is valid and false otherwise
bool PersonalityClass::Read(personality_t* thePersonality)
{
	bool returnValue = true;
	unsigned readAddr = 0;

	// Zero out personality structure. Helps to null-terminate strings
	memset (thePersonality, 0, sizeof (*thePersonality));
	
	// Read the data
	EEPROM.get (readAddr, *thePersonality);
	
	// Calculate the checksum of this data
	unsigned char checksum  = CalculateChecksum (thePersonality);
	
	// Read the stored checksum
	readAddr += sizeof (personality_t);
	unsigned char storedChecksum;
	EEPROM.get (readAddr, storedChecksum);
	

	if (checksum != storedChecksum)
		returnValue = false;
	
	return (returnValue);
}

