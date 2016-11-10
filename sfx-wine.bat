@echo off
REM Sound effects
echo ---------------------------------------------------------------------
echo Beginning xm2esf conversions
echo ---------------------------------------------------------------------
cd util
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_elevatormove.xif Z:\home\moffitt\Development\LICS\res\sfx\elevatormove.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_bogologo.xif Z:\home\moffitt\Development\LICS\res\sfx\bogologo.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_powerup_get.xif Z:\home\moffitt\Development\LICS\res\sfx\powerup_get.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_cubelift.xif Z:\home\moffitt\Development\LICS\res\sfx\cubelift.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_hurt.xif Z:\home\moffitt\Development\LICS\res\sfx\hurt.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_pause.xif Z:\home\moffitt\Development\LICS\res\sfx\pause.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_jump.xif Z:\home\moffitt\Development\LICS\res\sfx\jump.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_walk1.xif Z:\home\moffitt\Development\LICS\res\sfx\walk1.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_walk2.xif Z:\home\moffitt\Development\LICS\res\sfx\walk2.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_cubetoss.xif Z:\home\moffitt\Development\LICS\res\sfx\cubetoss.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_cubebounce.xif Z:\home\moffitt\Development\LICS\res\sfx\cubebounce.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_fizzle.xif Z:\home\moffitt\Development\LICS\res\sfx\fizzle.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_cubespawn.xif Z:\home\moffitt\Development\LICS\res\sfx\cubespawn.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_enemyexplode.xif Z:\home\moffitt\Development\LICS\res\sfx\enemyexplode.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_enemystrike.xif Z:\home\moffitt\Development\LICS\res\sfx\enemystrike.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\sfx\sfx_cubeexplode.xif Z:\home\moffitt\Development\LICS\res\sfx\cubeexplode.esf
REM clean up the working dir (oh god)
cd ..
