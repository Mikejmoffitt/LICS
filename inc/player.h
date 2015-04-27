#ifndef PLAYER_H
#define PLAYER_H

#include <genesis.h>

/* Lyle sprite table:

00 - stand
01 - walk1
02 - walk2
03 - walk3
04 - jump
05 - lift
06 - hurt
07 - jumpthrow
08 - stand_l
09 - walk1_l
0A - walk2_l
0B - walk3_l
0C - jump_l
0D - dazed
0E - look up
0F - die1
10 - die2
11 - splat1
12 - splat2
13 - getup / splat3
14 - crush1
15 - crush2
16 - throw
17 - kick
*/

/* Lyle Palette


*/

#define LYLE_3x3_CUTOFF 0x14
// Put Lyle's sprite #n at addr dest
void player_dma(u16 num, u16 dest);

#endif
