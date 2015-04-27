#include "mpad.h"

// Read from a Sega Mega Drive controller port.
// Returns a bitfield - SCBARLDU (EX: bit0, LSB, is 0 when "up" is held);
unsigned char pad_read(unsigned char pl)
{
	// Controller data register, offset by player number
	volatile unsigned char *ctrlr = (unsigned char *)VDP_GAMEPAD + (2 * pl);
	
	// Set bit 6 to read B,C,etc
	*ctrlr |= (0x40); 
	*ctrlr |= (0x40); 
	
	// Grab U,D,L,R and B,C
	volatile unsigned char ret = (*ctrlr & 0xF) | ((*ctrlr & 0x30) << 1); 
	
	// Clear bit 6 to get A and Start
	*ctrlr = *ctrlr & ~(0x40); 
	*ctrlr = *ctrlr & ~(0x40); 
	
	ret |= (*ctrlr & 0x10) | ((*ctrlr & 0x20) << 2); // Grab A and Start
	return ret;
}

