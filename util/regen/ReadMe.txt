================================================================================
			   Regen v0.97 and 0.97D
================================================================================

by AamirM, Copyright (C) 2007-2009

First of all, I would like to thank the following:

Charles MacDonald, Stéphane Dallongeville, Eke, notaz, TascoDeluxe, Steve Snake,
Karl Stenerud, Richard Mitton, Juergen Buchmueller, Tatsuyuki Satoh, Shadow,
Jarek Burczynski, Shay Green, King of Chaos, David Haywood, drx, Igor Pavlov,
Nemesis, Yakovlev Victor, Kaneda, Tiido, Mitsutaka Okazaki, ZLIB team, SDL team,
LibPNG team and all other people who gave suggestions, bug reports, feedback.


================================================================================
				Legal Stuff
================================================================================

Regen is Copyright (C) 2007-2009, AamirM. All rights reserved.

You may use and/or redistribute Regen provided that you :

1) Do not modify and/or alter, in any way, the files included.
2) You distribute all the files you got originally with Regen.
3) You may not sell, lease, rent or otherwise seek to gain monetary profit from
   Regen.
4) You may not distribute Regen with ROM images unless you have the legal right
   to distribute them.
5) You may not use Regen for commercial purposes.

DISCLAIMER: The author of Regen doesn't guarantee its fitness for any purpose,
implied or otherwise, and do not except responsibility for any damages
whatsoever that might occur when using Regen. All games emulated by Regen,
including any images and sounds therein, are copyrighted by their respective
copyright holders. Regen DOES NOT INCLUDE any ROM images of emulated games.

SEGA, Master System, GameGear, Genesis, MegaDrive, SegaCD, MegaCD, and 32X are
trademarks of Sega Enterprises Ltd.  Regen is not authorized by, endorsed by, or
affiliated with Sega Enterprises, Ltd. in any way.


================================================================================
				Introduction
================================================================================

Regen is an emulator which can emulate the following systems with
very high accuracy:

* Sega Genesis/MegaDrive.

* Sega Master System, GameGear, SG-1000, SC-3000.

It also contains many other useful features and stuff including debuggers :) .

You can get the latest version of Regen from:

Website : http://aamirm.hacking-cult.org
Forums  : http://aamirm.hacking-cult.org/forums/

Latest betas are only posted on the forum boards.

If you create language files, please send them to me so I can put them up on my
homepage for everyone to download. Thanks.


================================================================================
				Requirements
================================================================================

To run Regen at a decent speed requires a Pentium III or up with at least 64MB
RAM and Windows9x or higher is needed. With a little tweaking, Regen has been
successfully ran on a 400 Mhz Pentium 2 fullspeed!

Regen requires DirectX 7 or higher. Regen only supports 16 and 32 bit color
depths so your desktop must be set to either one of these.

For sound, Regen requires DirectSound compatible hardware. Sound card is
required.

DirectInput compatible input hardware is needed. Keyboard, mouse and
gamepads/joypads are supported.


================================================================================
				Default Keys
================================================================================

The default keys for Genesis/MegaDrive are:

For Player 1:
Start	--- Enter
Mode	--- Right Shift
A	--- A
B	--- S
C	--- D
X	--- Z
Y	--- X
Z	--- C
Up, Down, Left, Right --- Direction Keys

For Player 2:
Start	--- U
Mode	--- T
A	--- K
B	--- L
C	--- M
X	--- I
Y	--- O
Z	--- P
Up, Down, Left, Right --- Y, H, G, J

When using Mouse:
A	--- Right Button
B	--- Left Button
(all other buttons are mapped on keyboard)

For Master System/Game Gear:

PlayerX Button 1 = PlayerX Key A for Genesis
PlayerX Button 2 = PlayerX Key B for Genesis
Pause            = Player1 Key C for Genesis
Reset            = Player1 Key X for Genesis

Direction Keys are same as Genesis'.

There are some other things to explain as well but its just too much pain in the
ass to write :) . Just press each keys on the keyboard and/or mouse until some
action happens :D . If your still having problems, just ask me.


================================================================================
				Usage Instructions
================================================================================

Here is a half-hearted attempt at documenting the stuff in Regen and how to use
it. First, the menus will be explained and then the different dialog windows.
After that, some other small details and stuff will be explained. In Regen, a
menu item which shows a dialog windows ends with "...". For example,
"Paths and Preferences...".


