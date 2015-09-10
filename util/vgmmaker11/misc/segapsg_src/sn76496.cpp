/***************************************************************************

  sn76496.c
  by Nicola Salmoria
  with contributions by others

  Routines to emulate the:
  Texas Instruments SN76489, SN76489A, SN76494/SN76496
  ( Also known as, or at least compatible with, the TMS9919 and SN94624.)
  and the Sega 'PSG' used on the Master System, Game Gear, and Megadrive/Genesis
  This chip is known as the Programmable Sound Generator, or PSG, and is a 4
  channel sound generator, with three squarewave channels and a noise/arbitrary
  duty cycle channel.

  Noise emulation for all verified chips should be accurate:

  ** SN76489 uses a 15-bit shift register with taps on bits D and E, output on E,
  XOR function.
  It uses a 15-bit ring buffer for periodic noise/arbitrary duty cycle.
  Its output is inverted.
  ** SN94624 is the same as SN76489 but lacks the /8 divider on its clock input.
  ** SN76489A uses a 15-bit shift register with taps on bits D and E, output on F,
  XOR function.
  It uses a 15-bit ring buffer for periodic noise/arbitrary duty cycle.
  Its output is not inverted.
  ** SN76494 is the same as SN76489A but lacks the /8 divider on its clock input.
  ** SN76496 is identical in operation to the SN76489A, but the audio input is
  documented.
  All the TI-made PSG chips have an audio input line which is mixed with the 4 channels
  of output. (It is undocumented and may not function properly on the sn76489, 76489a
  and 76494; the sn76489a input is mentioned in datasheets for the tms5200)
  All the TI-made PSG chips act as if the frequency was set to 0x400 if 0 is
  written to the frequency register.
  ** Sega Master System III/MD/Genesis PSG uses a 16-bit shift register with taps
  on bits C and F, output on F
  It uses a 16-bit ring buffer for periodic noise/arbitrary duty cycle.
  (whether it uses an XOR or XNOR needs to be verified, assumed XOR)
  (whether output is inverted or not needs to be verified, assumed to be inverted)
  ** Sega Game Gear PSG is identical to the SMS3/MD/Genesis one except it has an
  extra register for mapping which channels go to which speaker.
  The register, connected to a z80 port, means:
  for bits 7  6  5  4  3  2  1  0
           L3 L2 L1 L0 R3 R2 R1 R0
  Noise is an XOR function, and audio output is negated before being output.
  All the Sega-made PSG chips act as if the frequency was set to 0 if 0 is written
  to the frequency register.
  ** NCR7496 (as used on the Tandy 1000) is similar to the SN76489 but with a
  different noise LFSR patttern: taps on bits A and E, output on E
  It uses a 15-bit ring buffer for periodic noise/arbitrary duty cycle.
  (all this chip's info needs to be verified)

  28/03/2005 : Sebastien Chevalier
  Update th SN76496Write func, according to SN76489 doc found on SMSPower.
   - On write with 0x80 set to 0, when LastRegister is other then TONE,
   the function is similar than update with 0x80 set to 1

  23/04/2007 : Lord Nightmare
  Major update, implement all three different noise generation algorithms and a
  set_variant call to discern among them.

  28/04/2009 : Lord Nightmare
  Add READY line readback; cleaned up struct a bit. Cleaned up comments.
  Add more TODOs. Fixed some unsaved savestate related stuff.

  04/11/2009 : Lord Nightmare
  Changed the way that the invert works (it now selects between XOR and XNOR
  for the taps), and added R->OldNoise to simulate the extra 0 that is always
  output before the noise LFSR contents are after an LFSR reset.
  This fixes SN76489/A to match chips. Added SN94624.

  14/11/2009 : Lord Nightmare
  Removed STEP mess, vastly simplifying the code. Made output bipolar rather
  than always above the 0 line, but disabled that code due to pending issues.

  16/11/2009 : Lord Nightmare
  Fix screeching in regulus: When summing together four equal channels, the
  size of the max amplitude per channel should be 1/4 of the max range, not
  1/3. Added NCR7496.

  18/11/2009 : Lord Nightmare
  Modify Init functions to support negating the audio output. The gamegear
  psg does this. Change gamegear and sega psgs to use XOR rather than XNOR
  based on testing. Got rid of R->OldNoise and fixed taps accordingly.
  Added stereo support for game gear.

  15/01/2010 : Lord Nightmare
  Fix an issue with SN76489 and SN76489A having the wrong periodic noise periods.
  Note that properly emulating the noise cycle bit timing accurately may require
  extensive rewriting.

  24/01/2010: Lord Nightmare
  Implement periodic noise as forcing one of the XNOR or XOR taps to 1 or 0 respectively.
  Thanks to PlgDavid for providing samples which helped immensely here.
  Added true clock divider emulation, so sn94624 and sn76494 run 8x faster than
  the others, as in real life.

  15/02/2010: Lord Nightmare & Michael Zapf (additional testing by PlgDavid)
  Fix noise period when set to mirror channel 3 and channel 3 period is set to 0 (tested on hardware for noise, wave needs tests) - MZ
  Fix phase of noise on sn94624 and sn76489; all chips use a standard XOR, the only inversion is the output itself - LN, Plgdavid
  Thanks to PlgDavid and Michael Zapf for providing samples which helped immensely here.

  23/02/2011: Lord Nightmare & Enik
  Made it so the Sega PSG chips have a frequency of 0 if 0 is written to the
  frequency register, while the others have 0x400 as before. Should fix a bug
  or two on sega games, particularly Vigilante on Sega Master System. Verified
  on SMS hardware.

  TODO: * Implement the TMS9919 - any difference to sn94624?
        * Implement the T6W28; has registers in a weird order, needs writes
          to be 'sanitized' first. Also is stereo, similar to game gear.
        * Test the NCR7496; Smspower says the whitenoise taps are A and E,
          but this needs verification on real hardware.
        * Factor out common code so that the SAA1099 can share some code.
        * Convert to modern device
***************************************************************************/

