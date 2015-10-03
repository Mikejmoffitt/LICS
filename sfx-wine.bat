@echo off
REM Sound effects
echo ---------------------------------------------------------------------
echo Beginning xm2esf conversions
echo ---------------------------------------------------------------------
cd util
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_jump.xif Z:\home\moffitt\Development\LICS\res\sfx\jump.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_walk1.xif Z:\home\moffitt\Development\LICS\res\sfx\walk1.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_walk2.xif Z:\home\moffitt\Development\LICS\res\sfx\walk2.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_cubetoss.xif Z:\home\moffitt\Development\LICS\res\sfx\cubetoss.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_cubebounce.xif Z:\home\moffitt\Development\LICS\res\sfx\cubebounce.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_fizzle.xif Z:\home\moffitt\Development\LICS\res\sfx\fizzle.esf
REM clean up the working dir (oh god)
cd ..
