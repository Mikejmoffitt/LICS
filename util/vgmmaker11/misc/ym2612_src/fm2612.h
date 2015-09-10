DLL void* ym2612_init(int baseclock,int rate);
DLL void  ym2612_shutdown(void *chip);
DLL void  ym2612_reset(void *chip);
DLL void  ym2612_render(void *chip, int *buffer,int length, bool add);
DLL int   ym2612_write(void *chip, int a, unsigned char v);
DLL void  ym2612_set_mute(void *chip, int mute);
DLL float ym2612_get_channel_volume(void *chip,int chn);
DLL const char* ym2612_about(void);