================================================================================
				Menus and Menu Items
================================================================================

---------
File Menu
---------

Load SMS/GG ROM 	- Load a Master System/Game Gear/SG-1000/SC-3000 ROM.

Load Genesis ROM 	- Load Genesis/Megadrive ROM.

Pause/Resume emulation 	- Toggles pausing of emulation.

Soft Reset 		- Soft resets the machine. In soft reset, some of the
			  machine state isn't cleared (like RAM). It is like the
			  reset button on the console.

Hard Reset 		- Hard resets the machine. In hard reset, all of the
			  things are cleared to a known state (usually zeros).

Power off 		- Powers off the virtual console.

Save State As 		- Save state to user specified file in some location.

Load State As 		- Load state from the user specified file.

Save Slot 		- This is a sub-menu and contain menu items to select
			  one of the ten available slots ten slots. These slots
			  can then be used to quickly save/load a state. These
			  avoid the hassle of having to specify the file name
			  each time you save/load a state.

Quick Save State 	- Quickly save a state to the currently active slot.

Quick Load State 	- Quickly load a state from the currently active slot.

Record Input 		- This feature allows you to record input during the
			  gameplay. This can then be used to recreate the
			  gameplay again. So in effect, its like recording a
			  movie. You will need to select the location where you
			  want to save the recording.

Replay Input 		- Replays the previously recorded input recording file.
			  It will ask you to select the recoding file.

Start AVI Recording 	- Record the movie in AVI format. This feature only
			  works in "Superfast" rendering mode. It requires that
			  you have a VERY FAST computer if you want to record
			  the AVI lag free. It will ask you to select a codec.
			  Currently, the recommended codec is the ZMBV (Zipped
			  Motion Block Video) codec from DosBox project. Its
			  loseless and provides good speed and compression.
			  A ZMBV based codec is currently being developed by
			  myself to provide enough speed for lag-free recording.
			  After you start the recording, pressing this same menu
			  item again will stop the recording.

File History 		- This sub-menu contains list of five most recently
			  loaded ROMs. Useful to quickly load a game if you
			  usually have some favourite game that you play all the
			  time.

Clear History 		- Clears the file history.

Exit 			- Quit from the warm world of Regen and return to the
			  cold world of Windows.

----------
VIDEO Menu
----------

Window Size 		- This sub menu contains three window sizes to select
			  (multiples of 320x240).

Fullscreen Resolution 	- This sub-menu lists all the fullscreen resolutions
			  that are supported by the system.

Enter Fullscreen 	- Toggles fullscreen mode.

VSync 			- Toggles vsync mode. This removes tearing in the video.

Video Plugins 		- This sub menu lists all the plugins that were found.
			  Regen supports the Kega RPI. Plugins should be placed
			  in a folder called "Plugins" where Regen is installed.
			  Upto 32 render plugins are supported.

Turbo Mode 		- Toggles turbo mode. In this mode, Regen will run as
			  fast as it can.

Frame Advance 		- Advance the emulation by one frame only. This is only
			  meaningful while emulation is paused.

Slow Mode 		- This menu is used to slow down the emulation speed.
			  This is useful for TASing (Tool Assisted Speedruns).

Smooth Animation 	- Enables interfram blending with motion blur. Makes
			  games look more realistic and closer to TV.

Scanlines settings 	- Enable scanlines with the corresponding intensity.

Custom Intensity 	- This allows you to set a custom scanline intensity
			  (for example 12%). Predefined scanline settings are
			  much faster. So its foolish to set custom intensity at
			  25, 50, 75 and 100 precents.

Stretch 		- Stretch the visible region to the window area
			  (borders are cut off).

Brighten 		- Brighten the output image. On Genesis, games which use
			  Shadow/Highlight colors, will look correct with this.

Monitor Properties 	- Select the monitor aspect ratio from here. This value
			  is used in aspect ratio correction calculations.

Correct Aspect Ratio 	- Toggles correction of aspect ration.

Output 4:3 Aspect 	- Outputs the image with 4:3 aspect (like real
			  hardware).

Superfast Rendering 	- Enables a fast rendering mode with fast critical
			  emulation path. IT REQUIRES THAT YOU RESTART THE
			  EMULATOR AFTER ENABLING IT. This is meant to be used
			  on slower computers. Enabling this will increase
			  performance quite a bit although other video option
			  will not be available (like video plugins). Turn it on
			  only if you are having speed issues. Leave it off
			  otherwise.

