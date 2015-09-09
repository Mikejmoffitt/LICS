	.file	"state.c"
| GNU C (GCC) version 4.8.2 (m68k-elf)
|	compiled by GNU C version 4.9.3, GMP version 5.0.5, MPFR version 2.4.2, MPC version 0.8.2
| GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
| options passed:  -I /opt/toolchains/gen/m68k-elf/include
| -I /opt/toolchains/gen/m68k-elf/m68k-elf/include -I src -I res -I inc
| src/state.c -mcpu=68000 -O2 -std=c99 -fomit-frame-pointer -fno-builtin
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
	.globl	state_load_room
	.type	state_load_room, @function
state_load_room:
	move.l %d3,-(%sp)	|,
	move.l %d2,-(%sp)	|,
	move.l 12(%sp),%d2	| roomnum, roomnum
	cmp.b state+154.l,%d2	| state.current_id, roomnum
	jeq .L1	|
	move.w #32767,state+8	|, state.cam_x
	move.w #32767,state+10	|, state.cam_y
	move.w #32767,sx_memo	|, sx_memo
	move.w #32767,sy_memo	|, sy_memo
	moveq #0,%d0	| D.2748
	move.b %d2,%d0	| roomnum, D.2748
	move.l %d0,-(%sp)	| D.2748,
	jsr map_by_id	|
	move.l %d0,%a0	|, D.2749
	move.l %d0,state	| D.2749, state.current_room
	lea (1068,%a0),%a1	|, D.2749,
	move.l %a1,state+4	|, state.current_map
	move.b #1,state+155	|, state.fresh_room
	clr.w %d0	| D.2751
	move.b 32(%a0),%d0	| _12->music, D.2751
	addq.l #4,%sp	|,
	cmp.w state+12.l,%d0	| state.current_music, D.2751
	jeq .L3	|
	move.w %d0,state+12	| D.2751, state.current_music
.L3:
	move.b %d2,state+154	| roomnum, state.current_id
	move.b 37(%a0),%d2	| _12->h, D.2747
	cmp.b #1,%d2	|, D.2747
	sne %d0	| tmp58
	neg.b %d0	| tmp58
	clr.w %d1	| D.2752
	move.b %d0,%d1	| tmp58, D.2752
	move.w %d1,state+18	| D.2752, state.vs_en
	move.b 35(%a0),%d1	| _12->w, D.2747
	cmp.b #1,%d1	|, D.2747
	sne %d3	| tmp63
	ext.w %d3	| tmp62
	neg.w %d3	| tmp62
	move.w %d3,state+20	| tmp62, state.hs_en
	cmp.b #1,%d1	|, D.2747
	jeq .L10	|
.L4:
	clr.l -(%sp)	|
	pea 2.w	|
	jsr VDP_setScrollingMode	|
	addq.l #8,%sp	|,
.L1:
	move.l (%sp)+,%d2	|,
	move.l (%sp)+,%d3	|,
	rts
.L10:
	cmp.b #1,%d2	|, D.2747
	jeq .L11	|
	tst.b %d0	| tmp58
	jeq .L4	|
	pea 1.w	|
	clr.l -(%sp)	|
	jsr VDP_setScrollingMode	|
	addq.l #8,%sp	|,
	move.l (%sp)+,%d2	|,
	move.l (%sp)+,%d3	|,
	rts
.L11:
	clr.l -(%sp)	|
	clr.l -(%sp)	|
	jsr VDP_setScrollingMode	|
	addq.l #8,%sp	|,
	move.l (%sp)+,%d2	|,
	move.l (%sp)+,%d3	|,
	rts
	.size	state_load_room, .-state_load_room
	.align	2
	.globl	state_update_scroll
	.type	state_update_scroll, @function
state_update_scroll:
	movem.l #15392,-(%sp)	|,
	move.l 24(%sp),%d0	| px, px
	move.l 28(%sp),%d2	| py, py
	clr.w state+22	| state.xscroll_cmd
	clr.w state+24	| state.yscroll_cmd
	tst.w state+20	| state.hs_en
	jeq .L17	|
	move.l state,%a0	| state.current_room, state.current_room
	moveq #0,%d1	| D.2786
	move.b 35(%a0),%d1	| _15->w, D.2786
	moveq #0,%d5	| D.2786
	move.w %d0,%d5	| px, D.2786
	move.l %d1,%d3	| D.2786, tmp120
	add.l %d1,%d3	| D.2786, tmp120
	add.l %d3,%d3	| tmp120, tmp121
	add.l %d1,%d3	| D.2786, tmp123
	lsl.l #6,%d3	|, tmp123
	add.l #-160,%d3	|, D.2786
	cmp.l %d5,%d3	| D.2786, D.2786
	jgt .L16	|
	subq.l #1,%d1	|, D.2786
	muls.w #320,%d1	|,
	move.w %d1,state+8	|, state.cam_x
.L15:
	tst.w state+18	| state.vs_en
	jne .L18	|
