
REM convert WAV files to headerless versions
util\riffstrip.exe xm\samples\snare1.wav 
util\riffstrip.exe xm\samples\bassdrum1.wav 

REM convert headerless WAV files to EWF format
util\pcm2ewf.exe xm\samples\snare1.wav.stripped res\ewf\snare1.ewf
util\pcm2ewf.exe xm\samples\bassdrum1.wav.stripped res\ewf\bassdrum1.ewf

REM clean up
del xm\samples\snare1.wav.stripped
del xm\samples\bassdrum1.wav.stripped

REM convert VGI instruments to EIF instruments

cd util
xm2esf ..\xm\bgm1\bgm1.xif ..\res\bgm\bgm1.esf
pause 