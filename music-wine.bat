@echo off
REM removing old object files
del out\res\bgm.o /S /Q
del out\res\eif.o /S /Q
del out\res\eef.o /S /Q
del out\res\ewf.o /S /Q

REM convert raw 8-bit PCM files to EWF format
echo ---------------------------------------------------------------------
echo Converting samples to EWF
echo ---------------------------------------------------------------------
util\pcm2ewf.exe xm\rawsamples\snare1.raw res\ewf\snare1.ewf
util\pcm2ewf.exe xm\rawsamples\snare2.raw res\ewf\snare2.ewf
util\pcm2ewf.exe xm\rawsamples\snare3.raw res\ewf\snare3.ewf
util\pcm2ewf.exe xm\rawsamples\bassdrum1.raw res\ewf\bassdrum1.ewf
util\pcm2ewf.exe xm\rawsamples\hat1.raw res\ewf\hat1.ewf

REM convert VGI instruments to EIF instruments
echo ---------------------------------------------------------------------
echo Converting VGI patches to EIF
echo ---------------------------------------------------------------------
util\vgi2eif.exe xm\inst\bass1.vgi res\eif\bass1.eif
util\vgi2eif.exe xm\inst\bass2.vgi res\eif\bass2.eif
util\vgi2eif.exe xm\inst\sawlead.vgi res\eif\sawlead.eif
util\vgi2eif.exe xm\inst\sqchords1.vgi res\eif\sqchords1.eif
util\vgi2eif.exe xm\inst\saw2.vgi res\eif\saw2.eif
util\vgi2eif.exe xm\inst\saw2at.vgi res\eif\saw2at.eif
util\vgi2eif.exe xm\inst\buzz.vgi res\eif\buzz.eif
util\vgi2eif.exe xm\inst\buzztar.vgi res\eif\buzztar.eif
util\vgi2eif.exe xm\inst\buzztarc1.vgi res\eif\buzztarc1.eif
util\vgi2eif.exe xm\inst\drone.vgi res\eif\drone.eif
util\vgi2eif.exe xm\inst\unsettling.vgi res\eif\unsettling.eif

echo ---------------------------------------------------------------------
echo Beginning xm2esf conversions
echo ---------------------------------------------------------------------
cd util
xm2esf Z:\home\moffitt\Development\LICS\xm\bgm1\bgm1.xif Z:\home\moffitt\Development\LICS\res\bgm\bgm1.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\bgm2\bgm2.xif Z:\home\moffitt\Development\LICS\res\bgm\bgm2.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\bgm3\bgm3.xif Z:\home\moffitt\Development\LICS\res\bgm\bgm3.esf
xm2esf Z:\home\moffitt\Development\LICS\xm\bgm4\bgm4.xif Z:\home\moffitt\Development\LICS\res\bgm\bgm4.esf

REM clean up the working dir (oh god)
cd ..
