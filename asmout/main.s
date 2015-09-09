	.file	"main.c"
| GNU C (GCC) version 4.8.2 (m68k-elf)
|	compiled by GNU C version 4.9.3, GMP version 5.0.5, MPFR version 2.4.2, MPC version 0.8.2
| GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
| options passed:  -I /opt/toolchains/gen/m68k-elf/include
| -I /opt/toolchains/gen/m68k-elf/m68k-elf/include -I src -I res -I inc
| src/main.c -mcpu=68000 -O2 -std=c99 -fomit-frame-pointer -fno-builtin
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
	.globl	room_setup
	.type	room_setup, @function
room_setup:
	move.l %a2,-(%sp)	|,
	move.l 8(%sp),%a2	| pl, pl
	clr.l -(%sp)	|
	jsr VDP_setEnable	|
	moveq #0,%d0	| D.2813
	move.b state+15,%d0	| state.next_id, D.2813
	move.l %d0,-(%sp)	| D.2813,
	jsr state_load_room	|
	move.l %a2,-(%sp)	| pl,
	jsr player_init_soft	|
	jsr particles_init	|
	jsr cubes_init	|
	move.l #65536,4(%a2)	|, pl_7(D)->y
	move.l #65536,(%a2)	|, pl_7(D)->x
	move.l state,%a0	| state.current_room, state.current_room
	moveq #0,%d0	| D.2813
	move.b 40(%a0),%d0	| _13->tileset, D.2813
	move.l %d0,-(%sp)	| D.2813,
	jsr map_load_tileset	|
	jsr particles_dma_tiles	|
	jsr cube_dma_tiles	|
	jsr hud_dma_tiles	|
	pea 64.w	|
	pea 64.w	|
	jsr state_update_scroll	|
	move.l %a2,-(%sp)	| pl,
	jsr player_draw	|
	jsr state_dma_scroll	|
	lea system_wait_v,%a2	|, tmp55
	jsr (%a2)	| tmp55
	jsr (%a2)	| tmp55
	jsr (%a2)	| tmp55
	jsr (%a2)	| tmp55
	lea (28,%sp),%sp	|,
	move.l (%sp)+,%a2	|,
	jra system_wait_v	|
	.size	room_setup, .-room_setup
	.align	2
	.globl	room_loop
	.type	room_loop, @function
room_loop:
	lea (-48,%sp),%sp	|,
	movem.l #16190,-(%sp)	|,
	move.w #1,state+14	|, state.next_id
	move.b #64,state+154	|, state.current_id
	lea (44,%sp),%a2	|,, tmp129
	move.l %a2,-(%sp)	| tmp129,
	jsr player_init	|
	addq.l #4,%sp	|,
	lea VDP_setPaletteColor,%a3	|, tmp135
	move.l #player_run,%d7	|, tmp125
	move.l #cubes_run,%d6	|, tmp124
	move.l #particles_run,%d5	|, tmp137
	move.l #VDP_setEnable,%d4	|, tmp133
	lea state_watch_transitions,%a6	|, tmp128
	move.l %a2,-(%sp)	| tmp129,
	jsr room_setup	|
	addq.w #1,state+14	|, state.next_id
	addq.l #4,%sp	|,
.L17:
	tst.w debug_bgcol	| debug_bgcol
	jeq .L3	|
	pea 512.w	|
	clr.l -(%sp)	|
	jsr (%a3)	| tmp135
	addq.l #8,%sp	|,
.L3:
	move.l %a2,-(%sp)	| tmp129,
	move.l %d7,%a0	| tmp125,
	jsr (%a0)	|
	addq.l #4,%sp	|,
	tst.w debug_bgcol	| debug_bgcol
	jne .L23	|
	move.l %a2,-(%sp)	| tmp129,
	move.l %d6,%a0	| tmp124,
	jsr (%a0)	|
	move.l %d5,%a0	| tmp137,
	jsr (%a0)	|
	move.l 48(%sp),%d2	| pl.x, D.2818
	moveq #10,%d0	|,
	asr.l %d0,%d2	|, D.2818
	move.l 52(%sp),%d3	| pl.y, D.2818
	asr.l %d0,%d3	|, D.2818
	addq.l #4,%sp	|,
	tst.w debug_bgcol	| debug_bgcol
	jne .L24	|
