@echo off
REM convert raw 8-bit PCM files to EWF format
echo ---------------------------------------------------------------------
echo Converting samples to EWF
echo ---------------------------------------------------------------------
util\pcm2ewf.exe xm\rawsamples\snare1.raw res\ewf\snare1.ewf
util\pcm2ewf.exe xm\rawsamples\snare2.raw res\ewf\snare2.ewf
util\pcm2ewf.exe xm\rawsamples\bassdrum1.raw res\ewf\bassdrum1.ewf
util\pcm2ewf.exe xm\rawsamples\hat1.raw res\ewf\hat1.ewf

REM convert VGI instruments to EIF instruments
echo ---------------------------------------------------------------------
echo Converting VGI patches to EIF
echo ---------------------------------------------------------------------
util\vgi2eif.exe xm\inst\bass1.vgi res\eif\bass1.eif
util\vgi2eif.exe xm\inst\bass2.vgi res\eif\bass2.eif
util\vgi2eif.exe xm\inst\sqchords1.vgi res\eif\sqchords1.eif
util\vgi2eif.exe xm\inst\sawlead.vgi res\eif\sawlead.eif
util\vgi2eif.exe xm\inst\saw2.vgi res\eif\saw2.eif
util\vgi2eif.exe xm\inst\saw2at.vgi res\eif\saw2at.eif

REM Now for the actual arrangements
echo ---------------------------------------------------------------------
echo Beginning xm2esf conversions
echo ---------------------------------------------------------------------
cd util
xm2esf ..\xm\bgm1\bgm1.xif ..\res\bgm\bgm1.esf
xm2esf ..\xm\bgm2\bgm2.xif ..\res\bgm\bgm2.esf
cd ..
pause 
