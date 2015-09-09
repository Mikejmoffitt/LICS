	.file	"hud.c"
| GNU C (GCC) version 4.8.2 (m68k-elf)
|	compiled by GNU C version 4.9.3, GMP version 5.0.5, MPFR version 2.4.2, MPC version 0.8.2
| GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
| options passed:  -I /opt/toolchains/gen/m68k-elf/include
| -I /opt/toolchains/gen/m68k-elf/m68k-elf/include -I src -I res -I inc
| src/hud.c -mcpu=68000 -O2 -std=c99 -fomit-frame-pointer -fno-builtin
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
	.globl	hud_dma_tiles
	.type	hud_dma_tiles, @function
hud_dma_tiles:
	pea 512.w	|
	pea 24864.w	|
	pea gfx_hud	|
	clr.l -(%sp)	|
	jsr VDP_doDMA	|
	lea (16,%sp),%sp	|,
	rts
	.size	hud_dma_tiles, .-hud_dma_tiles
	.align	2
	.globl	hud_draw_health
	.type	hud_draw_health, @function
hud_draw_health:
	movem.l #15392,-(%sp)	|,
	move.l 24(%sp),%d4	| max, max
	move.l 28(%sp),%d5	| val, val
	move.l #58125,-(%sp)	|,
	pea 5.w	|
	pea 8.w	|
	pea 8.w	|
	lea sprite_put,%a2	|, tmp56
	jsr (%a2)	| tmp56
	and.l #65535,%d4	|, D.2593
	addq.l #1,%d4	|, D.2591
	lea (16,%sp),%sp	|,
	moveq #1,%d0	|,
	cmp.l %d4,%d0	| D.2591,
	jeq .L2	|
	and.l #65535,%d5	|, D.2591
	moveq #24,%d3	|, ivtmp.15
	moveq #1,%d2	|, i
.L6:
	move.l #58131,%d0	|, D.2593
	cmp.l %d5,%d2	| D.2591, i
	jcc .L4	|
	move.b #17,%d0	|,
.L4:
	move.l %d0,-(%sp)	| D.2593,
	pea 4.w	|
	move.w %d3,%a0	| ivtmp.15,
	move.l %a0,-(%sp)	|,
	pea 8.w	|
	jsr (%a2)	| tmp56
	addq.l #1,%d2	|, i
	addq.w #8,%d3	|, ivtmp.15
	lea (16,%sp),%sp	|,
	cmp.l %d2,%d4	| i, D.2591
	jhi .L6	|
.L2:
	movem.l (%sp)+,#1084	|,
	rts
	.size	hud_draw_health, .-hud_draw_health
	.align	2
	.globl	hud_draw_cp
	.type	hud_draw_cp, @function
hud_draw_cp:
	movem.l #16176,-(%sp)	|,
	move.l 36(%sp),%d2	| val, val
	move.w %d2,%d7	| val, val
	jsr VDP_getScreenHeight	|
	move.w %d0,%d3	|, D.2608
	move.l #58121,-(%sp)	|,
	pea 5.w	|
	add.w #-24,%d0	|, D.2609
	move.w %d0,%a0	| D.2609,
	move.l %a0,-(%sp)	|,
	pea 8.w	|
	lea sprite_put,%a2	|, tmp132
	jsr (%a2)	| tmp132
	lea (16,%sp),%sp	|,
	cmp.w #15,%d2	|, val
	jls .L11	|
	addq.w #1,%d7	|, val
.L11:
	move.l #58133,-(%sp)	|,
	pea 4.w	|
	move.w %d3,%d0	| D.2608, D.2609
	add.w #-80,%d0	|, D.2609
	move.w %d0,%a0	| D.2609,
	move.l %a0,-(%sp)	|,
	pea 8.w	|
	jsr (%a2)	| tmp132
	move.l #62229,-(%sp)	|,
	pea 4.w	|
	move.w %d3,%d0	| D.2608, D.2609
	add.w #-26,%d0	|, D.2609
	move.w %d0,%a0	| D.2609,
	move.l %a0,-(%sp)	|,
	pea 8.w	|
	jsr (%a2)	| tmp132
	moveq #7,%d0	|, D.2611
	and.l %d7,%d0	| val, D.2611
	addq.l #1,%d0	|, D.2611
	move.w %d0,%a3	|, D.2608
	add.w %d0,%a3	| D.2611, D.2608
	lea (32,%sp),%sp	|,
	add.w #-33,%d3	|, ivtmp.25
	moveq #0,%d4	| ivtmp.28
	moveq #1,%d2	|, ivtmp.24
	moveq #0,%d6	| D.2612
	move.w %d7,%d6	| val, D.2612
.L18:
	move.w system_osc,%d1	| system_osc, D.2608
	and.w #1,%d1	|, D.2608
	moveq #11,%d0	|,
	lsl.w %d0,%d1	|, D.2608
	move.w %d1,%d0	| D.2608, attr
	add.w #-7401,%d0	|, attr
	cmp.w #48,%d7	|, val
	jeq .L20	|
	move.l %d4,%d5	| ivtmp.28, ivtmp.28
	addq.l #8,%d5	|, ivtmp.28
	cmp.l %d6,%d5	| D.2612, ivtmp.28
	jls .L14	|
	cmp.l %d6,%d4	| D.2612, ivtmp.28
	jhi .L15	|
	add.w %a3,%d0	| D.2608, attr
.L16:
	move.w %d0,-(%sp)	| attr,
	clr.w -(%sp)	|
	pea 4.w	|
	move.w %d3,%a0	| ivtmp.25,
	move.l %a0,-(%sp)	|,
	pea 8.w	|
	jsr (%a2)	| tmp132
	lea (16,%sp),%sp	|,
.L13:
	addq.l #1,%d2	|, ivtmp.24
	subq.w #8,%d3	|, ivtmp.25
	move.l %d5,%d4	| ivtmp.28, ivtmp.28
	moveq #7,%d0	|,
	cmp.l %d2,%d0	| ivtmp.24,
	jne .L18	|
.L21:
	movem.l (%sp)+,#3324	|,
	rts
.L14:
	cmp.l %d6,%d4	| D.2612, ivtmp.28
	jls .L16	|
.L15:
	move.w %d1,%d0	| D.2608, attr
	add.w #-7399,%d0	|, attr
	move.w %d0,-(%sp)	| attr,
	clr.w -(%sp)	|
	pea 4.w	|
	move.w %d3,%a0	| ivtmp.25,
	move.l %a0,-(%sp)	|,
	pea 8.w	|
	jsr (%a2)	| tmp132
	lea (16,%sp),%sp	|,
	jra .L13	|
.L20:
	move.w %d0,-(%sp)	| attr,
	clr.w -(%sp)	|
	pea 4.w	|
	move.w %d3,%a0	| ivtmp.25,
	move.l %a0,-(%sp)	|,
	pea 8.w	|
	jsr (%a2)	| tmp132
	lea (16,%sp),%sp	|,
	move.l %d4,%d5	| ivtmp.28, ivtmp.28
	addq.l #8,%d5	|, ivtmp.28
	addq.l #1,%d2	|, ivtmp.24
	subq.w #8,%d3	|, ivtmp.25
	move.l %d5,%d4	| ivtmp.28, ivtmp.28
	moveq #7,%d0	|,
	cmp.l %d2,%d0	| ivtmp.24,
	jne .L18	|
	jra .L21	|
	.size	hud_draw_cp, .-hud_draw_cp
	.ident	"GCC: (GNU) 4.8.2"
