DLL void  segapsg_write_stereo(void *chip,unsigned char data);
DLL void  segapsg_write_register(void *chip,unsigned char data);
DLL void  segapsg_render(void *chip,int *buffer,int samples,bool add);
DLL void  segapsg_set_gain(void *chip,int gain);
DLL void* segapsg_init(int base_clock,int rate,bool neg);
DLL void  segapsg_shutdown(void *chip);
DLL void  segapsg_set_mute(void *chip,int mask);
DLL float segapsg_get_channel_volume(void *chip,int channel);
DLL const char* segapsg_about(void);