.L39:
	jsr VDP_getScreenHeight	|
	move.w #240,%d2	|, D.2785
	sub.w %d0,%d2	|, D.2785
	move.w %d2,state+10	| D.2785, state.cam_y
	moveq #0,%d0	| D.2786
	move.w %d2,%d0	| D.2785, D.2786
.L19:
	move.w sy_memo,%a0	| sy_memo, D.2786
	cmp.l %a0,%d0	| D.2786, D.2786
	jeq .L22	|
	move.w %d2,sy_memo	| D.2785, sy_memo
	jsr VDP_getVerticalScrollingMode	|
	tst.b %d0	|
	jeq .L34	|
	lea state+90,%a0	|, ivtmp.34
.L25:
	move.w %d2,(%a0)+	| D.2785, MEM[base: _91, offset: 0B]
	cmp.l #state+154,%a0	|, ivtmp.34
	jne .L25	|
	move.w #2,state+24	|, state.yscroll_cmd
.L22:
	move.w state+8,%d2	| state.cam_x, D.2785
	moveq #0,%d0	| D.2786
	move.w %d2,%d0	| D.2785, D.2786
	move.w sx_memo,%a0	| sx_memo, D.2786
	cmp.l %d0,%a0	| D.2786, D.2786
	jeq .L35	|
.L26:
	move.w %d2,sx_memo	| D.2785, sx_memo
	neg.w %d2	| D.2790
	jsr VDP_getHorizontalScrollingMode	|
	tst.b %d0	|
	jeq .L36	|
	lea state+26,%a0	|, ivtmp.22
.L30:
	move.w %d2,(%a0)+	| D.2790, MEM[base: _111, offset: 0B]
	cmp.l #state+90,%a0	|, ivtmp.22
	jne .L30	|
	move.w #2,state+22	|, state.xscroll_cmd
	moveq #1,%d0	|, D.2791
.L27:
	tst.w state+24	| state.yscroll_cmd
	jne .L37	|
.L32:
	clr.w %d1	| iftmp.2
	or.w %d1,%d0	| iftmp.2,
	movem.l (%sp)+,#1084	|,
	rts
.L16:
	cmp.w #160,%d0	|, px
	jhi .L38	|
.L17:
	clr.w state+8	| state.cam_x
	tst.w state+18	| state.vs_en
	jeq .L39	|
.L18:
	add.w #-12,%d2	|, py
	move.l state,%a0	| state.current_room, state.current_room
	clr.w %d3	| D.2790
	move.b 37(%a0),%d3	| _33->h, D.2790
	mulu.w #240,%d3	|, D.2786
	lea VDP_getScreenHeight,%a2	|, tmp134
	jsr (%a2)	| tmp134
	moveq #0,%d1	| D.2786
	move.w %d2,%d1	| py, D.2786
	lsr.w #1,%d0	|, D.2785
	and.l #65535,%d0	|, D.2786
	sub.l %d0,%d3	| D.2786, D.2786
	cmp.l %d1,%d3	| D.2786, D.2786
	jgt .L20	|
	move.l state,%a0	| state.current_room, state.current_room
	clr.w %d2	| D.2785
	move.b 37(%a0),%d2	| _42->h, D.2785
	muls.w #240,%d2	|, D.2785
	jsr (%a2)	| tmp134
	sub.w %d0,%d2	|, D.2785
	move.w %d2,state+10	| D.2785, state.cam_y
	moveq #0,%d0	| D.2786
	move.w %d2,%d0	| D.2785, D.2786
	jra .L19	|
.L20:
	jsr (%a2)	| tmp134
	lsr.w #1,%d0	|, D.2785
	cmp.w %d2,%d0	| py, D.2785
	jcs .L40	|
	clr.w state+10	| state.cam_y
	moveq #0,%d0	| D.2786
	clr.w %d2	| D.2785
	jra .L19	|
.L34:
	move.w %d2,state+90	| D.2785, state.yscroll_vals
	move.w #1,state+24	|, state.yscroll_cmd
	move.w state+8,%d2	| state.cam_x, D.2785
	moveq #0,%d0	| D.2786
	move.w %d2,%d0	| D.2785, D.2786
	move.w sx_memo,%a0	| sx_memo, D.2786
	cmp.l %d0,%a0	| D.2786, D.2786
	jne .L26	|
.L35:
	tst.w state+22	| state.xscroll_cmd
	sne %d0	| tmp159
	ext.w %d0	| D.2791
	neg.w %d0	| D.2791
	tst.w state+24	| state.yscroll_cmd
	jeq .L32	|
.L37:
	moveq #2,%d1	|, iftmp.2
	or.w %d1,%d0	| iftmp.2,
	movem.l (%sp)+,#1084	|,
	rts
.L36:
	move.w %d2,state+26	| D.2790, state.xscroll_vals
	move.w #1,state+22	|, state.xscroll_cmd
	moveq #1,%d0	|, D.2791
	jra .L27	|
.L38:
	add.w #-160,%d0	|,
	move.w %d0,state+8	|, state.cam_x
	jra .L15	|
