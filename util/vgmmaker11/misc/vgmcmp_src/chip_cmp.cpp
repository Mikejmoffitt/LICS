#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include "stdbool.h"

typedef struct sn76496_data
{
	unsigned char FreqMSB[0x04];
	unsigned char FreqLSB[0x04];
	unsigned char VolData[0x04];
	unsigned char LastReg;
	bool LastRet;
} SN76496_DATA;
typedef struct ym2413_data
{
	unsigned char RegData[0x100];
	unsigned char RegFirst[0x100];
} YM2413_DATA;
typedef struct ym2612_data
{
	unsigned char RegData[0x200];
	unsigned char RegFirst[0x200];
	unsigned char KeyOn[0x08];
	unsigned char KeyFirst[0x08];
} YM2612_DATA;

typedef struct ym2151_data
{
	unsigned char RegData[0x100];
	unsigned char RegFirst[0x100];
	unsigned char MCMask[0x08];
	unsigned char MCFirst[0x08];
} YM2151_DATA;
typedef struct segapcm_data
{
	unsigned char* ROMData;
	unsigned char* ROMUsage;
	unsigned char RAMData[0x800];
	unsigned char RAMFirst[0x800];
} SEGAPCM_DATA;
typedef struct _rf5c68_channel
{
	unsigned char ChnReg[0x07];
	unsigned char RegFirst[0x07];
} RF5C68_CHANNEL;
#define RF_CBANK	0x00
#define RF_WBANK	0x01
#define RF_ENABLE	0x02
#define RF_CHN_MASK	0x03
typedef struct rf5c68_data
{
	RF5C68_CHANNEL chan[0x08];
	unsigned char RegData[0x04];
	unsigned char RegFirst[0x04];
} RF5C68_DATA;
typedef struct ym2203_data
{
	unsigned char RegData[0x100];
	unsigned char RegFirst[0x100];
	unsigned char KeyOn[0x04];
	unsigned char KeyFirst[0x04];
	unsigned char PreSclCmd;
} YM2203_DATA;
typedef struct ym2608_data
{
	unsigned char RegData[0x200];
	unsigned char RegFirst[0x200];
	unsigned char KeyOn[0x08];
	unsigned char KeyFirst[0x08];
	unsigned char PreSclCmd;
} YM2608_DATA;
typedef struct ym2610_data
{
	unsigned char RegData[0x200];
	unsigned char RegFirst[0x200];
	unsigned char KeyOn[0x08];
	unsigned char KeyFirst[0x08];
	unsigned char PreSclCmd;
} YM2610_DATA;
typedef struct ym3812_data
{
	unsigned char RegData[0x100];
	unsigned char RegFirst[0x100];
} YM3812_DATA;
typedef struct ym3526_data
{
	unsigned char RegData[0x100];
	unsigned char RegFirst[0x100];
} YM3526_DATA;
typedef struct y8950_data
{
	unsigned char RegData[0x100];
	unsigned char RegFirst[0x100];
} Y8950_DATA;
typedef struct ymf262_data
{
	unsigned char RegData[0x200];
	unsigned char RegFirst[0x200];
} YMF262_DATA;
typedef struct ymf271_slot
{
	unsigned char RegData[0x10];
	unsigned char RegFirst[0x10];
	
	unsigned char startaddr[3];
	unsigned char loopaddr[3];
	unsigned char endaddr[3];
	unsigned char slotnote;
	unsigned char sltnfirst;
} YMF271_SLOT;
typedef struct ymf271_group
{
	unsigned char Data;
	unsigned char First;
	unsigned char sync;
} YMF271_GROUP;
typedef struct ymf271_chip
{
	YMF271_SLOT slots[48];
	YMF271_GROUP groups[12];
	
	unsigned char ext_address[2];
	unsigned char ext_read;
} YMF271_DATA;
typedef struct ymf278b_data
{
	unsigned char RegData[0x300];
	unsigned char RegFirst[0x300];
} YMF278B_DATA;
typedef struct ymz280b_data
{
	unsigned char RegData[0x100];
	unsigned char RegFirst[0x100];
	unsigned char KeyOn[0x08];
} YMZ280B_DATA;
typedef struct ay8910_data
{
	unsigned char RegData[0x10];
	unsigned char RegFirst[0x10];
} AY8910_DATA;

typedef struct all_chips
{
	unsigned char GGSt;
	SN76496_DATA SN76496;
	YM2413_DATA YM2413;
	YM2612_DATA YM2612;
	YM2151_DATA YM2151;
	SEGAPCM_DATA SegaPCM;
	RF5C68_DATA RF5C68;
	YM2203_DATA YM2203;
	YM2608_DATA YM2608;
	YM2610_DATA YM2610;
	YM3812_DATA YM3812;
	YM3526_DATA YM3526;
	Y8950_DATA Y8950;
	YMF262_DATA YMF262;
	YMF271_DATA YMF271;
	YMF278B_DATA YMF278B;
	YMZ280B_DATA YMZ280B;
	RF5C68_DATA RF5C164;
	AY8910_DATA AY8910;
} ALL_CHIPS;

