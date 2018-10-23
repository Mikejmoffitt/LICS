#ifndef template_H
#define template_H

#include <genesis.h>
#include "enemy.h"

// Structure defining specific object type
typedef struct en_template en_template;
struct en_template
{	
	// Header present in each drawable and processable enemy type
	en_header head;
};

// Initialize enemy at address e
void en_init_template(en_template *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_template(void);

#endif