.L6:
	move.w 54(%sp),%a5	| pl.dy, D.2820
	move.w 52(%sp),%a4	| pl.dx, D.2820
	and.l #65535,%d3	|, D.2820
	and.l #65535,%d2	|, D.2820
	move.l %d3,-(%sp)	| D.2820,
	move.l %d2,-(%sp)	| D.2820,
	jsr state_update_scroll	|
	move.l %a5,-(%sp)	| D.2820,
	move.l %a4,-(%sp)	| D.2820,
	move.w %d0,-(%sp)	|,
	clr.w -(%sp)	|
	jsr map_draw_diffs	|
	lea (20,%sp),%sp	|,
	tst.w debug_bgcol	| debug_bgcol
	jne .L25	|
.L8:
	moveq #0,%d0	| D.2820
	move.w 86(%sp),%d0	| pl.hp, D.2820
	move.l %d0,-(%sp)	| D.2820,
	pea 8.w	|
	jsr hud_draw_health	|
	moveq #0,%d1	| D.2817
	move.w 96(%sp),%d1	| pl.cp, D.2817
	move.w %d1,%d0	|, D.2817
	addq.w #1,%d0	|, D.2817
	addq.l #1,%d1	|, D.2820
	asr.l #1,%d1	|, D.2820
	add.w %d1,%d0	| D.2820, D.2817
	move.w %d0,-(%sp)	| D.2817,
	clr.w -(%sp)	|
	jsr hud_draw_cp	|
	jsr particles_draw	|
	move.l %a2,-(%sp)	| tmp129,
	jsr player_draw	|
	jsr cubes_draw	|
	lea (16,%sp),%sp	|,
	tst.w debug_bgcol	| debug_bgcol
	jne .L26	|
.L9:
	jsr system_wait_v	|
	tst.w debug_bgcol	| debug_bgcol
	jne .L27	|
.L10:
	jsr map_dma	|
	tst.w debug_bgcol	| debug_bgcol
	jne .L28	|
.L11:
	jsr sprites_dma_simple	|
	tst.w debug_bgcol	| debug_bgcol
	jne .L29	|
.L12:
	jsr state_dma_scroll	|
	tst.w debug_bgcol	| debug_bgcol
	jne .L30	|
.L13:
	move.l %a2,-(%sp)	| tmp129,
	jsr player_dma	|
	pea 1.w	|
	move.l %d4,%a0	| tmp133,
	jsr (%a0)	|
	addq.l #8,%sp	|,
	btst #4,90(%sp)	|, pl.input
	jeq .L14	|
	move.w #30,88(%sp)	|, pl.cp
.L14:
	move.w 54(%sp),%a0	| pl.dy,
	move.l %a0,-(%sp)	|,
	move.w 56(%sp),%a0	| pl.dx,
	move.l %a0,-(%sp)	|,
	move.l %d3,-(%sp)	| D.2820,
	move.l %d2,-(%sp)	| D.2820,
	jsr (%a6)	| tmp128
	lea (16,%sp),%sp	|,
	tst.w %d0	|
	jeq .L17	|
	move.l %a2,-(%sp)	| tmp129,
	jsr room_setup	|
	addq.w #1,state+14	|, state.next_id
	addq.l #4,%sp	|,
	jra .L17	|
.L30:
	pea 238.w	|
	clr.l -(%sp)	|
	jsr (%a3)	| tmp135
	addq.l #8,%sp	|,
	jra .L13	|
.L29:
	pea 3598.w	|
	clr.l -(%sp)	|
	jsr (%a3)	| tmp135
	addq.l #8,%sp	|,
	jsr state_dma_scroll	|
	tst.w debug_bgcol	| debug_bgcol
	jeq .L13	|
	jra .L30	|
