// Sega CD Mode 1 support code for SGDK
// Michael Moffitt (https://github.com/mikejmoffitt)
// Adapted from Chilly Willy's Mode 1 CD player

#include "cdaudio.h"
#include <genesis.h>
#include <stdlib.h>

#define SCD_OFFSET 0x6D

#define SCD_BIOSLOC_1 0x415800
#define SCD_BIOSLOC_2 0x416000
#define SCD_BIOSLOC_3 0x41AD00

extern unsigned int Sub_Start;
extern unsigned int Sub_End;
extern void Kos_Decomp(volatile unsigned char *src, volatile unsigned char *dst);

volatile unsigned char *segacd_bios_addr;

static char ack;

static char wait_cmd_ack(void)
{
	char ack = 0;

	while (!ack)
	{
		ack = *(volatile unsigned char *)(0xA1200F);
	}
	return ack;
}

static void do_cmd_blocking(char cmd)
{
	volatile unsigned char *reg_r;
	volatile unsigned char *reg_w;
	reg_r = (volatile unsigned char *)0xA1200F;
	reg_w = (volatile unsigned char *)0xA1200E;
	while (*reg_r)
	{
		*reg_w = cmd;
	}
}

// Quick and dirty reimplementation of memcmp so we don't need to include any
// standard libs. This isn't a real implementation, this is really just
// returning zero if there is a mismatch.
static int memcmp(const volatile void *s1, const volatile void *s2, int n)
{
	while (n--)
	{
		if (*(unsigned char *)(s1 + n) != *(unsigned char *)(s2 + n))
		{
			return 1;
		}
	}
	return 0;
}

// Check for the CD BIOS
// Returns zero if a Sega CD was not detected. Returns location of Sega CD BIOS
// on success.
static volatile unsigned char *check_hardware(void)
{
	volatile unsigned char *bios;

	bios = (volatile unsigned char *)SCD_BIOSLOC_1;
	if (memcmp(bios + SCD_OFFSET, "SEGA", 4))
	{
		bios = (volatile unsigned char *)SCD_BIOSLOC_2;
		if (memcmp(bios + SCD_OFFSET, "SEGA", 4))
		{
			// Wondermega or X'Eye
			if (memcmp(bios + SCD_OFFSET, "WONDER", 6))
			{
				bios = (volatile unsigned char *)SCD_BIOSLOC_3;
				// Laseractive check
				if (memcmp(bios + SCD_OFFSET, "SEGA", 4))
				{
					return 0;
				}
			}
		}
	}
	return bios;
}

// Needed to clear Laseractive internal state, apparently
static void reset_gate_array(void)
{
	volatile unsigned short *loc1;
	volatile unsigned char *loc2;
	loc1 = (volatile unsigned short *)0xA12002;
	loc2 = (volatile unsigned char *)0xA12001;

	*loc1 = 0xFF00;
	*loc2 = 0x03;
	*loc2 = 0x02;
	*loc2 = 0x00;
}

// Reset Sub-CPU and req bus
static void subcpu_setup(void)
{
	*(volatile unsigned char *)(0xA12001) = 0x02;

	// Reset Sub-CPU and request the bus
	while (!(*(volatile unsigned char *)(0xA12001) & 2))
	{
		*(volatile unsigned char *)(0xA12001) = 0x02;
	}
}

static int decompress_bios(void)
{
	int i;

	// Configure for writing
	*(volatile unsigned short *)(0xA12002) = 0x0002;
	
	// Clear first bank of program RAM for laseractive
	memset((void *)0x420000, 0, 0x20000);

	// Decompress BIOS here
	Kos_Decomp(segacd_bios_addr, (unsigned char *)0x420000);

	// Copy program to program RAM
	memcpy((void *)0x426000, &Sub_Start, (int)&Sub_End - (int)&Sub_Start);
	if (memcmp((void *)0x426000, &Sub_Start, (int)&Sub_End - (int)&Sub_Start))
	{
		return 0;
	}
	return 1;
}

static void start_subcpu(void)
{
	*(volatile unsigned char *)(0xA1200E) = 0x00; // Clear main comm port
	*(volatile unsigned char *)(0xA12002) = 0x2A; // Write-protect up to 0x05400
	*(volatile unsigned char *)(0xA12001) = 0x01; // Clear bus req

	// Wait for the sub-CPU to be running
	while (!(*(volatile unsigned char *)(0xA12001) & 1))
	{
		*(volatile unsigned char *)(0xA12001) = 0x01;
	}

	// Enable level 2 interrupts on sub-CPU to poke it during vblank
	*(volatile unsigned char *)(0xA12000) = 0x8000;

	// Wait for the sub-CPU to set sub comm port to indicate it is
	// alive and working correctly
	while (*(volatile char *)(0xA1200F) != 'I')
	{
		// Show cool junk while we wait
		VDP_setPaletteColor(0, GET_HVCOUNTER);
		static int timeout = 0;
		timeout++;

		if (timeout > 1000000)
		{
			return;
		}
	}

	while (*(volatile char *)(0xA1200F) != 0x00)
	{

		VDP_setPaletteColor(0,0x0EE);
	}
}

int cdaudio_init(void)
{
	segacd_bios_addr = check_hardware();
	if (!segacd_bios_addr)
	{
		return 0;
	}
	reset_gate_array();
	subcpu_setup();
	if (!decompress_bios())
	{
		segacd_bios_addr = 0;
		return 0;
	}
	start_subcpu();
	return 1;
}

void cdaudio_play_once(unsigned char trk)
{
	if (!segacd_bios_addr)
	{
		return;
	}
	*(volatile unsigned short *)(0xA12010) = (unsigned short)trk;
	*(volatile unsigned char *)(0xA12012) = 0x00;
	do_cmd_blocking('P');
	ack = wait_cmd_ack();
	*(volatile unsigned char *)(0xA1200E) = 0x00;
}

void cdaudio_play_loop(unsigned char trk)
{

}

void cdaudio_stop(void)
{

}

void cdaudio_pause(void)
{

}

inline int cdaudio_is_active(void)
{
	return (segacd_bios_addr) ? 1 : 0;
}
