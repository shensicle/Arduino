#ifndef _PERSONALITY_H
#define _PERSONALITY_H


class PersonalityClass
{
	private:
		// The size of the personality data we're storing
		int PersonalitySize;
		
	
	public:
		// Constructor - allocate EEPROM space
		PersonalityClass (int personalitySize);
		
		// Write personality information to EEPROM, adding a checksum
		void Write (char* thePersonality);
		
		// Read personality information from EEPROM and validate the checksum
		// Returns true if personality is valid and false otherwise
		bool Read(char* thePersonality);
};

#endif
