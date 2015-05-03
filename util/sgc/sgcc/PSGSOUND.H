#define PSG_MEM   0xc00011

/* PSG Registers */

#define TONE1   0x80
#define VOL1    0x90
#define TONE2   0xa0
#define VOL2    0xb0
#define TONE3   0xc0
#define VOL3    0xd0
#define NOISE   0xe0
#define VOLN    0xf0

/* Noise Channel Register Bits */

#define FB_HI   0x04
#define FB_LO   0x00

#define CLK_DIV_2   0x00
#define CLK_DIV_4   0x01
#define CLK_DIV_8   0x02
#define FOLLOW_T3   0x03

/* low level PSG routines */

void set_noise(parm);
void set_freq(channel,freq);
void set_vol(channel,vol);

/* a medium level PSG interpreter built on top of the lower level routines */

/* Frequencies require 10 bits, so we have 6 leftover bits to define "opcodes" */
#define OPCODE_MASK    0xfc00
#define OPERAND_MASK   0x03ff

#define END_SOUND   (0<<10)

#define SET_TONE1   (1<<10)
#define SET_TONE2   (2<<10)
#define SET_TONE3   (3<<10)
#define SET_NOISE   (4<<10)
#define SET_TONEX   (5<<10)
#define SET_VOL1    (6<<10)
#define SET_VOL2    (7<<10)
#define SET_VOL3    (8<<10)
#define SET_VOLN    (9<<10)
#define SET_VOLX    (10<<10)

#define DELAY       (11<<10) /* new, replaces the following */

#define SET_DELAY   (12<<10) /* used with the next opcode */
#define WAIT_DELAY  (13<<10) /* these two opcodes are obsolete */

#define VOICES   4

void init_PSG();
void interpret_sound(index);
void generate_sound();
void start_sound(voice,sound);
int sound_finished(voice);
