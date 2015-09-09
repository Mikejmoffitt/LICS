	.file	"col.c"
| GNU C (GCC) version 4.8.2 (m68k-elf)
|	compiled by GNU C version 4.9.3, GMP version 5.0.5, MPFR version 2.4.2, MPC version 0.8.2
| GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
| options passed:  -I /opt/toolchains/gen/m68k-elf/include
| -I /opt/toolchains/gen/m68k-elf/m68k-elf/include -I src -I res -I inc
| src/col.c -mcpu=68000 -O2 -std=c99 -fomit-frame-pointer -fno-builtin
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
	.globl	col_init
	.type	col_init, @function
col_init:
	pea 4096.w	|
	pea 8448.w	|
	pea gfx_font	|
	clr.l -(%sp)	|
	jsr VDP_doDMA	|
	pea 4.w	|
	move.w PLAN_B,-(%sp)	| PLAN_B,
	subq.l #2,%sp	|,
	jsr VDP_setHorizontalScroll	|
	pea 16.w	|
	move.w PLAN_B,-(%sp)	| PLAN_B,
	subq.l #2,%sp	|,
	jsr VDP_setVerticalScroll	|
	lea (32,%sp),%sp	|,
	rts
	.size	col_init, .-col_init
	.align	2
	.globl	col_puts40
	.type	col_puts40, @function
col_puts40:
	movem.l #14384,-(%sp)	|,
	move.l 24(%sp),%d2	| x, x
	move.l 32(%sp),%a2	| s, s
	move.w %d2,%d4	| x, x
	move.w 30(%sp),%d3	| y, y
	move.b (%a2),%d0	| *s_10(D), D.2588
	jeq .L2	|
	addq.l #1,%a2	|, ivtmp.21
	lea VDP_setTileMapXY,%a3	|, tmp70
.L8:
	cmp.b #10,%d0	|, D.2588
	jeq .L13	|
	cmp.b #9,%d0	|, D.2588
	jne .L6	|
	addq.w #4,%d2	|, x
	move.b (%a2)+,%d0	| MEM[base: _28, offset: 0B], D.2588
	jne .L8	|
.L2:
	movem.l (%sp)+,#3100	|,
	rts
.L6:
	move.w %d3,-(%sp)	| y,
	clr.w -(%sp)	|
	move.w %d2,-(%sp)	| x,
	clr.w -(%sp)	|
	ext.w %d0	| D.2590
	add.w #-32256,%d0	|, D.2590
	move.w %d0,-(%sp)	| D.2590,
	clr.w -(%sp)	|
	moveq #0,%d0	| D.2589
	move.w aplan_adr,%d0	| aplan_adr, D.2589
	move.l %d0,-(%sp)	| D.2589,
	jsr (%a3)	| tmp70
	addq.w #1,%d2	|, x
	lea (16,%sp),%sp	|,
	move.b (%a2)+,%d0	| MEM[base: _28, offset: 0B], D.2588
	jne .L8	|
	jra .L2	|
.L13:
	addq.w #1,%d3	|, y
	move.w %d4,%d2	| x, x
	move.b (%a2)+,%d0	| MEM[base: _28, offset: 0B], D.2588
	jne .L8	|
	jra .L2	|
	.size	col_puts40, .-col_puts40
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"0x"
	.text
	.align	2
	.globl	col_puthex
	.type	col_puthex, @function
col_puthex:
	subq.l #4,%sp	|,
	movem.l #15904,-(%sp)	|,
	move.l 32(%sp),%d6	| x, x
	move.l 40(%sp),%d3	| val, val
	moveq #0,%d4	| D.2605
	move.w 38(%sp),%d4	| y, D.2605
	pea .LC0	|
	move.l %d4,-(%sp)	| D.2605,
	move.w %d6,-(%sp)	| x,
	clr.w -(%sp)	|
	lea col_puts40,%a2	|, tmp70
	jsr (%a2)	| tmp70
	move.w %d6,%d2	|, x
	add.w #9,%d2	|, x
	addq.w #1,%d6	|, D.2604
	lea (12,%sp),%sp	|,
	moveq #26,%d5	|, tmp71
	add.l %sp,%d5	|, tmp71