.L28:
	pea 654.w	|
	clr.l -(%sp)	|
	jsr (%a3)	| tmp135
	addq.l #8,%sp	|,
	jsr sprites_dma_simple	|
	tst.w debug_bgcol	| debug_bgcol
	jeq .L12	|
	jra .L29	|
.L27:
	pea 14.w	|
	clr.l -(%sp)	|
	jsr (%a3)	| tmp135
	addq.l #8,%sp	|,
	jsr map_dma	|
	tst.w debug_bgcol	| debug_bgcol
	jeq .L11	|
	jra .L28	|
.L26:
	clr.l -(%sp)	|
	clr.l -(%sp)	|
	jsr (%a3)	| tmp135
	addq.l #8,%sp	|,
	jsr system_wait_v	|
	tst.w debug_bgcol	| debug_bgcol
	jeq .L10	|
	jra .L27	|
.L25:
	pea 642.w	|
	clr.l -(%sp)	|
	jsr (%a3)	| tmp135
	addq.l #8,%sp	|,
	moveq #0,%d0	| D.2820
	move.w 86(%sp),%d0	| pl.hp, D.2820
	move.l %d0,-(%sp)	| D.2820,
	pea 8.w	|
	jsr hud_draw_health	|
	moveq #0,%d1	| D.2817
	move.w 96(%sp),%d1	| pl.cp, D.2817
	move.w %d1,%d0	|, D.2817
	addq.w #1,%d0	|, D.2817
	addq.l #1,%d1	|, D.2820
	asr.l #1,%d1	|, D.2820
	add.w %d1,%d0	| D.2820, D.2817
	move.w %d0,-(%sp)	| D.2817,
	clr.w -(%sp)	|
	jsr hud_draw_cp	|
	jsr particles_draw	|
	move.l %a2,-(%sp)	| tmp129,
	jsr player_draw	|
	jsr cubes_draw	|
	lea (16,%sp),%sp	|,
	tst.w debug_bgcol	| debug_bgcol
	jeq .L9	|
	jra .L26	|
.L24:
	pea 2280.w	|
	clr.l -(%sp)	|
	jsr (%a3)	| tmp135
	addq.l #8,%sp	|,
	tst.w debug_bgcol	| debug_bgcol
	jeq .L6	|
	pea 1092.w	|
	clr.l -(%sp)	|
	jsr VDP_setPaletteColor	|
	addq.l #8,%sp	|,
	move.w 54(%sp),%a5	| pl.dy, D.2820
	move.w 52(%sp),%a4	| pl.dx, D.2820
	and.l #65535,%d3	|, D.2820
	and.l #65535,%d2	|, D.2820
	move.l %d3,-(%sp)	| D.2820,
	move.l %d2,-(%sp)	| D.2820,
	jsr state_update_scroll	|
	move.l %a5,-(%sp)	| D.2820,
	move.l %a4,-(%sp)	| D.2820,
	move.w %d0,-(%sp)	|,
	clr.w -(%sp)	|
	jsr map_draw_diffs	|
	lea (20,%sp),%sp	|,
	tst.w debug_bgcol	| debug_bgcol
	jeq .L8	|
	jra .L25	|
.L23:
	pea 516.w	|
	clr.l -(%sp)	|
	jsr (%a3)	| tmp135
	addq.l #8,%sp	|,
	move.l %a2,-(%sp)	| tmp129,
	move.l %d6,%a0	| tmp124,
	jsr (%a0)	|
	move.l %d5,%a0	| tmp137,
	jsr (%a0)	|
	move.l 48(%sp),%d2	| pl.x, D.2818
	moveq #10,%d0	|,
	asr.l %d0,%d2	|, D.2818
	move.l 52(%sp),%d3	| pl.y, D.2818
	asr.l %d0,%d3	|, D.2818
	addq.l #4,%sp	|,
	tst.w debug_bgcol	| debug_bgcol
	jeq .L6	|
	jra .L24	|
	.size	room_loop, .-room_loop
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	jsr system_init	|
	jsr room_loop	|
	.size	main, .-main
	.comm	debug_bgcol,2,2
	.ident	"GCC: (GNU) 4.8.2"
