#include "mpad.h"

// Read from a Sega Mega Drive controller port.
u8 pad_read(u16 pl)
{
	// Controller data register, offset by player number
	vu8 *ctrlr = (unsigned char *)VDP_GAMEPAD + (pl << 1);
	
	// Set bit 6 to read B,C,etc
	*ctrlr |= (0x40); 
	
	// Grab U,D,L,R and B,C
	vu8 ret = (*ctrlr & 0xF) | ((*ctrlr & 0x30) << 1); 
	
	// Clear bit 6 to get A and Start
	*ctrlr = *ctrlr & ~(0x40); 
	
	ret |= (*ctrlr & 0x10) | ((*ctrlr & 0x20) << 2); // Grab A and Start
	return ~ret;
}

