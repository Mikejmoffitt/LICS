Lyle in Cube Sector v1.2
========================

V1.2? What?
-----------
This is a conversion of the freeware PC game Lyle in Cube Sector to the Sega Genesis / Mega Drive platform. I say conversion, not port, because it is a complete reimplementation of the original software's logic in C. The original was created using Clickteam Multimedia Fusion. 

I am considering this another version, higher numbered than the existing PC ones, as once it is complete I hope it will be at least as good if not better. With a little re-tooling of hardware interaction, porting to other platforms natively would not be difficult.

Considerations of Language and Dependencies
------------------------------
This project is written in C99, and is built using the SGDK toolchain. All game logic is written in C with original structures and routines, and SGDK is used for interaction with the Sega hardware. The provided Makefile is for use on Linux / Mac OS X systems which have SGDK set up (in /opt/toolchains/gen) but a Makefile (winmake) is provided for use on Windows when absolutely needed.

While common C practices promote the use of a relatively size-agnostic "unsigned int" type, here "u16", a 16-bit word shorthand is used. This is because the Mega Drive (from here on "MD") has as its CPU the Motorola 68000. While registers are 32-bit, both RAM and ROM are 16-bit. As a result anything above 16-bit words will take nearly double the time to be manipulated in memory. When approprite, u32 is used in faith that GCC will place it in a 32-bit register and not use memory. Results vary strongly across different optimization levels and versions of GCC in use, so unfortunately this sort of manual semi-optimization has become important.

The Echo sound engine (by Sik) is used for playback of BGM and SFX.
