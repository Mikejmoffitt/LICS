CC = m68k-elf-gcc
AS = m68k-elf-as
OBJC = m68k-elf-objcopy
LD = m68k-elf-ld
RM = rm -f
ASMZ80 = zasm
BINTOS = bintos
PCMTORAW = pcmtoraw
WAVTORAW = wavtoraw
NM = nm
NM2WCH = nm2wch
SIZEBND = sizebnd
MKISOFS = mkisofs
RESCOMP= rescomp

SCD_LOADER = scd/LukeProjectCD

OPTION = -O1 -std=c99 -Wall 
INCS = -I$(GENDEV)/m68k-elf/include -I$(GENDEV)/m68k-elf/m68k-elf/include -Isrc -Ires -Iinc -Iinc/objects -Iinc/system
CCFLAGS = $(OPTION) -m68000 -c -fomit-frame-pointer -fno-builtin
HWCCFLAGS = $(OPTION) -m68000 -c -fomit-frame-pointer -fno-builtin
Z80FLAGS = -vb2
ASFLAGS = -m68000 --register-prefix-optional
#LIBS =  -L$(GENDEV)/m68k-elf/lib -L$(GENDEV)/m68k-elf/lib/gcc/m68k-elf/4.8.2 -L$(GENDEV)/m68k-elf/m68k-elf/lib -lmd -lc -lgcc -lnosys -lm 
LIBS =  -L$(GENDEV)/m68k-elf/lib -L$(GENDEV)/m68k-elf/lib/gcc/m68k-elf/* -L$(GENDEV)/m68k-elf/m68k-elf/lib -lmd -lnosys 
LINKFLAGS = -T $(GENDEV)/ldscripts/sgdk.ld -nostdlib 
SCDLINKFLAGS = -T scd/mdcd.ld -nostdlib 
ARCHIVES = $(GENDEV)/m68k-elf/lib/libmd.a $(GENDEV)/m68k-elf/lib/gcc/m68k-elf/*/libgcc.a 

RESOURCES=
BOOT_RESOURCES=

BOOTSS=$(wildcard boot/*.s)
BOOT_RESOURCES+=$(BOOTSS:.s=.o)

SCDBOOTSS=$(wildcard scd/*.s)
SCDBOOT_RESOURCES=$(SCDBOOTSS:.s=.o)

#BMPS=$(wildcard res/*.bmp)
#VGMS=$(wildcard res/*.vgm)
#RAWS=$(wildcard res/*.raw)
#PCMS=$(wildcard res/*.pcm)
#MVSS=$(wildcard res/*.mvs)
#TFDS=$(wildcard res/*.tfd)
#WAVS=$(wildcard res/*.wav)
RESS=$(wildcard res/*.res)
#WAVPCMS=$(wildcard res/*.wavpcm)
#BMPS+=$(wildcard *.bmp)
#VGMS+=$(wildcard *.vgm)
#RAWS+=$(wildcard *.raw)
#PCMS+=$(wildcard *.pcm)
#MVSS+=$(wildcard *.mvs)
#TFDS+=$(wildcard *.tfd)
#WAVS+=$(wildcard *.wav)
#RESS+=$(wildcard *.res)
#WAVPCMS+=$(wildcard *.wavpcm)

#RESOURCES+=$(BMPS:.bmp=.o)
#RESOURCES+=$(VGMS:.vgm=.o)
#RESOURCES+=$(RAWS:.raw=.o)
#RESOURCES+=$(PCMS:.pcm=.o)
#RESOURCES+=$(MVSS:.mvs=.o)
#RESOURCES+=$(TFDS:.tfd=.o)
#RESOURCES+=$(WAVS:.wav=.o)
#RESOURCES+=$(WAVPCMS:.wavpcm=.o)
RESOURCES+=$(RESS:.res=.o)

CS=$(wildcard src/*.c) $(wildcard src/*/*.c)
SS=$(wildcard src/*.s) $(wildcard src/*/*.s)
S80S=$(wildcard src/*.s80) $(wildcard src/*/*.s80)
# CS+=$(wildcard *.c)
SS+=$(wildcard *.s)
S80S+=$(wildcard *.s80)
RESOURCES+=$(CS:.c=.o)
RESOURCES+=$(SS:.s=.o)
RESOURCES+=$(S80S:.s80=.o)

OBJS = $(RESOURCES)

.PHONY: all clean mednafen debug gens

all: lyle.bin 

mednafen: lyle.bin
	@exec mednafen $< 2> /dev/null

debug: lyle.bin
	@exec gens $< 2> /dev/null

gens: lyle.bin
	@exec gens $< 2> /dev/null

