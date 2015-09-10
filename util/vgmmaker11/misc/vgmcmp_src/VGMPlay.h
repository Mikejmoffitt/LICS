// Header file for VGM file handling

typedef struct _vgm_file_header
{
	unsigned long int fccVGM;
	unsigned long int lngEOFOffset;
	unsigned long int lngVersion;
	unsigned long int lngHzPSG;
	unsigned long int lngHzYM2413;
	unsigned long int lngGD3Offset;
	unsigned long int lngTotalSamples;
	unsigned long int lngLoopOffset;
	unsigned long int lngLoopSamples;
	unsigned long int lngRate;
	unsigned short int shtPSG_Feedback;
	unsigned char bytPSG_SRWidth;
	unsigned char bytPSG_Flags;
	unsigned long int lngHzYM2612;
	unsigned long int lngHzYM2151;
	unsigned long int lngDataOffset;
	unsigned long int lngHzSPCM;
	unsigned long int lngSPCMIntf;
	unsigned long int lngHzRF5C68;
	unsigned long int lngHzYM2203;
	unsigned long int lngHzYM2608;
	unsigned long int lngHzYM2610;
	unsigned long int lngHzYM3812;
	unsigned long int lngHzYM3526;
	unsigned long int lngHzY8950;
	unsigned long int lngHzYMF262;
	unsigned long int lngHzYMF278B;
	unsigned long int lngHzYMF271;
	unsigned long int lngHzYMZ280B;
	unsigned long int lngHzRF5C164;
	unsigned long int lngHzPWM;
	unsigned long int lngHzAY8910;
	unsigned char bytAYType;
	unsigned char bytAYFlag;
	unsigned char bytAYFlagYM2203;
	unsigned char bytAYFlagYM2608;
	unsigned char bytVolumeModifier;
	unsigned char bytReserved2;
	signed char bytLoopBase;
	unsigned char bytLoopModifier;
} VGM_HEADER;
typedef struct _vgm_gd3_tag
{
	unsigned long int fccGD3;
	unsigned long int lngVersion;
	unsigned long int lngTagLength;
	wchar_t* strTrackNameE;
	wchar_t* strTrackNameJ;
	wchar_t* strGameNameE;
	wchar_t* strGameNameJ;
	wchar_t* strSystemNameE;
	wchar_t* strSystemNameJ;
	wchar_t* strAuthorNameE;
	wchar_t* strAuthorNameJ;
	wchar_t* strReleaseDate;
	wchar_t* strCreator;
	wchar_t* strNotes;
} GD3_TAG;
typedef struct _vgm_pcm_bank_data
{
	unsigned long int DataSize;
	unsigned char* Data;
	unsigned long int DataStart;
} VGM_PCM_DATA;
typedef struct _vgm_pcm_bank
{
	unsigned long int BankCount;
	VGM_PCM_DATA* Bank;
	unsigned long int DataSize;
	unsigned long int DataPos;
} VGM_PCM_BANK;

#define FCC_VGM	0x206D6756	// 'Vgm '
#define FCC_GD3	0x20336447	// 'Gd3 '