void InitAllChips(void);
void FreeAllChips(void);
void SetChipSet(unsigned char ChipID);
bool GGStereo(unsigned char Data);
bool sn76496_write(unsigned char Command/*, unsigned char NextCmd*/);
bool ym2413_write(unsigned char Register, unsigned char Data);
bool ym2612_write(unsigned char Port, unsigned char Register, unsigned char Data);
bool ym2151_write(unsigned char Register, unsigned char Data);
bool segapcm_mem_write(unsigned short int Offset, unsigned char Data);
static bool rf_pcm_reg_write(RF5C68_DATA* chip, unsigned char Register, unsigned char Data);
bool rf5c68_reg_write(unsigned char Register, unsigned char Data);
bool ym2203_write(unsigned char Register, unsigned char Data);
bool ym2608_write(unsigned char Port, unsigned char Register, unsigned char Data);
bool ym2610_write(unsigned char Port, unsigned char Register, unsigned char Data);
bool ym3812_write(unsigned char Register, unsigned char Data);
bool ym3526_write(unsigned char Register, unsigned char Data);
bool y8950_write(unsigned char Register, unsigned char Data);
bool ymf262_write(unsigned char Port, unsigned char Register, unsigned char Data);
bool ymz280b_write(unsigned char Register, unsigned char Data);
bool rf5c164_reg_write(unsigned char Register, unsigned char Data);
bool ay8910_write_reg(unsigned char Register, unsigned char Data);
static bool ymf271_write_fm_reg(YMF271_SLOT* slot, unsigned char Register, unsigned char Data);
static bool ymf271_write_fm(YMF271_DATA* chip, unsigned char Port, unsigned char Register, unsigned char Data);
bool ymf271_write(unsigned char Port, unsigned char Register, unsigned char Data);

bool GetNextChipCommand(void);

unsigned char ChipCount = 0x02;
ALL_CHIPS* ChipData = NULL;
ALL_CHIPS* ChDat;

extern bool JustTimerCmds;

extern unsigned short int NxtCmdReg;
extern unsigned char NxtCmdVal;

void InitAllChips(void)
{
	unsigned char CurChip;
	
	if (ChipData == NULL)
		ChipData = (ALL_CHIPS*)malloc(ChipCount * sizeof(ALL_CHIPS));
	for (CurChip = 0x00; CurChip < ChipCount; CurChip ++)
	{
		memset(&ChipData[CurChip], 0xFF, sizeof(ALL_CHIPS));
		
		ChipData[CurChip].GGSt = 0x00;
		memset(ChipData[CurChip].YMZ280B.KeyOn, 0x00, sizeof(unsigned char) * 0x08);
	}
	
	SetChipSet(0x00);
	
	return;
}

void FreeAllChips(void)
{
	if (ChipData == NULL)
		return;
	
	free(ChipData);
	ChipData = NULL;
	
	return;
}

void SetChipSet(unsigned char ChipID)
{
	ChDat = ChipData + ChipID;
	
	return;
}

bool GGStereo(unsigned char Data)
{
	if (Data == ChDat->GGSt && ! JustTimerCmds)
		return false;
	
	ChDat->GGSt = Data;
	return true;
}

