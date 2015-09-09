	.file	"save.c"
| GNU C (GCC) version 4.8.2 (m68k-elf)
|	compiled by GNU C version 4.9.3, GMP version 5.0.5, MPFR version 2.4.2, MPC version 0.8.2
| GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
| options passed:  -I /opt/toolchains/gen/m68k-elf/include
| -I /opt/toolchains/gen/m68k-elf/m68k-elf/include -I src -I res -I inc
| src/save.c -mcpu=68000 -O2 -std=c99 -fomit-frame-pointer -fno-builtin
| -fverbose-asm
| options enabled:  -faggressive-loop-optimizations -fauto-inc-dec
| -fbranch-count-reg -fcaller-saves -fcombine-stack-adjustments -fcommon
| -fcompare-elim -fcprop-registers -fcrossjumping -fcse-follow-jumps
| -fdefer-pop -fdelete-null-pointer-checks -fdevirtualize -fdwarf2-cfi-asm
| -fearly-inlining -feliminate-unused-debug-types -fexpensive-optimizations
| -fforward-propagate -ffunction-cse -fgcse -fgcse-lm -fgnu-runtime
| -fguess-branch-probability -fhoist-adjacent-loads -fident -fif-conversion
| -fif-conversion2 -findirect-inlining -finline -finline-atomics
| -finline-functions-called-once -finline-small-functions -fipa-cp
| -fipa-profile -fipa-pure-const -fipa-reference -fipa-sra
| -fira-hoist-pressure -fira-share-save-slots -fira-share-spill-slots
| -fivopts -fkeep-static-consts -fleading-underscore -fmath-errno
| -fmerge-constants -fmerge-debug-strings -fmove-loop-invariants
| -fomit-frame-pointer -foptimize-register-move -foptimize-sibling-calls
| -foptimize-strlen -fpartial-inlining -fpeephole -fpeephole2
| -fprefetch-loop-arrays -freg-struct-return -fregmove -freorder-blocks
| -freorder-functions -frerun-cse-after-loop
| -fsched-critical-path-heuristic -fsched-dep-count-heuristic
| -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic
| -fsched-rank-heuristic -fsched-spec -fsched-spec-insn-heuristic
| -fsched-stalled-insns-dep -fshow-column -fshrink-wrap -fsigned-zeros
| -fsplit-ivs-in-unroller -fsplit-wide-types -fstrict-aliasing
| -fstrict-overflow -fstrict-volatile-bitfields -fsync-libcalls
| -fthread-jumps -ftoplevel-reorder -ftrapping-math -ftree-bit-ccp
| -ftree-builtin-call-dce -ftree-ccp -ftree-ch -ftree-coalesce-vars
| -ftree-copy-prop -ftree-copyrename -ftree-dce -ftree-dominator-opts
| -ftree-dse -ftree-forwprop -ftree-fre -ftree-loop-if-convert
| -ftree-loop-im -ftree-loop-ivcanon -ftree-loop-optimize
| -ftree-parallelize-loops= -ftree-phiprop -ftree-pre -ftree-pta
| -ftree-reassoc -ftree-scev-cprop -ftree-sink -ftree-slp-vectorize
| -ftree-slsr -ftree-sra -ftree-switch-conversion -ftree-tail-merge
| -ftree-ter -ftree-vect-loop-version -ftree-vrp -funit-at-a-time
| -fverbose-asm -fzero-initialized-in-bss -mstrict-align

	.text
	.align	2
	.globl	save_write
	.type	save_write, @function
save_write:
	move.l %a3,-(%sp)	|,
	move.l %a2,-(%sp)	|,
	jsr SRAM_enable	|
	lea sram,%a2	|, ivtmp.11
	lea SRAM_writeWord,%a3	|, tmp56
.L3:
	move.l %a2,%d0	| ivtmp.11, D.2560
	sub.l #sram,%d0	|, D.2560
	moveq #0,%d1	| D.2558
	move.w (%a2)+,%d1	| MEM[base: _17, offset: 0B], D.2558
	move.l %d1,-(%sp)	| D.2558,
	move.l %d0,-(%sp)	| D.2560,
	jsr (%a3)	| tmp56
	addq.l #8,%sp	|,
	cmp.l #sram+698,%a2	|, ivtmp.11
	jne .L3	|
	move.l (%sp)+,%a2	|,
	move.l (%sp)+,%a3	|,
	jra SRAM_disable	|
	.size	save_write, .-save_write
	.align	2
	.globl	save_clear
	.type	save_clear, @function
save_clear:
	lea sram,%a0	|, ivtmp.23
.L8:
	clr.w (%a0)+	| MEM[base: _27, offset: 0B]
	cmp.l #sram+698,%a0	|, ivtmp.23
	jne .L8	|
	move.w #4919,sram	|, sram.magic_0
	move.w #4919,sram+26	|, sram.magic_1
	move.w #4919,sram+688	|, sram.magic_2
	move.w #5,sram+48	|, sram.max_hp
	move.w #1,sram+56	|, sram.have_phantom
	move.w #1,sram+58	|, sram.have_fast_phantom
	move.w #1,sram+60	|, sram.have_cheap_phantom
	move.w #1,sram+52	|, sram.have_jump
	move.w #1,sram+50	|, sram.have_lift
	clr.w sram+690	| sram.opt_interlace
	clr.w sram+696	| sram.opt_ctrlscheme
	clr.w sram+692	| sram.opt_mus
	clr.w sram+694	| sram.opt_sfx
	jra save_write	|
	.size	save_clear, .-save_clear
	.align	2
	.globl	save_load
	.type	save_load, @function
save_load:
	move.l %a3,-(%sp)	|,
	move.l %a2,-(%sp)	|,
	jsr SRAM_enableRO	|
	lea sram,%a2	|, ivtmp.35
	lea SRAM_readWord,%a3	|, tmp47
.L13:
	move.l %a2,%d0	| ivtmp.35,
	sub.l #sram,%d0	|,
	move.l %d0,-(%sp)	|,
	jsr (%a3)	| tmp47
	move.w %d0,(%a2)+	|, MEM[base: _24, offset: 0B]
	addq.l #4,%sp	|,
	cmp.l #sram+698,%a2	|, ivtmp.35
	jne .L13	|
	jsr SRAM_disable	|
	cmp.w #4919,sram.l	|, sram.magic_0
	jeq .L17	|
.L14:
	move.l (%sp)+,%a2	|,
	move.l (%sp)+,%a3	|,
	jra save_clear	|
.L17:
	cmp.w #4919,sram+26.l	|, sram.magic_1
	jne .L14	|
	cmp.w #4919,sram+688.l	|, sram.magic_2
	jne .L14	|
	move.l (%sp)+,%a2	|,
	move.l (%sp)+,%a3	|,
	rts
	.size	save_load, .-save_load
	.comm	sram,698,2
	.ident	"GCC: (GNU) 4.8.2"