.L40:
	jsr (%a2)	| tmp134
	lsr.w #1,%d0	|, D.2785
	sub.w %d0,%d2	| D.2785, D.2785
	move.w %d2,state+10	| D.2785, state.cam_y
	moveq #0,%d0	| D.2786
	move.w %d2,%d0	| D.2785, D.2786
	jra .L19	|
	.size	state_update_scroll, .-state_update_scroll
	.align	2
	.globl	state_dma_scroll
	.type	state_dma_scroll, @function
state_dma_scroll:
	move.w state+22,%d0	| state.xscroll_cmd, D.2799
	cmp.w #2,%d0	|, D.2799
	jeq .L54	|
	tst.w %d0	| D.2799
	jne .L55	|
.L44:
	move.w state+24,%d0	| state.yscroll_cmd, D.2799
	cmp.w #2,%d0	|, D.2799
	jeq .L56	|
	tst.w %d0	| D.2799
	jne .L57	|
	rts
.L57:
	move.w state+90,%a0	| state.yscroll_vals,
	move.l %a0,-(%sp)	|,
	move.w PLAN_A,-(%sp)	| PLAN_A,
	subq.l #2,%sp	|,
	jsr VDP_setVerticalScroll	|
	addq.l #8,%sp	|,
	rts
.L55:
	move.w state+26,%a0	| state.xscroll_vals,
	move.l %a0,-(%sp)	|,
	move.w PLAN_A,-(%sp)	| PLAN_A,
	subq.l #2,%sp	|,
	jsr VDP_setHorizontalScroll	|
	addq.l #8,%sp	|,
	jra .L44	|
.L54:
	pea 1.w	|
	pea 32.w	|
	pea state+26	|
	clr.l -(%sp)	|
	move.w PLAN_A,-(%sp)	| PLAN_A,
	subq.l #2,%sp	|,
	jsr VDP_setHorizontalScrollTile	|
	lea (20,%sp),%sp	|,
	jra .L44	|
.L56:
	pea 1.w	|
	pea 32.w	|
	pea state+90	|
	clr.l -(%sp)	|
	move.w PLAN_A,-(%sp)	| PLAN_A,
	subq.l #2,%sp	|,
	jsr VDP_setVerticalScrollTile	|
	lea (20,%sp),%sp	|,
	rts
	.size	state_dma_scroll, .-state_dma_scroll
	.align	2
	.globl	state_watch_transitions
	.type	state_watch_transitions, @function
state_watch_transitions:
	movem.l #14368,-(%sp)	|,
	move.l 20(%sp),%a0	| px, px
	move.l 24(%sp),%a1	| py, py
	move.w %a0,%d1	| px, px
	move.w %a1,%d4	| py, py
	move.w 30(%sp),%d0	| dx, dx
	move.w 34(%sp),%d3	| dy, dy
	tst.w %d0	| dx
	jne .L60	|
	tst.w %d3	| dy
	jeq .L68	|
	move.l state,%a2	| state.current_room, D.2805
.L64:
	cmp.w #6,%d4	|, py
	jhi .L65	|
	tst.w %d3	| dy
	jlt .L71	|
.L65:
	clr.w %d1	| D.2808
	move.b 37(%a2),%d1	| _24->h, D.2808
	mulu.w #240,%d1	|, D.2807
	move.l %d1,%a0	| D.2807, D.2807
	subq.l #6,%a0	|, D.2807
	moveq #0,%d1	| D.2807
	move.w %a1,%d1	| py, D.2807
	cmp.l %a0,%d1	| D.2807, D.2807
	jlt .L68	|
	tst.w %d0	| dx
	jle .L68	|
.L71:
	moveq #1,%d0	|, D.2804
.L73:
	movem.l (%sp)+,#1052	|,
	rts
.L68:
	clr.w %d0	| D.2804
	movem.l (%sp)+,#1052	|,
	rts
.L60:
	cmp.w #6,%d1	|, px
	jhi .L63	|
	tst.w %d0	| dx
	jlt .L71	|
.L63:
	move.l state,%a2	| state.current_room, D.2805
	moveq #0,%d2	| D.2807
	move.b 35(%a2),%d2	| _6->w, D.2807
	move.l %d2,%d1	| D.2807, tmp57
	add.l %d2,%d1	| D.2807, tmp57
	add.l %d1,%d1	| tmp57, tmp58
	add.l %d2,%d1	| D.2807, tmp60
	lsl.l #6,%d1	|, tmp60
	subq.l #6,%d1	|, D.2807
	move.w %a0,%d2	| px, D.2807
	cmp.l %d1,%d2	| D.2807, D.2807
	jlt .L64	|
	tst.w %d0	| dx
	jle .L64	|
	moveq #1,%d0	|, D.2804
	jra .L73	|
	.size	state_watch_transitions, .-state_watch_transitions
	.local	sy_memo
	.comm	sy_memo,2,2
	.local	sx_memo
	.comm	sx_memo,2,2
	.comm	state,156,2
	.ident	"GCC: (GNU) 4.8.2"