bool sn76496_write(unsigned char Command/*, unsigned char NextCmd*/)
{
	SN76496_DATA* chip;
	unsigned char Channel;
	unsigned char Reg;
	unsigned char Data;
	bool RetVal;
	unsigned char NextCmd;
	
	if (JustTimerCmds)
		return true;
	
	chip = &ChDat->SN76496;
	
	RetVal = GetNextChipCommand();
	NextCmd = RetVal ? NxtCmdVal : 0x80;
	
	RetVal = true;
	if (Command & 0x80)
	{
		Reg = (Command & 0x70) >> 4;
		Channel = Reg >> 1;
		Data = Command & 0x0F;
		chip->LastReg = Reg;
		
		switch(Reg)
		{
		case 0:	// Tone 0: Frequency
		case 2:	// Tone 1: Frequency
		case 4:	// Tone 2: Frequency
			if (Data == chip->FreqMSB[Channel])
			{
				if (NextCmd & 0x80)
				{
					// Next command doesn't depend on the current one
					RetVal = false;
				}
				else
				{
					Data = NextCmd & 0x7F;	// NextCmd & 0x3F
					if (Data == chip->FreqLSB[Channel])
						RetVal = false;
				}
			}
			else
			{
				chip->FreqMSB[Channel] = Data;
			}
			break;
		case 6:	// Noise:  Frequency, Mode
			return true;	// a Noise Mode write resets the Noise Shifter
		case 1:	// Tone 0: Volume
		case 3:	// Tone 1: Volume
		case 5:	// Tone 2: Volume
		case 7:	// Noise:  Volume
			if ((Command & 0x0F) == chip->VolData[Channel])
			{
				if (NextCmd & 0x80)
				{
					// Next command doesn't depend on the current one
					RetVal = false;
				}
				else
				{
					Data = NextCmd & 0x0F;
					if (Data == chip->VolData[Channel])
						RetVal = false;
					printf("Warning! Data Command after Volume Command!\n");
				}
			}
			else
			{
				chip->VolData[Channel] = Data;
			}
			break;
		}
	}
	else
	{
		Reg = chip->LastReg;
		Channel = Reg >> 1;
		Data = Command & 0x7F;	// Command & 0x3F
		
		if (! (Reg & 0x10))
		{
			if (Data == chip->FreqLSB[Channel])
				RetVal = chip->LastRet;	// remove event only, if previous event was removed
			else
				chip->FreqLSB[Channel] = Data;
		}
		else
		{
			// I still handle this correctly
			if (Data == chip->VolData[Channel])
				RetVal = chip->LastRet;
			else
				chip->VolData[Channel] = Data;
		}
	}
	chip->LastRet = RetVal;
	//if (Channel != 0x00)
	//if (Channel != 0x01)
	//if (Channel != 0x02 || (ChDat != ChipData && !(Reg & 0x01)))
	//if (! (Channel == 0x03 || (ChDat != ChipData && Channel == 0x02 && !(Reg & 0x01))))
	//	RetVal = false;
	
	return RetVal;
}

bool ym2413_write(unsigned char Register, unsigned char Data)
{
	if (! ChDat->YM2413.RegFirst[Register] && Data == ChDat->YM2413.RegData[Register])
		return false;
	
	ChDat->YM2413.RegFirst[Register] = JustTimerCmds;
	ChDat->YM2413.RegData[Register] = Data;
	return true;
}

bool ym2612_write(unsigned char Port, unsigned char Register, unsigned char Data)
{
	YM2612_DATA* chip;
	unsigned short int RegVal;
	unsigned char Channel;
	
	chip = &ChDat->YM2612;
	RegVal = (Port << 8) | Register;
	switch(RegVal)
	{
	case 0x24:	// Timer Registers
	case 0x25:
	case 0x26:
		return false;
	// no OPN Prescaler Registers for YM2612
	case 0x27:
		Data &= 0xC0;	// mask out all timer-relevant bits
		
		if (! chip->RegFirst[RegVal] && Data == chip->RegData[RegVal])
			return false;
		
		chip->RegFirst[RegVal] = 0x00;
		chip->RegData[RegVal] = Data;
		break;
	case 0x28:
		Channel = Data & 0x07;
		
		if (! chip->KeyFirst[Channel] && Data == chip->KeyOn[Channel])
			return false;
		
		chip->KeyFirst[Channel] = JustTimerCmds;
		chip->KeyOn[Channel] = Data;
		break;
	case 0x2A:
		/*// Hack for Pier Solar Beta
		// Remove every 2nd DAC command (because every DAC command is sent 2 times)
		// this includes a check and a warning output
		chip->RegFirst[RegVal] ^= 0x01;
		if (chip->RegFirst[RegVal] & 0x01)
		{
			if (Data == chip->RegData[RegVal])
				return false;
			else //if (Data != chip->RegData[RegVal])
				printf("Warning! DAC Compression failed!\n");
		}
		chip->RegData[RegVal] = Data;*/
		return true;	// I leave this on for later optimizations
	default:
		// no SSG emulator for YM2612
		switch(RegVal & 0xF4)
		{
		case 0xA0:	// A0-A3 and A8-AB
			if ((RegVal & 0x03) == 0x03)
				break;
			
			if (! chip->RegFirst[RegVal] && Data == chip->RegData[RegVal])
				return false;
			
			chip->RegFirst[RegVal] = JustTimerCmds;
			chip->RegData[RegVal] = Data;
			return true;
		case 0xA4:	// A4-A7 and AC-AF - Frequence Latch
			if ((RegVal & 0x03) == 0x03)
				break;
			
			// FINALLY, I got it to work properly
			// The vgm I tested (Dyna Brothers 2 - 28 - Get Crazy - More Rave.vgz) was
			// successfully tested against the Gens and MAME cores.
			while(GetNextChipCommand())
			{
				if ((NxtCmdReg & 0x1FC) == (RegVal & 0x1FC))
				{
					return false;	// this will be ignored, because the A0 write is missing
				}
				else if ((NxtCmdReg & 0x1FF) == (RegVal & 0x1FB))
				{
					if (chip->RegFirst[RegVal])
					{
						chip->RegFirst[RegVal] = JustTimerCmds;
						chip->RegData[RegVal] = Data;
						chip->RegFirst[RegVal & 0x1FB] = 0x01;
						return true;
					}
					else if (chip->RegData[RegVal] == Data &&
							chip->RegData[RegVal & 0x1FB] == NxtCmdVal)
					{
						chip->RegFirst[RegVal] = JustTimerCmds;
						chip->RegData[RegVal] = Data;
						chip->RegFirst[RegVal & 0x1FB] = JustTimerCmds;
						return false;
					}
					else
					{
						chip->RegFirst[RegVal] = JustTimerCmds;
						chip->RegData[RegVal] = Data;
						chip->RegFirst[RegVal & 0x1FB] = 0x01;
						return true;
					}
				}
			}
			chip->RegData[RegVal] = Data;
			return true;
		}
		
		if (! chip->RegFirst[RegVal] && Data == chip->RegData[RegVal])
			return false;
		
		chip->RegFirst[RegVal] = JustTimerCmds;
		chip->RegData[RegVal] = Data;
		break;
	}
	
	return true;
}