.L20:
	clr.b 27(%sp)	| num
	moveq #15,%d0	|, D.2606
	and.l %d3,%d0	| val, D.2606
	moveq #9,%d1	|,
	cmp.l %d0,%d1	| D.2606,
	jcs .L16	|
	add.b #48,%d0	|,
	move.b %d0,26(%sp)	|, num
	lsr.l #4,%d3	|, val
	move.l %d5,-(%sp)	| tmp71,
	move.l %d4,-(%sp)	| D.2605,
	move.w %d2,-(%sp)	| x,
	clr.w -(%sp)	|
	jsr (%a2)	| tmp70
	subq.w #1,%d2	|, x
	lea (12,%sp),%sp	|,
	cmp.w %d2,%d6	| x, D.2604
	jne .L20	|
.L22:
	movem.l (%sp)+,#1148	|,
	addq.l #4,%sp	|,
	rts
.L16:
	add.b #55,%d0	|,
	move.b %d0,26(%sp)	|, num
	lsr.l #4,%d3	|, val
	move.l %d5,-(%sp)	| tmp71,
	move.l %d4,-(%sp)	| D.2605,
	move.w %d2,-(%sp)	| x,
	clr.w -(%sp)	|
	jsr (%a2)	| tmp70
	subq.w #1,%d2	|, x
	lea (12,%sp),%sp	|,
	cmp.w %d2,%d6	| x, D.2604
	jne .L20	|
	jra .L22	|
	.size	col_puthex, .-col_puthex
	.align	2
	.globl	col_puts
	.type	col_puts, @function
col_puts:
	movem.l #16184,-(%sp)	|,
	move.l 40(%sp),%d2	| x, x
	move.l 48(%sp),%a2	| s, s
	move.w %d2,%a4	| x, x
	move.w 46(%sp),%d4	| y, y
	move.w %d2,%d5	|, D.2624
	and.w #1,%d5	|, D.2624
	and.l #65535,%d5	|, flip
	move.b (%a2),%d0	| *s_14(D), D.2622
	jeq .L24	|
	addq.l #1,%a2	|, ivtmp.40
	move.l %d5,%d3	| flip, flip
	lea VDP_setTileMapXY,%a3	|, tmp58
.L32:
	cmp.b #10,%d0	|, D.2622
	jeq .L37	|
	cmp.b #9,%d0	|, D.2622
	jeq .L38	|
	moveq #0,%d7	| D.2623
	move.w %d4,%d7	| y, D.2623
	move.w %d2,%d1	| x, D.2624
	lsr.w #1,%d1	|, D.2624
	and.l #65535,%d1	|, D.2623
	ext.w %d0	| D.2624
	add.w #-32128,%d0	|, D.2624
	and.l #65535,%d0	|, D.2623
	moveq #0,%d6	|
	tst.l %d3	| flip
	jeq .L29	|
	move.w bplan_adr,%d6	| bplan_adr,
	move.l %d6,%a0	|, iftmp.6
	move.l %d7,-(%sp)	| D.2623,
	move.l %d1,-(%sp)	| D.2623,
	move.l %d0,-(%sp)	| D.2623,
	move.l %a0,-(%sp)	| iftmp.6,
	jsr (%a3)	| tmp58
	addq.w #1,%d2	|, x
	eor.w #1,%d3	|, flip
	lea (16,%sp),%sp	|,
.L27:
	move.b (%a2)+,%d0	| MEM[base: _4, offset: 0B], D.2622
	jne .L32	|
.L24:
	movem.l (%sp)+,#7420	|,
	rts
.L37:
	addq.w #1,%d4	|, y
	move.l %d5,%d3	| flip, flip
	move.w %a4,%d2	| x, x
	move.b (%a2)+,%d0	| MEM[base: _4, offset: 0B], D.2622
	jne .L32	|
	jra .L24	|
.L29:
	move.w aplan_adr,%d6	| aplan_adr,
	move.l %d6,%a0	|, iftmp.6
	move.l %d7,-(%sp)	| D.2623,
	move.l %d1,-(%sp)	| D.2623,
	move.l %d0,-(%sp)	| D.2623,
	move.l %a0,-(%sp)	| iftmp.6,
	jsr (%a3)	| tmp58
	addq.w #1,%d2	|, x
	eor.w #1,%d3	|, flip
	lea (16,%sp),%sp	|,
	jra .L27	|
.L38:
	addq.w #4,%d2	|, x
	move.b (%a2)+,%d0	| MEM[base: _4, offset: 0B], D.2622
	jne .L32	|
	jra .L24	|
	.size	col_puts, .-col_puts
	.ident	"GCC: (GNU) 4.8.2"
