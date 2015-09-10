#################################################
XM2ESF  -  Convert XM files to echo stream format
#################################################

Written in	Power BASIC Console Compiler
Platform	Win32

To use this at all, you are going to need ECHO:

http://github.com/sikthehedgehog/Echo

That said, this program is the only program so far
to allow creating music and sfx for the echo
sound engine.

XM2ESF - Convert XM to Echo Stream Format
*** ALPHA VERSION ***

Copyright (C) 2011 Oerg866 

Echo (C) 2011 Sik      http://echo.mdscene.net (Website N/A yet)

PLEASE REPORT BUGS TO GITHUB OR oerg866@tototek.com!!

This program is freeware. It must not be sold. This is a temporary
disclaimer until we actually bother setting up license stuffs =P

Please take a few seconds to acknowledge that this
Software is currently in alpha state, and may or may not
Function correctly. You hereby agree that its creator can not
be held liable in case anything goes wrong.

Also, lots of hours of bugfixing and testing went into this.
Please show a little compassion or something when using this
program :)

------------------------

usage: xm2esf <infile> <outfile>

<infile>  is the pointer to an XIF file. An XIF file is a descriptor
          file that contains all the parameters which xm2esf will
          respect while creating the resulting ESF file, and also the
          path and filename to the input XM file.

          Creating an XIF file is possible using 'xm2esfgui' by
          Oerg866. It is very easy to use and creates a XIF file
          that is perfectly parseable by xm2esf :)

<outfile> is the output path ad filename xm2esf will use to create
          the resulting ESF file. Please note that if this file exists
          it WILL BE OVERWRITTEN, even if you encounter a bug and the
          conversion somehow fails (well.. it shouldn't :P but just
          keep in mind to back up often while xm2esf is in alpha stage

Bye :D!