bool ym2151_write(unsigned char Register, unsigned char Data)
{
	YM2151_DATA* chip;
	unsigned char Channel;
	
	chip = &ChDat->YM2151;
	switch(Register)
	{
	case 0x08:
		Channel = Data & 0x07;
		if (! chip->MCFirst[Channel] && (Data & 0xF8) == chip->MCMask[Channel])
			return false;
		
		chip->MCFirst[Channel] = JustTimerCmds;
		chip->MCMask[Channel] = Data & 0xF8;
		break;
	case 0x10:	// Timer Registers
	case 0x11:
	case 0x12:
	case 0x14:
		if (! chip->RegFirst[Register] && Data == chip->RegData[Register])
			return false;
		
		chip->RegFirst[Register] = 0x00;
		chip->RegData[Register] = Data;
		break;
	default:
		if (! chip->RegFirst[Register] && Data == chip->RegData[Register])
			return false;
		
		chip->RegFirst[Register] = JustTimerCmds;
		chip->RegData[Register] = Data;
		break;
	}
	
	return true;
}

bool segapcm_mem_write(unsigned short int Offset, unsigned char Data)
{
	// SegaPCM Chips are almost impossible to spam, and it's difficult
	// to remove memory write, as they're changed by the chip during emulation
	return true;
}

static bool rf_pcm_reg_write(RF5C68_DATA* chip, unsigned char Register, unsigned char Data)
{
	RF5C68_CHANNEL* chan;
	unsigned char OldVal;
	
	switch(Register)
	{
	case 0x00:	// Envelope
	case 0x01:	// Pan
	case 0x02:	// FD Low / step
	case 0x03:	// FD High / step
	case 0x04:	// Loop Start Low
	case 0x05:	// Loop Start High
	case 0x06:	// Start
		chan = &chip->chan[chip->RegData[RF_CBANK]];
		
		if (Register == 0x06)
		{
			OldVal = chip->RegData[RF_CHN_MASK] & (0x01 << chip->RegData[RF_CBANK]);
			if (! OldVal)
				chan->RegFirst[Register] = 0x01;
		}
		
		if (! chan->RegFirst[Register] && Data == chan->ChnReg[Register])
			return false;
		
		chan->RegFirst[Register] = JustTimerCmds;
		chan->ChnReg[Register] = Data;
		break;
	case 0x07:	// Control Register
		OldVal = chip->RegData[RF_ENABLE];
		if (Data & 0x40)
			OldVal |= chip->RegData[RF_CBANK];
		else
			OldVal |= chip->RegData[RF_WBANK];
		if (! chip->RegFirst[RF_ENABLE] && Data == OldVal)
			return false;
		
		chip->RegFirst[RF_ENABLE] = JustTimerCmds;
		chip->RegData[RF_ENABLE] = Data & 0x80;
		if (Data & 0x40)
			chip->RegData[RF_CBANK] = Data & 0x07;
		else
			chip->RegData[RF_WBANK] = Data & 0x0F;
		
		if (! chip->RegFirst[RF_ENABLE] && (Data & 0x40))
		{
			// additional test for 2 Channel Select-Commands after each other
			// that makes first first useless, of course :)
			while(GetNextChipCommand())
			{
				if (NxtCmdReg <= 0x06)
					return true;
				else if (NxtCmdReg == 0x07 && (Data & 0x40))
					return false;
			}
		}
		break;
	case 0x08:	// Channel On/Off Register
		if (! chip->RegFirst[RF_CHN_MASK] && Data == chip->RegData[RF_CHN_MASK])
			return false;
		
		chip->RegFirst[RF_CHN_MASK] = JustTimerCmds;
		chip->RegData[RF_CHN_MASK] = Data;
		break;
	}
	
	return true;
}