#include <windows.h>

typedef int INT32;
typedef unsigned int UINT32;
typedef short int INT16;
typedef unsigned short int UINT16;

#define DLL extern "C" __declspec (dllexport)

#include "sn76496.h"


#define MAX_OUTPUT 0x7fff
#define NOISEMODE (R->Register[6]&4)?1:0


typedef struct _sn76496_state sn76496_state;

struct _sn76496_state
{
	//sound_stream * Channel;
	INT32 VolTable[16];	/* volume table (for 4-bit to db conversion)*/
	INT32 Register[8];	/* registers */
	INT32 LastRegister;	/* last register written */
	INT32 Volume[4];	/* db volume of voice 0-2 and noise */
	UINT32 RNG;			/* noise generator LFSR*/
	INT32 ClockDivider;	/* clock divider */
	INT32 CurrentClock;
	INT32 FeedbackMask;	/* mask for feedback */
	INT32 WhitenoiseTap1;	/* mask for white noise tap 1 (higher one, usually bit 14) */
	INT32 WhitenoiseTap2;	/* mask for white noise tap 2 (lower one, usually bit 13)*/
	INT32 Negate;		/* output negate flag */
	INT32 StereoMask;	/* the stereo output mask */
	INT32 Period[4];	/* Length of 1/2 of waveform */
	INT32 Count[4];		/* Position within the waveform */
	INT32 Output[4];	/* 1-bit output of each channel, pre-volume */
	INT32 CyclestoREADY;/* number of cycles until the READY line goes active */
	INT32 Freq0IsMax;	/* flag for if frequency zero acts as if it is one more than max (0x3ff+1) or if it acts like 0 */
	UINT32 MuteMask;	/* to mute channels separately */
	float SampleDiv;
	INT32 VolReg[4];
};



void segapsg_write_stereo(void *chip,unsigned char data)
{
	sn76496_state *R=(sn76496_state*)chip;

	R->StereoMask = data;
}



