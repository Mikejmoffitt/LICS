// system.h
// A few basic routines for setting up the system and managing vblank timing
#ifndef GLOBALSYS_H
#define GLOBALSYS_H

#include <genesis.h>

#define SYSTEM_PLANE_W 64
#define SYSTEM_PLANE_H 32

// This just counts up forever. It is often used for animations.
extern u16 system_osc;

void system_init(void);
void system_wait_v(void);

#endif