bool rf5c68_reg_write(unsigned char Register, unsigned char Data)
{
	RF5C68_DATA* chip = &ChDat->RF5C68;
	
	return rf_pcm_reg_write(chip, Register, Data);
}

bool ym2203_write(unsigned char Register, unsigned char Data)
{
	YM2203_DATA* chip;
	unsigned char Channel;
	
	chip = &ChDat->YM2203;
	switch(Register)
	{
	case 0x24:	// Timer Registers
	case 0x25:
	case 0x26:
		return false;
	case 0x27:
		Data &= 0xC0;	// mask out all timer-relevant bits
		
		if (! chip->RegFirst[Register] && Data == chip->RegData[Register])
			return false;
		
		chip->RegFirst[Register] = 0x00;
		chip->RegData[Register] = Data;
		break;
	case 0x2D:	// OPN Prescaler Registers
	case 0x2E:
	case 0x2F:
		if (chip->PreSclCmd == Register)
			return false;
		chip->PreSclCmd = Register;
		break;
	case 0x28:
		Channel = Data & 0x03;
		
		if (! chip->KeyFirst[Channel] && Data == chip->KeyOn[Channel])
			return false;
		
		chip->KeyFirst[Channel] = JustTimerCmds;
		chip->KeyOn[Channel] = Data;
		break;
	default:
		if ((Register & 0xF0) < 0x10)
			return true;	// I don't know anything about the SSG emulator
							// UPDATE: I think, this is just the AY8910-part
		switch(Register & 0xF4)
		{
		case 0xA0:	// A0-A3 and A8-AB
			return true;	// Rewrite is neccessary: Phase Increment is recalculated
		case 0xA4:	// A4-A7 and AC-AF - Frequence Latch
			return true;
			// For some reason (I really can't say why) the code below doesn't work properly
			/*if ((RegVal & 0x03) == 0x03)
				break;
			RegVal &= 0x0FC;	// Registers A4-A6 (AC-AE for Ch3) write to the same offset
			break;*/
		}
		
		if (! chip->RegFirst[Register] && Data == chip->RegData[Register])
			return false;
		
		chip->RegFirst[Register] = JustTimerCmds;
		chip->RegData[Register] = Data;
		break;
	}
	
	return true;
}

bool ym2608_write(unsigned char Port, unsigned char Register, unsigned char Data)
{
	YM2608_DATA* chip;
	unsigned short int RegVal;
	unsigned char Channel;
	
	chip = &ChDat->YM2608;
	RegVal = (Port << 8) | Register;
	switch(RegVal)
	{
	case 0x24:	// Timer Registers
	case 0x25:
	case 0x26:
		return false;
	case 0x27:
		Data &= 0xC0;	// mask out all timer-relevant bits
		
		if (! chip->RegFirst[RegVal] && Data == chip->RegData[RegVal])
			return false;
		
		chip->RegFirst[RegVal] = 0x00;
		chip->RegData[RegVal] = Data;
		break;
	case 0x2D:	// OPN Prescaler Registers
	case 0x2E:
	case 0x2F:
		if (chip->PreSclCmd == Register)
			return false;
		break;
	case 0x28:
		Channel = Data & 0x07;
		
		if (! chip->KeyFirst[Channel] && Data == chip->KeyOn[Channel])
			return false;
		
		chip->KeyFirst[Channel] = JustTimerCmds;
		chip->KeyOn[Channel] = Data;
		break;
	default:
		if ((RegVal & 0x1F0) < 0x10)
			return true;	// I don't know anything about the SSG emulator
							// UPDATE: just the AY8910-part (like above)
		if ((RegVal & 0x1F0) >= 0x100 && (RegVal & 0x1F0) < 0x110)	// DELTA-T
		{
			if ((RegVal & 0x0F) != 0x0E)	// DAC Data is handled like DAC of YM2612
				return true;	// I assume that these registers are not spammed
		}
		switch(RegVal & 0xF4)
		{
		case 0xA0:	// A0-A3 and A8-AB
			return true;	// Rewrite is neccessary: Phase Increment is recalculated
		case 0xA4:	// A4-A7 and AC-AF - Frequence Latch
			return true;
			// For some reason (I really can't say why) the code below doesn't work properly
			/*if ((RegVal & 0x03) == 0x03)
				break;
			RegVal &= 0x0FC;	// Registers A4-A6 (AC-AE for Ch3) write to the same offset
			break;*/
		}
		
		if (! chip->RegFirst[RegVal] && Data == chip->RegData[RegVal])
			return false;
		
		chip->RegFirst[RegVal] = JustTimerCmds;
		chip->RegData[RegVal] = Data;
		break;
	}
	
	return true;
}