void segapsg_write_register(void *chip,unsigned char data)
{
	sn76496_state *R=(sn76496_state*)chip;
	int n, r, c;

	/* set number of cycles until READY is active; this is always one
           'sample', i.e. it equals the clock divider exactly; until the
           clock divider is fully supported, we delay until one sample has
           played. The fact that this below is '2' and not '1' is because
           of a ?race condition? in the mess crvision driver, where after
           any sample is played at all, no matter what, the cycles_to_ready
           ends up never being not ready, unless this value is greater than
           1. Once the full clock divider stuff is written, this should no
           longer be an issue. */
	R->CyclestoREADY = 2;

	if (data & 0x80)
	{
		r = (data & 0x70) >> 4;
		R->LastRegister = r;
		R->Register[r] = (R->Register[r] & 0x3f0) | (data & 0x0f);
	}
	else
    {
		r = R->LastRegister;
	}
	c = r/2;
	switch (r)
	{
		case 0:	/* tone 0 : frequency */
		case 2:	/* tone 1 : frequency */
		case 4:	/* tone 2 : frequency */
		    if ((data & 0x80) == 0) R->Register[r] = (R->Register[r] & 0x0f) | ((data & 0x3f) << 4);
			if ((R->Register[r] != 0) || (R->Freq0IsMax == 0)) R->Period[c] = R->Register[r];
			else R->Period[c] = 0x400;

			if (r == 4)
			{
				/* update noise shift frequency */
				if ((R->Register[6] & 0x03) == 0x03)
					R->Period[3] = 2 * R->Period[2];
			}
			break;
		case 1:	/* tone 0 : volume */
		case 3:	/* tone 1 : volume */
		case 5:	/* tone 2 : volume */
		case 7:	/* noise  : volume */
			R->Volume[c] = R->VolTable[data & 0x0f];
			R->VolReg[c]=data&0x0f;
			if ((data & 0x80) == 0) R->Register[r] = (R->Register[r] & 0x3f0) | (data & 0x0f);
			break;
		case 6:	/* noise  : frequency, mode */
			{
				//if ((data & 0x80) == 0) logerror("sn76489: write to reg 6 with bit 7 clear; data was %03x, new write is %02x! report this to LN!\n", R->Register[6], data);
				if ((data & 0x80) == 0) R->Register[r] = (R->Register[r] & 0x3f0) | (data & 0x0f);
				n = R->Register[6];
				/* N/512,N/1024,N/2048,Tone #3 output */
				R->Period[3] = ((n&3) == 3) ? 2 * R->Period[2] : (1 << (5+(n&3)));
				R->RNG = R->FeedbackMask;
			}
			break;
	}
}



void segapsg_render(void *chip,int *buffer,int samples,bool add)
{
	sn76496_state *R=(sn76496_state*)chip;
	int i;
	int out = 0;
	int out2 = 0;
	UINT32 mask;
	float cnt = 0;

	mask=R->MuteMask&R->StereoMask;

	while (samples > 0)
	{
		// clock chip once
		
		out=0;
		out2=0;

		while(cnt<R->SampleDiv)
		{
			cnt+=1.0f;

			if (R->CurrentClock > 0) // not ready for new divided clock
			{
				R->CurrentClock--;
			}
			else // ready for new divided clock, make a new sample
			{
				R->CurrentClock = R->ClockDivider-1;
				/* decrement Cycles to READY by one */
				if (R->CyclestoREADY >0) R->CyclestoREADY--;

				// handle channels 0,1,2
				for (i = 0;i < 3;i++)
				{
					R->Count[i]--;
					if (R->Count[i] <= 0)
					{
						R->Output[i] ^= 1;
						R->Count[i] = R->Period[i];
					}
				}

				// handle channel 3
				R->Count[3]--;
				if (R->Count[3] <= 0)
				{
				// if noisemode is 1, both taps are enabled
				// if noisemode is 0, the lower tap, whitenoisetap2, is held at 0
					if (((R->RNG & R->WhitenoiseTap1)?1:0) ^ ((((R->RNG & R->WhitenoiseTap2)?1:0))*(NOISEMODE)))
					{
						R->RNG >>= 1;
						R->RNG |= R->FeedbackMask;
					}
					else
					{
						R->RNG >>= 1;
					}
					R->Output[3] = R->RNG & 1;

					R->Count[3] = R->Period[3];
				}
			}

			out += (((mask&0x10)&&R->Output[0])?R->Volume[0]:0)
				+ (((mask&0x20)&&R->Output[1])?R->Volume[1]:0)
				+ (((mask&0x40)&&R->Output[2])?R->Volume[2]:0)
				+ (((mask&0x80)&&R->Output[3])?R->Volume[3]:0);

			out2 +=(((mask&0x01)&&R->Output[0])?R->Volume[0]:0)
				+ (((mask&0x02)&&R->Output[1])?R->Volume[1]:0)
				+ (((mask&0x04)&&R->Output[2])?R->Volume[2]:0)
				+ (((mask&0x08)&&R->Output[3])?R->Volume[3]:0);
		}

		out=(int)(((float)out)/cnt);
		out2=(int)(((float)out2)/cnt);

		cnt-=R->SampleDiv;

		if(R->Negate) { out = -out; out2 = -out2; }

		if(add)
		{
			*buffer+++=out;
			*buffer+++=out2;
		}
		else
		{
			*buffer++=out;
			*buffer++=out2;
		}

		samples--;
	}
}