Disable Sprite Limits 	- Disable the Genesis/Megadrive VDP sprite limitations.
			  It does not work for SMS/GG. To disable SMS/GG sprite
			  limitations, see the Master System/Game Gear menu.

Realistic Interlacing 	- Enables realistic interlacing mode. In this mode,
			  Regen will draw odd lines on odd frames and even lines
			  on even frames. But this can cause lots of flickering
			  and some people don't want this so disabling this will
			  remove the flickering.

----------
SOUND Menu
----------

Rate 			- Select the output sampling rate of sound.

Disable Sound 		- Disable sound emulation.

SuperHQ 		- This will enable high quality emulation of the FM and
			  PSG. Enabling this will result in very accurate sound
			  which is very close to the real hardware. The sound
			  chips will be ran at their original rates and then
			  resampled to the selected output rate. Enabling this
			  can cause a little bit of slow down in speed however.

Lowpass Filter 		- Apply lowpass filtering to the sound.

Overdrive 		- Enable/Disable overdriving (amplification) of the
			  final sound ouput before its fed into the sound card.
			  Enabling this can cause the sound to get clipped.

Start WAV Dump 		- Starts logging the output sound in WAV format.

Boost PSG Noise 	- If enabled, boosts the PSG noise channel output. Regen
			  emulates the noise levels accurately by default but in
			  real thing, the filtering by TV or other things can
			  cause the noise to be loud. This option simulates
			  that. It is recommended to turn this setting off.

Volume Levels 		- Change the output volume levels of PSG and FM.

Overdrive Factor 	- Select the final amplification level from here.

Lowpass rolloff 	- Configures the internal filtering applied to sound
			  when in "SuperHQ" mode. So this means it only works
			  when "SuperHQ" is enabled. The other "Lowpass filter"
			  is not affected by this. That applies filtering to the
			  final sound output and is avaialable even when SuperHQ
			  is disabled.

-----------
SYSTEM Menu
-----------

Region 			- Change the region of the virtual console to USA,
			  Europe or Japan or let Regen autodetect it according
			  to the ROM header (recommended). You will need to
			  reload the game after changing the region. It is
			  recommended to leave this setting to "Autodetect".

Autofix Checksum 	- Fix checksum of games with wrong checksum in the
			  headers. This is required for some games to work
			  correctly. This should be disabled for unlicensed
			  games with special handlings.

Use SRAM and EEPROM 	- Enable the loading/saving of SRAM or EEPROM.

Cheats 			- Depending on some other settings, this will either
			  bring up the cheat dialog box or will ask you specify
			  the cheat file for the game and then bring up the
			  cheat dialog. It will bring up the cheat window
			  automatically if a cheat file was found in the
			  configured cheats directory and cheats auto-loading is
			  enabled. Otherwise it will ask you for a cheat file.
			  For automatic loading of cheats, put the cheat files
			  in the configured cheats directory with the same name
			  as the ROM but with a ".dat" extension.

Search Cheats 		- Bring up the cheat searching window.

Patch 			- Depending on some other settings, this will either
			  patch up the ROM with a UPS patch and reset the game
			  or it will ask you for a patch file. It will
			  automatically patch up a ROM if a patch is found in
			  the configured patches directory and auto-patching is
			  enabled. Otherwise it will ask you for a patch file.
			  For automatic patching, the patches should be kept in
			  the configured patches directory with same name as the
			  ROM name but with a ".ups" extension.

Redefine Keys 		- Brings up the controller configuration window.

Reset to defualt keys 	- Resets the controller configuations to default values.

Capture Mouse 		- Captures/Releases the mouse in lightgun/paddle games.

Master System/Game Gear - This menu contains settings for Master System and
			  GameGear. This menu is explained in detail later.

---------
MISC Menu
---------

Paths and Prefernces 	- Brings up the configuration window where you can
			  configure the various paths and other settings.

Always on top 		- Makes the Regen window top most.

Autopause 		- Automatically pause the emulation when Regen loses
			  focus.

Disable Menu hotkeys 	- Disables the menu shortcuts. Useful if you don't use
			  the shortcuts and they conflict with key settings.

High Priority 		- Sets the Regen's process priority to high. If you are
			  experincing jerkiness/stuttering in video/sound try
			  enabling this.