bool ym2610_write(unsigned char Port, unsigned char Register, unsigned char Data)
{
	YM2610_DATA* chip;
	unsigned short int RegVal;
	unsigned char Channel;
	
	chip = &ChDat->YM2610;
	RegVal = (Port << 8) | Register;
	switch(RegVal)
	{
	case 0x24:	// Timer Registers
	case 0x25:
	case 0x26:
		return false;
	// no OPN Prescaler Registers for YM2610
	case 0x27:
		Data &= 0xC0;	// mask out all timer-relevant bits
		
		if (! chip->RegFirst[RegVal] && Data == chip->RegData[RegVal])
			return false;
		
		chip->RegFirst[RegVal] = 0x00;
		chip->RegData[RegVal] = Data;
		break;
	case 0x28:
		Channel = Data & 0x07;
		
		if (! chip->KeyFirst[Channel] && Data == chip->KeyOn[Channel])
			return false;
		
		chip->KeyFirst[Channel] = JustTimerCmds;
		chip->KeyOn[Channel] = Data;
		break;
	default:
		if (RegVal == 0x1C)
			return false;	// controls only Status Bits
		if ((RegVal & 0x1F0) < 0x10)
			return true;	// I don't know anything about the SSG emulator
							// UPDATE: just the AY8910-part (like above)
		if ((RegVal & 0x1F0) >= 0x010 && (RegVal & 0x1F0) < 0x020)	// DELTA-T
			return true;	// I assume that these registers are not spammed
		if ((RegVal & 0x1F0) >= 0x100 && (RegVal & 0x1F0) < 0x130)	// ADPCM
			return true;	// I assume that these registers are not spammed
		switch(RegVal & 0xF4)
		{
		case 0xA0:	// A0-A3 and A8-AB
			return true;	// Rewrite is neccessary: Phase Increment is recalculated
		case 0xA4:	// A4-A7 and AC-AF - Frequence Latch
			return true;
			// For some reason (I really can't say why) the code below doesn't work properly
			/*if ((RegVal & 0x03) == 0x03)
				break;
			RegVal &= 0x0FC;	// Registers A4-A6 (AC-AE for Ch3) write to the same offset
			break;*/
		}
		
		if (! chip->RegFirst[RegVal] && Data == chip->RegData[RegVal])
			return false;
		
		chip->RegFirst[RegVal] = JustTimerCmds;
		chip->RegData[RegVal] = Data;
		break;
	}
	
	return true;
}

bool ym3812_write(unsigned char Register, unsigned char Data)
{
	switch(Register)
	{
	case 0x002:	// IRQ and Timer Registers
	case 0x003:
		return false;
	case 0x004:
		return false;
		/*if (Data & 0x80)
			Data &= 0x80;
		
		if (! ChDat->YM3812.RegFirst[Register] && Data == ChDat->YM3812.RegData[Register])
			return false;
		
		ChDat->YM3812.RegFirst[Register] = 0x00;
		ChDat->YM3812.RegData[Register] = Data;
		break;*/
	default:
		if (! ChDat->YM3812.RegFirst[Register] && Data == ChDat->YM3812.RegData[Register])
			return false;
		
		ChDat->YM3812.RegFirst[Register] = JustTimerCmds;
		ChDat->YM3812.RegData[Register] = Data;
		break;
	}
	
	return true;
}

bool ym3526_write(unsigned char Register, unsigned char Data)
{
	switch(Register)
	{
	case 0x002:	// IRQ and Timer Registers
	case 0x003:
		return false;
	case 0x004:
		return false;
		/*if (Data & 0x80)
			Data &= 0x80;
		
		if (! ChDat->YM3526.RegFirst[Register] && Data == ChDat->YM3526.RegData[Register])
			return false;
		
		ChDat->YM3526.RegFirst[Register] = 0x00;
		ChDat->YM3526.RegData[Register] = Data;
		break;*/
	default:
		if (! ChDat->YM3526.RegFirst[Register] && Data == ChDat->YM3526.RegData[Register])
			return false;
		
		ChDat->YM3526.RegFirst[Register] = JustTimerCmds;
		ChDat->YM3526.RegData[Register] = Data;
		break;
	}
	
	return true;
}

