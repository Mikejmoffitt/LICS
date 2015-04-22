#include "serial.h"

void serial_init(u8 num, u8 bps, u8 int_enable)
{
	u8 *port = (u8 *)VDP_SCTRL1;
	// select port using port_num (0 - 2)
	port += (num * VDP_S_OFFSET);

	*port = bps | (int_enable << 3);
}

void serial_puts(u8 num, char *c)
{
	while (*c)
	{
		serial_putc(num,*(c++));
	}
}

void serial_putc(u8 num, char c)
{
	u8 *port = (u8 *)VDP_TXDATA1;
	port += (num * VDP_S_OFFSET);
	*port = c;
}

char serial_getc(u8 num)
{
	u8 *port = (u8 *)VDP_RXDATA1;
	port += (num * VDP_S_OFFSET);
	return *port;
}

u8 serial_get_error(u8 num)
{
	u8 *port = (u8 *)VDP_SCTRL1;
	port += (num * VDP_S_OFFSET);
	return (*port >> 2) & 0x01;
}

u8 serial_get_ready(u8 num)
{	
	u8 *port = (u8 *)VDP_SCTRL1;
	port += (num * VDP_S_OFFSET);
	return (*port >> 1) & 0x01;
}

u8 serial_get_full(u8 num)
{	
	u8 *port = (u8 *)VDP_SCTRL1;
	port += (num * VDP_S_OFFSET);
	return (*port) & 0x01;
}

/* vim: set noet: */