Disable Screensaver 	- Disable screensaver while running games.

Show FPS 		- Show the FPS (Frame Per Seconds) indicator.

Show Messages 		- Show the emulator messages.

Choose Message Font 	- Select the on screen text message font and color.

Alternate Timing 	- Normally Regen uses sound card for timing but some
			  sound cards have problems with this. This causes FPS
			  being jumpy and video/sound being skippy. Enabling
			  this will cause Regen to use other timers for timing
			  to fix that problem. So if you are experiencing those
			  problems try enabling this.

Power Saving Mode 	- Normally Regen will use almost 99-100% CPU to provide
			  smooth audio and video. This is all good when on a
			  desktop PC. But when using Regen on
			  laptop/notebook/netbook, this eats battery life.
			  Enabling this will cause Regen not to use 100% CPU.
			  But this can cause ocassional jumpiness/skips in sound
			  and video.

Select Language 	- Brings up language selection window. See the section
			  on "Localization" for more information.

----------
TOOLS Menu
----------

Screenshot (unfiltered) - Take the raw screenshot without any filters and
			  effects applied.

Screenshot (filtered)   - Take the screenshot of the output image with all the
			  effects and video filters applied. This is the image
			  that you see on the output.

The following features are only enabled in the debugger build of Regen. I don't
explain these features here in much detail since their users already know what
they are and how to use them :) .

68000 Debugger 		- Bring up the M68000 debugger window.

Z80 Debugger 		- Bring up the Z80 debugger window.

ROM Viewer/Editor 	- Bring up the ROM viewer and editor.

RAM Viewer/Editor 	- Bring up the RAM viewer and editor.

Overclock M68000 	- Overclock or underclock the virtual 68k.

FM Scope 		- Shows the oscilloscope display of sound.

YM2612 Debugger 	- Bring up YM2612 (FM) debugger.

Z80 RAM Viewer/Editor 	- Bring up the Z80 RAM viewer and editor.

VDP Layer Select 	- Bring a window to select the different VDP layers.

VDP Debugger 		- Bring up the VDP debugger window.

Spectrum Analyzer 	- Shows the frequency spectrum on the sound output.

---------
HELP Menu
---------


Documentation 		- View this file.

View history 		- Show the history of Regen and its releases.

Regen Homepage 		- Go to the Regen's homepage on the internet.

Regen Forum Borads 	- Go to Regen forum boards for troubleshooting and for
			  talking about all Regen related stuff (like requests).