bool y8950_write(unsigned char Register, unsigned char Data)
{
	switch(Register)
	{
	case 0x002:	// IRQ and Timer Registers
	case 0x003:
		return false;
	case 0x004:
		return false;
		/*if (Data & 0x80)
			Data &= 0x80;
		
		if (! ChDat->Y8950.RegFirst[Register] && Data == ChDat->Y8950.RegData[Register])
			return false;
		
		ChDat->Y8950.RegFirst[Register] = 0x00;
		ChDat->Y8950.RegData[Register] = Data;
		break;*/
	default:
		if (! ChDat->Y8950.RegFirst[Register] && Data == ChDat->Y8950.RegData[Register])
			return false;
		
		ChDat->Y8950.RegFirst[Register] = JustTimerCmds;
		ChDat->Y8950.RegData[Register] = Data;
		break;
	}
	
	return true;
}

bool ymf262_write(unsigned char Port, unsigned char Register, unsigned char Data)
{
	YMF262_DATA* chip = &ChDat->YMF262;
	unsigned short int RegVal;
	
	RegVal = (Port << 8) | Register;
	switch(RegVal)
	{
	case 0x002:	// IRQ and Timer Registers
	case 0x003:
		return false;
	case 0x004:
		return false;
		/*if (Data & 0x80)
			Data &= 0x80;
		
		if (! ChDat->YMF262.RegFirst[RegVal] && Data == ChDat->YMF262.RegData[RegVal])
			return false;
		
		ChDat->YMF262.RegFirst[RegVal] = 0x00;
		ChDat->YMF262.RegData[RegVal] = Data;
		break;*/
	default:
		if (! ChDat->YMF262.RegFirst[RegVal] && Data == ChDat->YMF262.RegData[RegVal])
			return false;
		
		ChDat->YMF262.RegFirst[RegVal] = JustTimerCmds;
		ChDat->YMF262.RegData[RegVal] = Data;
		break;
	}
	
	return true;
}

bool ymz280b_write(unsigned char Register, unsigned char Data)
{
	YMZ280B_DATA* chip = &ChDat->YMZ280B;
	
	// the KeyOn-Register can be sent 2x to stop a sound instantly
	if ((Register & 0xE3) == 0x01)
		return true;
	
	if (! chip->RegFirst[Register] && Data == chip->RegData[Register])
		return false;
	
	chip->RegFirst[Register] = JustTimerCmds;
	chip->RegData[Register] = Data;
	
	return true;
}

bool rf5c164_reg_write(unsigned char Register, unsigned char Data)
{
	RF5C68_DATA* chip = &ChDat->RF5C164;
	
	return rf_pcm_reg_write(chip, Register, Data);
}

bool ay8910_write_reg(unsigned char Register, unsigned char Data)
{
	Register &= 0x0F;
	
	if (Register >= 0x0E && Register <= 0x0F)	// Port Writes
		return false;
	
	if (! ChDat->AY8910.RegFirst[Register] && Data == ChDat->AY8910.RegData[Register])
		return false;
	
	ChDat->AY8910.RegFirst[Register] = JustTimerCmds;
	ChDat->AY8910.RegData[Register] = Data;
	return true;
}

static bool ymf271_write_fm_reg(YMF271_SLOT* slot, unsigned char Register, unsigned char Data)
{
	if (! slot->RegFirst[Register] && slot->RegData[Register] == Data)
		return false;
	
	slot->RegFirst[Register] = JustTimerCmds;
	slot->RegData[Register] = Data;
	return true;
}

