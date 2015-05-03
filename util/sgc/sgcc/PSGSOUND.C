#include "genesis.h"

#include "psgsound.h"

void set_noise(parm)
unsigned char parm;
{
   uchar *psg = (uchar *) PSG_MEM;
   *psg = NOISE+parm;
}

void set_freq(channel,freq)
unsigned int channel;
unsigned int freq;
{
   uchar *psg = (uchar *) PSG_MEM;
   switch (channel) {
      case 1:
         *psg = TONE1+(freq & 0x0f);
         *psg = freq >> 4;
      break;
      case 2:
         *psg = TONE2+(freq & 0x0f);
         *psg = freq >> 4;
      break;
      case 3:
         *psg = TONE3+(freq & 0x0f);
         *psg = freq >> 4;
      break;
   }
}

void set_vol(channel,vol)
unsigned int channel;
unsigned int vol;
{
   uchar *psg = (uchar *) PSG_MEM;
   vol = vol & 0x0f;
   vol = 0x0f - vol;
   switch (channel) {
      case 1:
         *psg = VOL1+vol;
      break;
      case 2:
         *psg = VOL2+vol;
      break;
      case 3:
         *psg = VOL3+vol;
      break;
      case 4:
         *psg = VOLN+vol;
      break;
   }
}

uint freq[VOICES];
uint vol[VOICES];
uint *seq[VOICES];
uint delay[VOICES];

void init_PSG()
{
   uint index;

   for (index=0; index<VOICES; index++ ) {
      vol[index] = 0;
      set_vol(index+1,vol[index]);
      seq[index] = 0;
      delay[index];
   }
}

void interpret_sound(index)
uint index;
{
   uint opcode;
   uint operand;

   if ( seq[index] ) {
      opcode = *seq[index] & OPCODE_MASK;
      operand = *seq[index] & OPERAND_MASK;
      switch ( opcode ) {
         case END_SOUND:
            seq[index] = 0;
         break;
         case SET_DELAY: /* obsolete */
            delay[index] = operand;
            seq[index]++;
         break;
         case WAIT_DELAY: /* obsolete */
            if (delay[index] == 0)
               seq[index]++;
            else
               delay[index]--;
         break;
         case DELAY:
            if (delay[index] == 0) { /* first time for this opcode */
               delay[index] = operand;
            } else { /* subsequent time (delay is "in progress") */
               delay[index]--;
               if (delay[index] == 0)
                  seq[index]++;
               /* else the delay continues */
            }
         break;
         case SET_NOISE:
            set_noise(operand);
            seq[index]++;
         break;
         case SET_VOLN:
            set_vol(4, operand);
            seq[index]++;
         break;
         case SET_TONE1:
            set_freq(1,operand);
            seq[index]++;
         break;
         case SET_VOL1:
            set_vol(1, operand);
            seq[index]++;
         break;
         case SET_TONE2:
            set_freq(2,operand);
            seq[index]++;
         break;
         case SET_VOL2:
            set_vol(2, operand);
            seq[index]++;
         break;
         case SET_TONE3:
            set_freq(3,operand);
            seq[index]++;
         break;
         case SET_VOL3:
            set_vol(3, operand);
            seq[index]++;
         break;
         case SET_TONEX:
            set_freq(index+1,operand);
            seq[index]++;
         break;
         case SET_VOLX:
            set_vol(index+1, operand);
            seq[index]++;
         break;
      }
   }
}


void generate_sound()
{
   interpret_sound(0);
   interpret_sound(1);
   interpret_sound(2);
   interpret_sound(3);
}

void start_sound(voice,sound)
uint voice;
uint *sound;
{
   seq[voice-1] = sound;
}

int sound_finished(voice)
{
   if (seq[voice-1] != 0)
      return FALSE;
   else
      return TRUE;
}

uint Explode1[] = {
   SET_NOISE+FB_LO+FOLLOW_T3,
   SET_VOL3+0,
   SET_TONE3+60, SET_VOLN+15, DELAY+30,
   SET_TONE3+61, DELAY+3,
   SET_TONE3+62, DELAY+3,
   SET_TONE3+63, DELAY+3,
   SET_TONE3+64, DELAY+3,
   SET_TONE3+65, DELAY+3,
   SET_TONE3+66, DELAY+3,
   SET_VOLN+14, SET_TONE3+67, DELAY+3,
   SET_VOLN+13, SET_TONE3+68, DELAY+3,
   SET_VOLN+12, SET_TONE3+69, DELAY+3,
   SET_VOLN+11, SET_TONE3+70, DELAY+3,
   SET_VOLN+10, SET_TONE3+71, DELAY+3,
   SET_VOLN+9, SET_TONE3+72, DELAY+2,
   SET_VOLN+8, SET_TONE3+73, DELAY+2,
   SET_VOLN+7, SET_TONE3+74, DELAY+2,
   SET_VOLN+6, SET_TONE3+75, DELAY+2,
   SET_VOLN+5, SET_TONE3+76, DELAY+2,
   SET_VOLN+4, SET_TONE3+77, DELAY+2,
   SET_VOLN+3, SET_TONE3+78, DELAY+2,
   SET_VOLN+2, SET_TONE3+79, DELAY+1,
   SET_VOLN+1, SET_TONE3+80,
   SET_VOLN+0, END_SOUND
};

uint Shot1[] = {
   SET_TONEX+440,
   SET_VOLX+10,
   SET_TONEX+460,
   SET_TONEX+480,
   SET_TONEX+500,
   SET_TONEX+520,
   SET_TONEX+540,
   SET_VOLX+0,
   END_SOUND
};

uint Ding1[] = {
   SET_VOLX+12,
   SET_TONEX+110,
   SET_VOLX+11,
   SET_VOLX+10,
   SET_VOLX+9,
   SET_VOLX+8,
   SET_VOLX+7,
   SET_VOLX+6,
   SET_VOLX+5,
   SET_VOLX+4,
   SET_VOLX+3,
   SET_VOLX+2,
   SET_DELAY+6,
   WAIT_DELAY,
   SET_VOLX+1,
   SET_DELAY+5,
   WAIT_DELAY,
   SET_VOLX+0,
   END_SOUND
};

uint Dong1[] = {
   SET_TONEX+1023,
   SET_VOLX+13,
   SET_VOLX+12,
   SET_VOLX+11,
   SET_VOLX+10,
   SET_VOLX+9,
   SET_VOLX+8,
   SET_VOLX+7,
   SET_VOLX+6,
   SET_VOLX+5,
   SET_VOLX+4,
   SET_VOLX+3,
   SET_VOLX+2,
   SET_VOLX+1,
   SET_VOLX+0,
   END_SOUND
};

uint TanTanTaan1[] = { /* By Haroldo */
   SET_VOLX+15,
   SET_TONEX+494,
   DELAY+10,
   SET_VOLX+0,
   DELAY+10,
   SET_TONEX+493,
   SET_VOLX+15,
   DELAY+6,
   SET_TONEX+323,
   SET_VOLX+15,
   DELAY+20,
   SET_VOLX+0,
   END_SOUND
};