AamirM's Homepage 	- Go to AamirM's (Regen's author) homepage to see his
			  other emulators and latest developments from him.

About Regen 		- Show the credits and version of Regen.

------------------------------
MASTER SYSTEM / GAME GEAR Menu
------------------------------

Input Port 1,2 		- Select the device to connect to the virtual controller
			  port. Regen automatically detects some of the games
			  with special controllers and selects these controllers
			  automatically.

Enable Keyboard 	- Enable the keyboard emulation for the SC-3000.
			  Enabling this with anything other than SC-3000 can
			  cause controllers to not behave correctly.

Enable YM2413 		- Enable the YM2413 FM sound chip that came on some
			  Master System models and some games use it if present
			  to generate different music. Recommended setting is to
			  leave this on unless you want to hear the PSG music in
			  some game.

3D Glass simulation 	- Simulates the 3D glasses. Leave this off unless a game
			  uses these as it can cause incorrect video output for
			  other games.

Enable BIOS 		- Enable BIOS. Its not really needed. But some games
			  rely on the state brought up by BIOS to work correct
			  (but Regen can do this witout it anyway). You can
			  configure the BIOS path using the "Paths and
			  Prefernces" window.

Sprite Limitations 	- Enables/disables the SMS VDP sprite limitations.


================================================================================
				Dialog Windows
================================================================================

Here, only some of the windows are explained.

-----------
ROM Browser
-----------

ROM browser opens up automatically when you load a ROM and select an archive
(Zip/7zip) file which contain many other files (and maybe more ROMs). It will
list all the files in that archive. Now, you can select your ROM from it by
either double-clicking the file from the list or selecting and pressing "OK".

-------------
Volume Levels
-------------

This dialog allows you to adjust the volume levels of the FM and PSG sound
outputs. Regen will emulate the levels correctly and accurately by default but
some people are picky about the levels between the PSG and FM so they can change
the levels from here. Note however that some games have loud FM levels so
setting this value very high can cause clipped sound. Recommended stting is to
leave it at 1.0 for both. Or keep it between 1.0 and 2.5 and then use the
overdriving feature to ampilfy.

------
Cheats
------

This dialog list the cheats which have been loaded from the cheat file of the
loaded game. This dialog will appear automatically if a cheat file is found in
the configured "Cheats" directory and automatic loading of cheats is enabled.
Otherwise you will have to specify the cheat file to use.

This dialog has two list boxes, main (on the left) and options (on the right).
The main list box lists the cheat name. The options list box lists the different
options in the selected cheat. For example, a main cheat could be "Unlimited
Life" and its options can be "On" or "Off". Another example is where the main
cheat can be "Weapon" and its option can be "Hand gun", "Machine gun", Rocket
Launcher" etc...

This window also includes a small cheat editor. It can be brought up by double-
clicking any of the cheat options. This will then allow you to modify the addess
and data pairs of that cheat.

See the section on "Cheat Files" for information on how to create cheat files.

-------------
Search Cheats
-------------

This window is used for searching cheats. To start searching, you will have to
press "Reset" at least one time so that Regen will start to log the changes.

You can add a cheat by selecting an address from the list box and clicking
"Add Cheat". After a cheat has been added, it will now appear in the "Cheats"
dialog window as well. Added cheats are created using two options "On" and
"Off". Newly added cheats are turned on by default.

Other things should be pretty self explainatory.

---------------------
Paths and Preferneces
---------------------

This window is used for configuration of front-end related things. From here you
can configure various paths and other related settings. Each different setting
are explained here.

* SRAM:
      This allows you to configure the path where the game's Static RAM and/or
      EEPROM files will be save and loaded from. Click "Browse" to select the
      path. By default, Regen creates a "SRAM" directory where it is installed
      and sets this path to point to there.

* Save States:
      This allows you to select where the quick states will be saved to/loaded
      from. Click "Browse" to select the path. By default, Regen creates a
      "Saves" directory where it is installed and sets this path to point to
      there. The "Compressed" feature is currently not implemented.

* UPS Patches:
      This allows you to configure the directry where the UPS patch files will
      be searched for. By default, Regen creates a "Patches" directory and sets
      this path to point to there. It has two further options, "Auto patch" and
      "Ignore CRC errors". "Auto patch" will automatically patch the ROM if a
      UPS patch with same name as ROM but with a ".ups" extension is found in
      the "Patches" directory. "Ignore CRC errors" will ignore any CRC errors
      that occur during the patch.

* Multimedia:
      This allows you to configure the path where the game's sound and/or
      AVI recording files will be save. Click "Browse" to select the path. By
      default, Regen creates a "Sound" directory where it is installed
      and sets this path to point to there.

* Cheats:
      This allows you to configure the directry where the cheat files will
      be searched for. By default, Regen creates a "Cheats" directory and sets
      this path to point to there. It has one further option, "Auto load".
      "Auto load" will automaically load up the cheats if a cheat file is found
      in the configured "Cheats" directory. For automatic loading, the cheat
      file should be of same name as the ROM but with a ".dat" extension.

* Screenshots:
      This alloes you to configure the path where the screenshots will be saved.
      Click "Browse" to select that path. By default, Regen creates a
      "Screenshot" directory where it is installed and sets this path to point
      to there. You can further select whether to save the screenshots in BMP
      format or in PNG format.

* SMS BIOS:
      This allows you to configure the SMS BIOS. It is empty by default.

--------------
68000 Debugger
--------------

This is only available in debugger build of Regen. It should be pretty self
explainatory but there are a few small details.

* Double clicking a line in disassembly window will add it as a PC break point.

* Traces are created at the same location as the ROM file.

* This window automatically opens up if there is an illegal read/write by the
  main processor.


================================================================================
				Cheat Files
================================================================================

The format of the Regen cheat file is like this:

[Cheat Number]
Name=Cheat Name
0=Option 1,address:data
1=Option 2,address:data,address:data
2=Option 3,address:data
3=Option 4,address:data,address:data,address:data
.....

Cheat Number must start from 0. Address/data pairs can be either Pro Action
Replay (PAR), Game Genie or simple hex codes. They can also be intermixed. For
example a cheat option can have a PAR pair, a Game Genie pair and hex pair.
An example is as follows:

[0]
Name=Invincibility
0=Off
1=On,FFC262:0002

[1]
Name=Chaos Emerald Modifier
0=Off
1=None,FF06A3:0000
2=One,FF06A3:0001
3=Two,FF06A3:0002
4=Three,FF06A3:0003
5=Four,FF06A3:0004
6=Five,FF06A3:0005
7=Six,FF06A3:0006
8=Seven,FF06A3:0007

There is a special keyword called "Default" that enables a cheat option
automatically when it is loaded. For example using the above example,

[0]
Name=Invincibility
0=Off
1=On,FFC262:0002
Default=1

This will automatically enable the option number 1 ("On") upon being loaded.

Thats pretty much all there is to it. There is a converter available from my
homepage that can convert from existing PAT files to Regen cheat dats. If you
need a converter from something else, just ask me.


================================================================================
			   Advanced/Other Settings
================================================================================

These setting can only be set by modifying the "Regen.ini" file. Here are these
settings:

* "SoundBufferSize"
	This setting changes the sound buffer size. If you noticing sound/video
	lags/skips, try playing with this value. By default it is 7.

* "ShortHistoryNames"
	Setting this to one will not display the full path to the ROM file but
	instead will only display the ROM name. It is disabled by default.

* "TextColor_R", "TextColor_G", "TextColor_B"
	This allows you to modify the color of the on screen text messages. All
	are set to 255 by default for white color.

* "EmulateLockups"
	Setting this to one will lock up the virtual console if something is
	done by the game that causes the real hardware to lock up as well.
	Disabled by default.

* "LargeROMSpace"
	Setting this to one will allow 8 MB rom space. Disabled by default.


================================================================================
				Start up screen
================================================================================

Regen can show up an image on start up in the main screen. By default, this
image is Regen's logo type thing created by me in my "Photoshop" (aka MS Paint).
But you can replace this image by any other image. Just rename the image to
"Logo.png" and put it in the folder where Regen is installed.


================================================================================
				Translating Regen
================================================================================

Here are the steps you need to follow to translate Regen:

1) You need to edit Regen.rc. You can either use Visual Studio's resource
   editor or any other editor and translate all the strings, menus, dialogs
   etc.. in it. Please do not change the order of the menu items.