regen: lyle.bin
	@exec wine ./util/regen/Regen.exe ../../lyle.bin

fusion: lyle.bin
	@exec util/Fusion $< 2> /dev/null

kega: lyle.bin
	/Applications/Kega\ Fusion.app/Contents/MacOS/Kega\ Fusion $< 2> /dev/null

test: lyle.bin
	@exec util/megaloader/megaloader md $< /dev/ttyUSB0 2> /dev/null

test32: lyle.bin
	@exec util/megaloader/mega32 md $< /dev/ttyUSB0 2> /dev/null

boot/sega.o: boot/rom_head.bin
	@$(AS) $(ASFLAGS) boot/sega.s -o $@

scd/segacd.o: 
	@$(AS) $(ASFLAGS) scd/segacd.s -o $@


out.iso: out.elf_scd
	#
	# Create a sega cd image.  Limited to 256K or smaller Roms
	#
	$(NM) -n -S -t x out.elf_scd > out.nm
	$(OBJC) -O binary out.elf_scd lyle.bin
	$(SIZEBND) lyle.bin -sizealign 131072   
	$(OBJC) -O binary out.elf_scd $(SCD_LOADER)/_filesystem/M_INIT.PRG
	$(SIZEBND) $(SCD_LOADER)/_filesystem/M_INIT.PRG -sizealign 131072    
	$(MKISOFS) -iso-level 1 -o $(SCD_LOADER)/filesystem.img -pad $(SCD_LOADER)/_filesystem
	tail -c +32769 $(SCD_LOADER)/filesystem.img > $(SCD_LOADER)/filesystem.bin
	$(RM) -f $(SCD_LOADER)/filesystem.img
	cd $(SCD_LOADER) && $(AS) $(ASFLAGS) -M -ahlsm=listing.asm  main-us-as.asm -o out.iso
	tail -c +53 $(SCD_LOADER)/out.iso > out.iso
	$(RM) -f $(SCD_LOADER)/filesystem.bin

%.bin: %.elf
	@$(OBJC) -O binary $< temp.bin
	@dd if=temp.bin of=$@ bs=8k conv=sync
	@$(RM) temp.bin

%.elf: $(OBJS) $(BOOT_RESOURCES)
	@$(CC) -o $@ $(LINKFLAGS) $(BOOT_RESOURCES) $(ARCHIVES) $(OBJS) $(LIBS)

%.elf_scd: $(OBJS) $(SCDBOOT_RESOURCES)
	@$(CC) -o $@ $(SCDLINKFLAGS) $(SCDBOOT_RESOURCES) $(ARCHIVES) $(OBJS) $(LIBS)

%.o80: %.s80
	@$(ASMZ80) $(Z80FLAGS) -o $@ $<

%.c: %.o80
	@$(BINTOS) $<

%.o: %.c
	@echo "$<"
	@$(CC) $(CCFLAGS) $(INCS) -c $< -o $@ 2>&1 | python3 ./gccerrc.py

%.o: %.s 
	@echo "$<"
	@$(AS) $(ASFLAGS) $< -o $@

%.s: %.bmp
	@bintos -bmp $<

%.rawpcm: %.pcm
	@$(PCMTORAW) $< $@

%.raw: %.wav
	@$(WAVTORAW) $< $@ 16000

%.pcm: %.wavpcm
	@$(WAVTORAW) $< $@ 22050

#%.tfc: %.tfd
#	$(TFMCOM) $<

#%.o80: %.s80
#	$(ASMZ80) $(FLAGSZ80) $< $@ out.lst

%.s: %.tfd
	@$(BINTOS) -align 32768 $<

%.s: %.mvs
	@$(BINTOS) -align 256 $<

%.s: %.esf
	@$(BINTOS) -align 32768 $<

%.s: %.eif
	@$(BINTOS) -align 256 $<

%.s: %.vgm 
	@$(BINTOS) -align 256 $<

%.s: %.raw
	@$(BINTOS) -align 256 -sizealign 256 $<

%.s: %.rawpcm
	@$(BINTOS) -align 128 -sizealign 128 -nullfill 136 $<

%.s: %.rawpcm
	@$(BINTOS) -align 128 -sizealign 128 -nullfill 136 $<

%.s: %.res
	@ $(RESCOMP) $< $@ > /dev/null

boot/rom_head.bin: boot/rom_head.o
	@$(LD) $(LINKFLAGS) --oformat binary -o $@ $<
	

clean:
	$(RM) $(RESOURCES)
	$(RM) *.o *.bin *.elf *.elf_scd *.map *.iso
	$(RM) boot/*.o boot/*.bin
	$(RM) $(RESS:.res=.h) $(RESS:.res=.s)
