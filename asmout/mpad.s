	.file	"mpad.c"
| GNU C (GCC) version 4.8.2 (m68k-elf)
|	compiled by GNU C version 4.9.3, GMP version 5.0.5, MPFR version 2.4.2, MPC version 0.8.2
| GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
| options passed:  -I /opt/toolchains/gen/m68k-elf/include
| -I /opt/toolchains/gen/m68k-elf/m68k-elf/include -I src -I res -I inc
| src/mpad.c -mcpu=68000 -O2 -std=c99 -fomit-frame-pointer -fno-builtin
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
	.globl	pad_read
	.type	pad_read, @function
pad_read:
	subq.l #4,%sp	|,
	move.l %d2,-(%sp)	|,
	moveq #0,%d0	| D.2495
	move.w 14(%sp),%d0	| pl, D.2495
	add.l %d0,%d0	| D.2495, D.2495
	move.l %d0,%a0	| D.2495, ctrlr
	add.l #10551299,%a0	|, ctrlr
	move.b (%a0),%d0	| *ctrlr_5, D.2497
	or.b #64,%d0	|, D.2497
	move.b %d0,(%a0)	| D.2497, *ctrlr_5
	move.b (%a0),%d0	| *ctrlr_5, ret.0
	move.b (%a0),%d1	| *ctrlr_5, D.2497
	moveq #48,%d2	|,
	and.l %d2,%d1	|, D.2495
	add.l %d1,%d1	| D.2495, D.2495
	and.b #15,%d0	|, ret.0
	or.b %d1,%d0	| D.2495, ret.0
	move.b %d0,7(%sp)	| ret.0, ret
	move.b (%a0),%d0	| *ctrlr_5, D.2497
	and.b #-65,%d0	|, D.2497
	move.b %d0,(%a0)	| D.2497, *ctrlr_5
	move.b (%a0),%d0	| *ctrlr_5, ret.3
	move.b (%a0),%d1	| *ctrlr_5, D.2497
	move.b 7(%sp),%d2	| ret, ret.1
	and.b #16,%d0	|, ret.3
	or.b %d2,%d0	| ret.1, ret.3
	moveq #32,%d2	|,
	and.l %d2,%d1	|, D.2495
	add.l %d1,%d1	| D.2495, tmp77
	add.l %d1,%d1	| tmp77, D.2495
	or.b %d1,%d0	| D.2495, ret.3
	move.b %d0,7(%sp)	| ret.3, ret
	move.b 7(%sp),%d0	| ret, ret.4
	not.b %d0	|
	move.l (%sp)+,%d2	|,
	addq.l #4,%sp	|,
	rts
	.size	pad_read, .-pad_read
	.ident	"GCC: (GNU) 4.8.2"