2) After editing, you need to compile it. A Visual Studio .NET 2003 project
   is provided in the folder "Template". A makefile to build using gcc/mingw is
   also provided. You can also use the provided Visual Studio project
   file with Visual Studio 2005 and Visual Studio 2008 (they will convert
   it automatically). You can also use any other compiler (for example
   gcc/mingw). After, compiling, it will produce "Satellite.dll".

3) Create a folder with the name of language ID, to which you are translating,
   in the "Languages" folder where Regen is installed. Place "Satellite.dll"
   in that folder. You can use the link given below to find out the language ID
   for your language. For example, if I am translating to Japanese, I will
   create a folder like this:

   [Regen Folder]\Languages\1041\Satellite.dll
                             /\
                             ||
                         Language ID

4) Start up Regen and go to "Misc->Select Language". If everything went right,
   your language should now appear in the list. Select the language and press
   "OK".

Important Note
--------------

* Do not change the order of the menus/menu items.

* Do not remove "\n" and things like "%s" and %d" etc.. from a string etc..

* Make sure the version of the DLL matches with the version of Regen.
  To check this, FILEVERSION, FileVersion, PRODUCTVERSION and ProductVersion
  in Regen.rc and Regen should match. If the versions do not match,
  Regen will not load it.

Language IDs
------------

To find out the language ID for your language, go to:

http://msdn.microsoft.com/en-us/goglobal/bb964664.aspx


================================================================================
				Command Line
================================================================================

You can also load games from the command line. The syntax is:

	regen [--fullscreen] game_name


================================================================================
				   Contact
================================================================================

Bugs, feature request, and any thing emulation related is welcomed.
But ROM requests will be ignored.
Please do read this Readme completely (yes, including the history file)
before mailing question(s) as many of the answers (and limitations, bugs)
are mentioned here.

You can report bugs, request features at any of the following:

Website : http://aamirm.hacking-cult.org
Forums  : http://aamirm.hacking-cult.org/forums/

You can also contact me at "aamir dot m at live dot com"

where dot = .
      at  = @

Thanks for trying my emulator and stay safe,

AamirM





