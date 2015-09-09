	.file	"system.c"
| GNU C (GCC) version 4.8.2 (m68k-elf)
|	compiled by GNU C version 4.9.3, GMP version 5.0.5, MPFR version 2.4.2, MPC version 0.8.2
| GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
| options passed:  -I /opt/toolchains/gen/m68k-elf/include
| -I /opt/toolchains/gen/m68k-elf/m68k-elf/include -I src -I res -I inc
| src/system.c -mcpu=68000 -O2 -std=c99 -fomit-frame-pointer -fno-builtin
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
	.type	v_int, @function
v_int:
	move.w #1,vbl_active	|, vbl_active
	moveq #0,%d0	|
	rts
	.size	v_int, .-v_int
	.align	2
	.type	h_int, @function
h_int:
	moveq #0,%d0	|
	rts
	.size	h_int, .-h_int
	.align	2
	.globl	system_init
	.type	system_init, @function
system_init:
	move.l %a2,-(%sp)	|,
	jsr VDP_init	|
	jsr SYS_disableInts	|
	pea 1.w	|
	jsr VDP_setHInterrupt	|
	pea 217.w	|
	jsr VDP_setHIntCounter	|
	pea v_int	|
	jsr SYS_setVIntCallback	|
	pea h_int	|
	jsr SYS_setHIntCallback	|
	jsr SYS_enableInts	|
	pea 32.w	|
	pea 64.w	|
	jsr VDP_setPlanSize	|
	jsr VDP_setScreenWidth320	|
	jsr VDP_setScreenHeight240	|
	clr.l -(%sp)	|
	jsr VDP_setHilightShadow	|
	clr.l -(%sp)	|
	clr.l -(%sp)	|
	jsr VDP_setScrollingMode	|
	lea (36,%sp),%sp	|,
	jsr sprites_init	|
	jsr save_load	|
	clr.l -(%sp)	|
	lea pad_read,%a2	|, tmp53
	jsr (%a2)	| tmp53
	addq.l #4,%sp	|,
	btst #4,%d0	|,
	jne .L6	|
	clr.l -(%sp)	|
	jsr (%a2)	| tmp53
	addq.l #4,%sp	|,
	tst.b %d0	|
	jlt .L6	|
	cmp.w #1,sram+690.l	|, sram.opt_interlace
	jeq .L10	|
.L7:
	clr.l -(%sp)	|
	jsr VDP_setScanMode	|
	addq.l #4,%sp	|,
	move.l (%sp)+,%a2	|,
	rts
.L6:
	cmp.w #1,sram+690.l	|, sram.opt_interlace
	sne %d0	| tmp58
	ext.w %d0	| tmp57
	neg.w %d0	| tmp57
	move.w %d0,sram+690	| tmp57, sram.opt_interlace
	jsr save_write	|
	cmp.w #1,sram+690.l	|, sram.opt_interlace
	jne .L7	|
.L10:
	pea 1.w	|
	jsr VDP_setScanMode	|
	addq.l #4,%sp	|,
	move.l (%sp)+,%a2	|,
	rts
	.size	system_init, .-system_init
	.align	2
	.globl	system_wait_v
	.type	system_wait_v, @function
system_wait_v:
	addq.w #1,system_osc	|, system_osc
.L14:
	move.w vbl_active,%d0	| vbl_active, vbl_active.3
	jeq .L14	|
	move.w #0,vbl_active	|, vbl_active
	rts
	.size	system_wait_v, .-system_wait_v
	.comm	system_osc,2,2
	.local	vbl_active
	.comm	vbl_active,2,2
	.ident	"GCC: (GNU) 4.8.2"
