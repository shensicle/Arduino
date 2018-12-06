#ifndef _PERSONALITY_H
#define _PERSONALITY_H

// The definition of the personality structure for the current sketch. To change
// personality for a different sketch, change this structure and leave the class
// defined in this file alone.
#include <PersonalityDefs.h>

class PersonalityClass
{
	private:
		
		// Return the one's complement checksum of the personality structure
		unsigned char CalculateChecksum (personality_t* thePersonality);
	
	public:
		// Constructor - allocate EEPROM space
		PersonalityClass (void);
		
		// Write personality information to EEPROM, adding a checksum
		void Write (personality_t* thePersonality);
		
		// Read personality information from EEPROM and validate the checksum
		// Returns true if personality is valid and false otherwise
		bool Read(personality_t* thePersonality);
};

#endif
