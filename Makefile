CC = m68k-elf-gcc
AS = m68k-elf-as
OBJC = m68k-elf-objcopy
LD = m68k-elf-ld
RM = rm -f
ASMZ80 = zasm
BINTOS = bintos
RESCOMP= rescomp

ECHO = printf

OPTION = -O1 -std=c99 -Wall
INCS = -I$(GENDEV)/m68k-elf/include -I$(GENDEV)/m68k-elf/m68k-elf/include -Isrc -Ires -Iinc -Iinc/objects -Iinc/system
CCFLAGS = $(OPTION) -m68000 -c -fomit-frame-pointer -fno-builtin -Wno-overflow
Z80FLAGS = -vb2
ASFLAGS = -m68000 --register-prefix-optional
LIBS =  -L$(GENDEV)/m68k-elf/lib -L$(GENDEV)/m68k-elf/lib/gcc/m68k-elf/* -L$(GENDEV)/m68k-elf/m68k-elf/lib -lmd -lnosys -lgcc
LINKFLAGS = -T $(GENDEV)/ldscripts/sgdk.ld -nostdlib
ARCHIVES = $(GENDEV)/m68k-elf/lib/libmd.a $(GENDEV)/m68k-elf/lib/gcc/m68k-elf/*/libgcc.a

RESOURCES=
BOOT_RESOURCES=

BOOTSS=$(wildcard boot/*.s)
BOOT_RESOURCES+=$(BOOTSS:.s=.o)

RESS=$(wildcard res/*.res)
RESOURCES+=$(RESS:.res=.o)
CS=$(wildcard src/*.c) $(wildcard src/*/*.c)
SS=$(wildcard src/*.s) $(wildcard src/*/*.s)
S80S=$(wildcard src/*.s80) $(wildcard src/*/*.s80)
SS+=$(wildcard *.s)
S80S+=$(wildcard *.s80)

RESOURCES+=$(CS:.c=.o)
RESOURCES+=$(SS:.s=.o)
RESOURCES+=$(S80S:.s80=.o)

OBJS = $(RESOURCES)

.PHONY: all clean

all: lyle.bin

gens: lyle.bin
	@exec gens $< 2> /dev/null

regen: lyle.bin
	@exec wine ./util/regen/Regen.exe ../../lyle.bin

kmod: lyle.bin
	@exec wine ./util/kmod/gens.exe lyle.bin

umdk: lyle.bin
	@exec util/umdkv2-bin/loader -w $<:0 -x 2

fusion: lyle.bin
	@exec util/Fusion $< 2> /dev/null

test: lyle.bin
	@exec util/megaloader/megaloader md $< /dev/ttyUSB0 2> /dev/null

test32: lyle.bin
	@exec util/megaloader/mega32 md $< /dev/ttyUSB0 2> /dev/null

boot/sega.o: boot/rom_head.bin
	@$(AS) $(ASFLAGS) boot/sega.s -o $@

%.bin: %.elf
	@$(OBJC) -O binary $< temp.bin
	@bash -c '$(ECHO) "\e[32m"'
	@dd if=temp.bin of=$@ bs=8k conv=sync
	@rm temp.bin
	@bash -c '$(ECHO) "\e[0m"'

%.elf: $(OBJS) $(BOOT_RESOURCES)
	@$(CC) -n -o $@ $(LINKFLAGS) $(BOOT_RESOURCES) $(ARCHIVES) $(OBJS) $(LIBS)

%.o80: %.s80
	@$(ASMZ80) $(Z80FLAGS) -o $@ $<

%.c: %.o80
	@$(BINTOS) $<

%.o: %.c
	@bash -c '$(ECHO) "\t\e[96m[ CC ]\e[0m $<\n"'
	@$(CC) $(CCFLAGS) $(INCS) -c $< -o $@ 2>&1 | python3 ./gccerrc.py

%.o: %.s
	@bash -c '$(ECHO) "\t\e[94m[ AS ]\e[0m $<\n"'
	@$(AS) $(ASFLAGS) $< -o $@

%.s: %.esf
	@$(BINTOS) -align 32768 $<

%.s: %.eif
	@$(BINTOS) -align 256 $<

%.s: %.res
	@ $(RESCOMP) $< $@ > /dev/null

boot/rom_head.bin: boot/rom_head.o
	@$(LD) $(LINKFLAGS) --oformat binary -o $@ $<

clean:
	@$(RM) $(RESOURCES)
	@$(RM) *.o *.bin *.elf *.map
	@$(RM) boot/*.o boot/*.bin
	@$(RM) $(RESS:.res=.h) $(RESS:.res=.s)
