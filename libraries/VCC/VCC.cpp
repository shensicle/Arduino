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

#include <VCC.h>
#include <Arduino.h>

// --------------------------------------------------------------                           
// Method to start an acquisition of Vcc
void Vcc::StartAcquisition (void)
{
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);

  // Wait for Vref to settle 
  ADCSRA |= _BV(ADSC); // Convert
}
        
// --------------------------------------------------------------
// Constructor
Vcc::Vcc (unsigned long updateInterval)
{ 
	UpdateInterval = updateInterval; 
	LastVccValue = 0; 
	StartAcquisition(); 
}
	
// --------------------------------------------------------------
// Called periodically to update LastVCCValue
void Vcc::Update (void)
{
  long result;
  
  if (! bit_is_set(ADCSRA,ADSC))
  {
  	  result = ADCL;
  	  result |= ADCH<<8;
  	  result = 1126400L / result; // Back-calculate AVcc in mV
  	  LastVccValue = result;  // a nice atomic operation
  	  StartAcquisition();
  }
}
	 