static bool ymf271_write_fm(YMF271_DATA* chip, unsigned char Port, unsigned char Register, unsigned char Data)
{
	YMF271_SLOT *slot;
	unsigned char SlotReg;
	unsigned char SlotNum;
	unsigned char SyncMode;
	unsigned char SyncReg;
	unsigned char RetVal;
	
	if ((Register & 0x03) == 0x03)
		return true;
	
	SlotNum = ((Register & 0x0F) / 0x04 * 0x03) + (Register & 0x03);
	slot = &chip->slots[12 * Port + SlotNum];
	SlotReg = (Register >> 4) & 0x0F;
	if (SlotNum >= 12 || 12 * Port > 48)
		printf("Error");
	
	// check if the register is a synchronized register
	SyncReg = 0;
	switch(SlotReg)
	{
	case  0:
	case  9:
	case 10:
	case 12:
	case 13:
	case 14:
		SyncReg = 1;
		break;
	default:
		break;
	}
	
	// check if the slot is key on slot for synchronizing
	SyncMode = 0;
	switch (chip->groups[SlotNum].sync)
	{
	case 0:		// 4 slot mode
		if (Port == 0)
			SyncMode = 1;
		break;
	case 1:		// 2x 2 slot mode
		if (Port == 0 || Port == 1)
			SyncMode = 1;
		break;
	case 2:		// 3 slot + 1 slot mode
		if (Port == 0)
			SyncMode = 1;
		break;
	default:
		break;
	}

	if (SyncMode && SyncReg)		// key-on slot & synced register
	{
		RetVal = false;
		switch(chip->groups[SlotNum].sync)
		{
		case 0:		// 4 slot mode
			RetVal |= ymf271_write_fm_reg(&chip->slots[(12 * 0) + SlotNum], SlotReg, Data);
			RetVal |= ymf271_write_fm_reg(&chip->slots[(12 * 1) + SlotNum], SlotReg, Data);
			RetVal |= ymf271_write_fm_reg(&chip->slots[(12 * 2) + SlotNum], SlotReg, Data);
			RetVal |= ymf271_write_fm_reg(&chip->slots[(12 * 3) + SlotNum], SlotReg, Data);
			break;
		case 1:		// 2x 2 slot mode
			if (Port == 0)		// Slot 1 - Slot 3
			{
				RetVal |= ymf271_write_fm_reg(&chip->slots[(12 * 0) + SlotNum], SlotReg, Data);
				RetVal |= ymf271_write_fm_reg(&chip->slots[(12 * 2) + SlotNum], SlotReg, Data);
			}
			else				// Slot 2 - Slot 4
			{
				RetVal |= ymf271_write_fm_reg(&chip->slots[(12 * 1) + SlotNum], SlotReg, Data);
				RetVal |= ymf271_write_fm_reg(&chip->slots[(12 * 3) + SlotNum], SlotReg, Data);
			}
			break;
		case 2:		// 3 slot + 1 slot mode
			// 1 slot is handled normally
			RetVal |= ymf271_write_fm_reg(&chip->slots[(12 * 0) + SlotNum], SlotReg, Data);
			RetVal |= ymf271_write_fm_reg(&chip->slots[(12 * 1) + SlotNum], SlotReg, Data);
			RetVal |= ymf271_write_fm_reg(&chip->slots[(12 * 2) + SlotNum], SlotReg, Data);
			break;
		default:
			break;
		}
	}
	else		// write register normally
	{
		RetVal = ymf271_write_fm_reg(&chip->slots[(12 * Port) + SlotNum], SlotReg, Data);
	}
	
	return RetVal;
}

bool ymf271_write(unsigned char Port, unsigned char Register, unsigned char Data)
{
	YMF271_DATA* chip = &ChDat->YMF271;
	YMF271_SLOT* slot;
	YMF271_GROUP* group;
	unsigned char SlotNum;
	unsigned char Addr;
	
	switch(Port)
	{
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
		return ymf271_write_fm(chip, Port, Register, Data);
	case 0x04:
		if ((Register & 0x03) == 0x03)
			return true;
		
		SlotNum = ((Register & 0x0F) / 0x04 * 0x03) + (Register & 0x03);
		Addr = (Register >> 4) % 3;
		slot = &chip->slots[SlotNum * 4];
		
		switch((Register >> 4) & 0x0F)
		{
		case 0:
		case 1:
		case 2:
			slot->startaddr[Addr] = Data;
			return true;
		case 3:
		case 4:
		case 5:
			slot->endaddr[Addr] = Data;
			return true;
		case 6:
		case 7:
		case 8:
			slot->loopaddr[Addr] = Data;
			return true;
		case 9:
			if (! slot->sltnfirst && slot->slotnote == Data)
				return false;
			slot->sltnfirst = JustTimerCmds;
			slot->slotnote = Data;
			break;
		}
		break;
	case 0x06:
		if (! (Register & 0xF0))
		{
			if ((Register & 0x03) == 0x03)
				return true;
			
			SlotNum = ((Register & 0x0F) / 0x04 * 0x03) + (Register & 0x03);
			group = &chip->groups[SlotNum];
			
			if (! group->First && group->Data == Data)
				return false;
			group->First = JustTimerCmds;
			group->Data = Data;
			group->sync = Data & 0x03;
		}
		else
		{
			switch (Register)
			{
			case 0x10:	// Timer A LSB
			case 0x11:	// Timer A MSB
			case 0x12:	// Timer B
			case 0x13:	// Timer A/B Load, Timer A/B IRQ Enable, Timer A/B Reset
				return false;
			case 0x14:
				chip->ext_address[0] = Data;
				return true;
			case 0x15:
				chip->ext_address[1] = Data;
				return true;
			case 0x16:
				chip->ext_address[2] = Data;
				chip->ext_read = Data & 0x80;
				//if (! chip->ext_read)
				//	chip->ext_address[2] = (chip->ext_address[2] + 1) & 0x7FFFFF;
				return true;
			case 0x17:
				//chip->ext_address[2] = (chip->ext_address[2] + 1) & 0x7FFFFF;
				return true;
			}
		}
		break;
	}
	
	return true;
}