void segapsg_set_gain(void *chip,int gain)
{
	sn76496_state *R=(sn76496_state*)chip;
	int i;
	double out;


	gain &= 0xff;

	/* increase max output basing on gain (0.2 dB per step) */
	out = MAX_OUTPUT / 4; // four channels, each gets 1/4 of the total range
	while (gain-- > 0)
		out *= 1.023292992;	/* = (10 ^ (0.2/20)) */

	/* build volume table (2dB per step) */
	for (i = 0;i < 15;i++)
	{
		/* limit volume to avoid clipping */
		if (out > MAX_OUTPUT / 4) R->VolTable[i] = MAX_OUTPUT / 4;
		else R->VolTable[i] = (INT32)out;

		out /= 1.258925412;	/* = 10 ^ (2/20) = 2dB */
	}
	R->VolTable[15] = 0;
}



void* segapsg_init(int base_clock,int rate,bool neg)
{
	sn76496_state *R;
	int i;

	R=(sn76496_state*)malloc(sizeof(sn76496_state));
	memset(R,0,sizeof(sn76496_state));

	R->SampleDiv=(float)base_clock/4/(float)rate;

	for (i = 0;i < 4;i++)
	{
		R->Volume[i] = 15;
		R->VolReg[i]=15;
	}

	R->LastRegister = 0;
	for (i = 0;i < 8;i+=2)
	{
		R->Register[i] = 0;
		R->Register[i + 1] = 0x0f;	/* volume = 0 */
	}

	for (i = 0;i < 4;i++)
	{
		R->Output[i] = R->Period[i] = R->Count[i] = 0;
	}

	/* Default is SN76489A */
	R->ClockDivider = 16/4;
	R->FeedbackMask = 0x8000;     /* mask for feedback */
	R->WhitenoiseTap1 = 0x01;   /* mask for white noise tap 1*/
	R->WhitenoiseTap2 = 0x08;   /* mask for white noise tap 2*/
	R->Negate = neg?1:0; /* channels are not negated */
	R->CyclestoREADY = 1; /* assume ready is not active immediately on init. is this correct?*/
	R->StereoMask = 0xFF; /* all channels enabled */
	R->Freq0IsMax = 0; /* frequency set to 0 results in freq = 0x400 rather than 0 */

	R->RNG = R->FeedbackMask;
	R->Output[3] = R->RNG & 1;

	R->MuteMask=0xff;

	return R;
}



void segapsg_shutdown(void *chip)
{
	free(chip);
}



void segapsg_set_mute(void *chip,int mask)
{
	sn76496_state *R=(sn76496_state*)chip;
	R->MuteMask=(mask&0x0f)|((mask<<4)&0xf0);
}



float segapsg_get_channel_volume(void *chip,int channel)
{
	sn76496_state *R=(sn76496_state*)chip;
	return ((float)15-R->VolReg[channel])/15.0f;
}



const char* segapsg_about(void)
{
	return "SN76489 and its variants emulation code is from MAME\nby Nicola Salmoria with contributions by others";
}