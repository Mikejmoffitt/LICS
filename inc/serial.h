#ifndef SERIAL_H
#define SERIAL_H

#include <genesis.h>

#define BAUD_4800 0x00
#define BAUD_2400 0x40
#define BAUD_1200 0x80
#define BAUD_300 0xC0

#define VDP_GAMEPAD 0xA10003
#define VDP_TXDATA1 0xA1000F
#define VDP_RXDATA1 0xA10011
#define VDP_SCTRL1 0xA10013

#define VDP_PORT_1 0
#define VDP_PORT_2 1
#define VDP_PORT_EXT 2
#define VDP_S_OFFSET 6

void serial_init(u8 num, u8 bps, u8 int_enable);
void serial_puts(u8 num, char *c);
void serial_putc(u8 num, char c);
char serial_getc(u8 num);

u8 serial_get_error(u8 num);
u8 serial_get_ready(u8 num);
u8 serial_get_full(u8 num);

#endif

