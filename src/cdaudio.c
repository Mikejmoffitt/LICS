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

#define DBCOL(c) VDP_setPaletteColor(0,c)

/* ----- Hardware access utils ----- */
volatile uint8_t *segacd_bios_addr;
extern uint32_t Sub_Start;
extern uint32_t Sub_End;
extern void Kos_Decomp(volatile uint8_t *src, volatile uint8_t *dst);
extern int16_t set_sr(int16_t new_sr);

static volatile uint16_t *reg_cpu = (volatile uint16_t *)SEGACD_REG_CPU;
static volatile uint16_t *reg_mem = (volatile uint16_t *)SEGACD_REG_MEM;
static volatile uint16_t *reg_hint = (volatile uint16_t *)SEGACD_REG_HINT;
static volatile uint16_t *reg_stopwatch = (volatile uint16_t *)SEGACD_REG_STOPWATCH;
static volatile int8_t *reg_comm_r = (volatile int8_t *)(SEGACD_REG_COMM + 1);
static volatile int8_t *reg_comm_w = (volatile uint8_t *)(SEGACD_REG_COMM);

// Write to writeonly COMM port; upper 8 bits
static inline void comm_write(int8_t msg)
{
	*reg_comm_w = msg;
}

// Read from readonly COMM port; lower 8 bits
static inline char comm_read(void)
{
	return *reg_comm_r;
}

// Block until sub-CPU bus is requested
static inline void bus_req(void)
{
	while (*reg_cpu & 0x0002 == 0)
	{
		*reg_cpu = (*reg_cpu & 0xFF00) | 0x02;
	}
}

// Block until sub-CPU acknowledges command
static int8_t wait_cmd_ack(void)
{
	int8_t ack = 0;

	while (!ack)
	{
		ack = comm_read();
	}

	return ack;
}

// Do a command, blocking until it has been accepted
static void wait_do_cmd(int8_t cmd)
{
	while (comm_read())
	{
		__asm__("nop");
	}
	comm_write(cmd);
}

static inline void wait_cpu_running(void)
{
	while ((*reg_cpu & 0x0001) == 0)
	{
		*reg_cpu = (*reg_cpu & 0xFF00) | 0x01;
	}
}

// Quick and dirty reimplementation of memcmp so we don't need to include any
// standard libs. This isn't a real implementation, this is really just
// returning zero if there is a mismatch.
static int32_t memcmp(const volatile void *s1, const volatile void *s2, int32_t n)
{
	while (n--)
	{
		if (*(uint8_t *)(s1 + n) != *(uint8_t *)(s2 + n))
		{
			return 1;
		}
	}
	return 0;
}

static inline void wait(int32_t iterations)
{
	while (iterations--)
	{
		__asm__("nop");
	}
}

/* ----- SegaCD initialization support functions ----- */

// Check for the CD BIOS
// Returns zero if a Sega CD was not detected. Returns location of Sega CD BIOS
// on success.
static volatile uint8_t *check_hardware(void)
{
	volatile uint8_t *bios;

	bios = (volatile uint8_t *)SCD_BIOSLOC_1;
	if (memcmp(bios + SCD_OFFSET, "SEGA", 4))
	{
		bios = (volatile uint8_t *)SCD_BIOSLOC_2;
		if (memcmp(bios + SCD_OFFSET, "SEGA", 4))
		{
			// Wondermega or X'Eye
			if (memcmp(bios + SCD_OFFSET, "WONDER", 6))
			{
				bios = (volatile uint8_t *)SCD_BIOSLOC_3;
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
	volatile uint8_t *loc2;
	loc2 = (volatile uint8_t *)0xA12001;

	*reg_mem = 0xFF00;
	*loc2 = 0x03;
	*loc2 = 0x02;
	*loc2 = 0x00;
}

// Reset Sub-CPU and req bus
static void subcpu_setup(void)
{
	bus_req();
}

static int32_t decompress_bios(void)
{
	// Configure for writing
	*reg_mem = 0x0002;
	
	// Clear first bank of program RAM for laseractive
	memset((void *)SEGACD_PROGRAM_ADDR, 0, SEGACD_PROGRAM_LEN);

	// Decompress BIOS here
	Kos_Decomp(segacd_bios_addr, (uint8_t *)SEGACD_PROGRAM_ADDR);

	// Copy program to program RAM
	memcpy((void *)SEGACD_PROGRAM_OFF, &Sub_Start, (int)&Sub_End - (int)&Sub_Start);
	if (memcmp((void *)SEGACD_PROGRAM_OFF, &Sub_Start, (int)&Sub_End - (int)&Sub_Start))
	{
		while(1)
		{
			DBCOL(0x00E);
		}
		return 0;
	}
	return 1;
}

void cdaudio_check_disc(void)
{
	int8_t ack;
	wait_do_cmd('C');
	DBCOL(0xEEE);
	ack = wait_cmd_ack();
	DBCOL(0x624);
	comm_write(0x00);
}

static void start_subcpu(void)
{
	comm_write(0x00); // Clear main comm port
	*reg_mem = 0x2A01;
	// *(volatile uint8_t *)(0xA12002) = 0x2A; // Write-protect up to 0x05400
	// *(volatile uint8_t *)(0xA12001) = 0x01; // Clear bus req
	
	DBCOL(0xEEE);

	// Wait for the sub-CPU to be running
	wait_cpu_running();

	DBCOL(0x080);

	// Enable level 2 interrupts on sub-CPU to poke it during vblank

	*reg_cpu = (*reg_cpu & 0xEFFF) | SEGACD_CPU_IEN_MASK;
	set_sr(0x2000);

	// Wait for the sub-CPU to set sub comm port to indicate it is
	// alive and working correctly
	while (comm_read() != 'I')
	{
		// Show cool junk while we wait
		static int32_t timeout = 0;
		DBCOL(timeout);
		timeout++;

		if (timeout > 2000000)
		{
			return;
		}
	}
	DBCOL(0x048);

	// Wait for sub-CPU being ready to receive commands
	while (comm_read() != 0x00)
	{
		__asm__("nop");
	}
	DBCOL(0xE62);

	wait(100);
}

int32_t cdaudio_init(void)
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
	cdaudio_check_disc();
	DBCOL(0x68E);
	return 1;
}

void cdaudio_play_once(uint8_t trk)
{
	int8_t ack;
	*(volatile uint16_t *)(SEGACD_TRACK_W) = (uint16_t)trk;
	*(volatile uint8_t *)(SEGACD_PMODE_W) = 0x00;
	wait_do_cmd('P');
	ack = wait_cmd_ack();
	comm_write(0x00);
}

void cdaudio_play_loop(uint8_t trk)
{
	
}

void cdaudio_stop(void)
{
	int8_t ack;
	wait_do_cmd('S');
	ack = wait_cmd_ack();
	comm_write(0x00);
}

void cdaudio_pause(void)
{

}

inline int32_t cdaudio_is_active(void)
{
	return (segacd_bios_addr) ? 1 : 0;
}
