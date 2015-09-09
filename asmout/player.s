	.file	"player.c"
| GNU C (GCC) version 4.8.2 (m68k-elf)
|	compiled by GNU C version 4.9.3, GMP version 5.0.5, MPFR version 2.4.2, MPC version 0.8.2
| GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
| options passed:  -I /opt/toolchains/gen/m68k-elf/include
| -I /opt/toolchains/gen/m68k-elf/m68k-elf/include -I src -I res -I inc
| src/player.c -mcpu=68000 -O2 -std=c99 -fomit-frame-pointer -fno-builtin
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
	.type	player_eval_grounded.part.1, @function
player_eval_grounded.part.1:
	movem.l #12336,-(%sp)	|,
	move.l 20(%sp),%a2	| pl, pl
	move.l (%a2),%d2	| pl_1(D)->x, D.3359
	moveq #10,%d0	|,
	asr.l %d0,%d2	|, D.3359
	move.l 4(%a2),%d0	| pl_1(D)->y, D.3359
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3359
	addq.w #1,%d0	|, D.3360
	moveq #0,%d3	| D.3361
	move.w %d0,%d3	| D.3360, D.3361
	move.l %d3,-(%sp)	| D.3361,
	move.w %d2,%d0	|, D.3360
	addq.w #3,%d0	|, D.3360
	move.w %d0,-(%sp)	| D.3360,
	clr.w -(%sp)	|
	lea map_collision,%a3	|, tmp53
	jsr (%a3)	| tmp53
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L2	|
.L4:
	move.w #1,12(%a2)	|, pl_1(D)->grounded
	movem.l (%sp)+,#3084	|,
	rts
.L2:
	move.l %d3,-(%sp)	| D.3361,
	subq.w #4,%d2	|, D.3360
	move.w %d2,-(%sp)	| D.3360,
	clr.w -(%sp)	|
	jsr (%a3)	| tmp53
	addq.l #8,%sp	|,
	tst.w %d0	|
	jne .L4	|
	clr.w 12(%a2)	| pl_1(D)->grounded
	movem.l (%sp)+,#3084	|,
	rts
	.size	player_eval_grounded.part.1, .-player_eval_grounded.part.1
	.align	2
	.globl	player_init_soft
	.type	player_init_soft, @function
player_init_soft:
	move.l 4(%sp),%a0	| pl, pl
	clr.w 8(%a0)	| pl_2(D)->dx
	clr.w 10(%a0)	| pl_2(D)->dy
	clr.w 12(%a0)	| pl_2(D)->grounded
	clr.w 18(%a0)	| pl_2(D)->anim_cnt
	clr.w 20(%a0)	| pl_2(D)->anim_frame
	clr.w 36(%a0)	| pl_2(D)->holding_cube
	clr.w 24(%a0)	| pl_2(D)->throw_cnt
	clr.w 22(%a0)	| pl_2(D)->throwdown_cnt
	clr.w 26(%a0)	| pl_2(D)->kick_cnt
	clr.w 28(%a0)	| pl_2(D)->lift_cnt
	clr.w 40(%a0)	| pl_2(D)->cp_cnt
	clr.w 30(%a0)	| pl_2(D)->hurt_cnt
	clr.w 38(%a0)	| pl_2(D)->invuln_cnt
	clr.w 32(%a0)	| pl_2(D)->action_cnt
	clr.w 16(%a0)	| pl_2(D)->control_disabled
	clr.b 46(%a0)	| pl_2(D)->input
	clr.b 47(%a0)	| pl_2(D)->input_prev
	clr.w 34(%a0)	| pl_2(D)->cubejump_disable
	pea 16.w	|
	pea 96.w	|
	pea pal_lyle	|
	pea 1.w	|
	jsr VDP_doDMA	|
	lea (16,%sp),%sp	|,
	rts
	.size	player_init_soft, .-player_init_soft
	.align	2
	.globl	player_init
	.type	player_init, @function
player_init:
	move.l 4(%sp),%a0	| pl, pl
	clr.l (%a0)	| pl_2(D)->x
	clr.l 4(%a0)	| pl_2(D)->y
	clr.w 14(%a0)	| pl_2(D)->direction
	move.w #5,42(%a0)	|, pl_2(D)->hp
	move.w #30,44(%a0)	|, pl_2(D)->cp
	jra player_init_soft	|
	.size	player_init, .-player_init
	.align	2
	.globl	player_dma_setup
	.type	player_dma_setup, @function
player_dma_setup:
	move.l 4(%sp),%a0	| pl, pl
	move.w 20(%a0),%d1	| pl_4(D)->anim_frame, num
	cmp.w #19,%d1	|, num
	jhi .L10	|
	move.w %d1,%d0	| num, offset
	add.w %d1,%d0	| num, offset
	add.w %d1,%d0	| num, offset
	add.w %d0,%d0	| offset, offset
	moveq #96,%d1	|, iftmp.1
.L11:
	move.w %d1,lyle_dma_len	| iftmp.1, lyle_dma_len
	and.l #65535,%d0	|, D.3369
	lsl.l #5,%d0	|, D.3369
	add.l #gfx_lyle,%d0	|, D.3369
	move.l %d0,lyle_dma_src	| D.3369, lyle_dma_src
	move.w #24576,lyle_dma_dest	|, lyle_dma_dest
	rts
.L10:
	add.w #-20,%d1	|, num
	move.w %d1,%d0	| num, offset
	lsl.w #3,%d0	|, offset
	add.w %d1,%d0	| num, offset
	add.w #120,%d0	|, offset
	move.w #144,%d1	|, iftmp.1
	cmp.w #119,%d0	|, offset
	jhi .L11	|
	moveq #96,%d1	|, iftmp.1
	move.w %d1,lyle_dma_len	| iftmp.1, lyle_dma_len
	and.l #65535,%d0	|, D.3369
	lsl.l #5,%d0	|, D.3369
	add.l #gfx_lyle,%d0	|, D.3369
	move.l %d0,lyle_dma_src	| D.3369, lyle_dma_src
	move.w #24576,lyle_dma_dest	|, lyle_dma_dest
	rts
	.size	player_dma_setup, .-player_dma_setup
	.align	2
	.globl	player_dma
	.type	player_dma, @function
player_dma:
	moveq #0,%d0	| D.3374
	move.w lyle_dma_len,%d0	| lyle_dma_len, D.3374
	move.l %d0,-(%sp)	| D.3374,
	move.w lyle_dma_dest,%d0	| lyle_dma_dest, D.3374
	move.l %d0,-(%sp)	| D.3374,
	move.l lyle_dma_src,-(%sp)	| lyle_dma_src,
	clr.l -(%sp)	|
	jsr VDP_doDMA	|
	lea (16,%sp),%sp	|,
	rts
	.size	player_dma, .-player_dma
	.align	2
	.globl	player_draw
	.type	player_draw, @function
player_draw:
	move.l %a2,-(%sp)	|,
	move.l %d2,-(%sp)	|,
	move.l 12(%sp),%a2	| pl, pl
	tst.w 38(%a2)	| pl_7(D)->invuln_cnt
	jeq .L17	|
	move.w system_osc,%d0	| system_osc, D.3377
	and.w #7,%d0	|, D.3377
	cmp.w #3,%d0	|, D.3377
	jhi .L16	|
.L17:
	move.w 20(%a2),%d0	| pl_7(D)->anim_frame, D.3377
	cmp.w #15,%d0	|, D.3377
	jls .L28	|
	cmp.w #19,%d0	|, D.3377
	jhi .L21	|
	move.w 14(%a2),%d0	| pl_7(D)->direction, D.3377
	move.w #-4,%a0	|, D.3380
	moveq #-15,%d1	|, D.3380
	move.w #9,%a1	|, D.3378
.L20:
	moveq #11,%d2	|,
	lsl.w %d2,%d0	|, D.3377
	add.w #-7424,%d0	|, D.3377
	move.l %d0,%d2	|,
	and.l #65280,%d2	|,
	move.l %d2,-(%sp)	|,
	move.l %a1,-(%sp)	| D.3378,
	move.l 4(%a2),%d0	| pl_7(D)->y, D.3379
	moveq #10,%d2	|,
	asr.l %d2,%d0	|, D.3379
	sub.w state+10,%d0	| state.cam_y, D.3380
	add.w %d1,%d0	| D.3380, D.3380
	move.w %d0,%a1	| D.3380,
	move.l %a1,-(%sp)	|,
	moveq #-8,%d0	|, D.3380
	sub.w state+8,%d0	| state.cam_x, D.3380
	move.l (%a2),%d1	| pl_7(D)->x, D.3379
	asr.l %d2,%d1	|, D.3379
	add.w %d1,%d0	| D.3379, D.3380
	add.w %a0,%d0	| D.3380, D.3380
	move.w %d0,%a0	| D.3380,
	move.l %a0,-(%sp)	|,
	jsr sprite_put	|
	move.w 36(%a2),%d0	| pl_7(D)->holding_cube, D.3377
	lea (16,%sp),%sp	|,
	jeq .L16	|
	move.w %d0,-(%sp)	| D.3377,
	clr.w -(%sp)	|
	move.l 4(%a2),%d0	| pl_7(D)->y, D.3379
	asr.l %d2,%d0	|, D.3379
	add.w #-38,%d0	|, D.3377
	move.w %d0,-(%sp)	| D.3377,
	clr.w -(%sp)	|
	move.l (%a2),%d0	| pl_7(D)->x, D.3379
	asr.l %d2,%d0	|, D.3379
	subq.w #8,%d0	|, D.3377
	move.w %d0,-(%sp)	| D.3377,
	clr.w -(%sp)	|
	jsr cube_draw_single	|
	lea (12,%sp),%sp	|,
.L16:
	move.l (%sp)+,%d2	|,
	move.l (%sp)+,%a2	|,
	rts
.L28:
	move.w 14(%a2),%d0	| pl_7(D)->direction, D.3377
	sub.l %a0,%a0	| D.3380
	moveq #-23,%d1	|, D.3380
	move.w #6,%a1	|, D.3378
	jra .L20	|
.L21:
	move.w 14(%a2),%d0	| pl_7(D)->direction, D.3377
	jeq .L29	|
	move.w #-8,%a0	|, D.3380
	moveq #-23,%d1	|, D.3380
	move.w #10,%a1	|, D.3378
	jra .L20	|
.L29:
	sub.l %a0,%a0	| D.3380
	moveq #-23,%d1	|, D.3380
	move.w #10,%a1	|, D.3378
	jra .L20	|
	.size	player_draw, .-player_draw
	.align	2
	.globl	player_run
	.type	player_run, @function
player_run:
	movem.l #15416,-(%sp)	|,
	move.l 32(%sp),%a2	| pl, pl
	tst.w 16(%a2)	| pl_2(D)->control_disabled
	jeq .L216	|
	clr.b 46(%a2)	| pl_2(D)->input
	clr.b 47(%a2)	| pl_2(D)->input_prev
	clr.b %d0	| D.3426
.L33:
	move.w 12(%a2),%d1	| pl_2(D)->grounded, D.3425
	jeq .L45	|
	move.w #2,34(%a2)	|, pl_2(D)->cubejump_disable
.L45:
	btst #6,%d0	|, D.3426
	jeq .L48	|
	btst #6,47(%a2)	|, pl_2(D)->input_prev
	jne .L48	|
	tst.w %d1	| D.3425
	jne .L217	|
	tst.w 36(%a2)	| pl_2(D)->holding_cube
	jeq .L48	|
	tst.w sram+52	| sram.have_jump
	jeq .L48	|
	tst.w 34(%a2)	| pl_2(D)->cubejump_disable
	jne .L48	|
	move.w #20,22(%a2)	|, pl_2(D)->throwdown_cnt
	move.l (%a2),%d2	| pl_2(D)->x, D.3428
	moveq #10,%d0	|,
	asr.l %d0,%d2	|, D.3428
	move.w %d2,%d3	| D.3428, cx
	move.w %d2,%d1	|, back_x
	cmp.w #1,14(%a2)	|, pl_2(D)->direction
	jeq .L218	|
	add.w #-9,%d1	|, back_x
.L51:
	move.l 4(%a2),%d0	| pl_2(D)->y, D.3428
	moveq #10,%d4	|,
	asr.l %d4,%d0	|, D.3428
	move.w %d0,-(%sp)	| D.3428,
	clr.w -(%sp)	|
	move.w %d1,-(%sp)	| back_x,
	clr.w -(%sp)	|
	lea map_collision,%a3	|, tmp411
	jsr (%a3)	| tmp411
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L52	|
	and.l #65535,%d2	|, D.3429
	move.l %d2,%d3	| D.3429, D.3429
	addq.l #4,%d3	|, D.3429
	asr.l #3,%d3	|, D.3429
	lsl.w #3,%d3	|, cx
.L52:
	pea 256.w	|
	clr.l -(%sp)	|
	pea 2.w	|
	moveq #0,%d0	| D.3429
	move.w 36(%a2),%d0	| pl_2(D)->holding_cube, D.3429
	move.l %d0,-(%sp)	| D.3429,
	move.l 4(%a2),%d0	| pl_2(D)->y, D.3428
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3428
	add.w #-12,%d0	|, D.3425
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.w %d3,-(%sp)	| cx,
	clr.w -(%sp)	|
	jsr cube_spawn	|
	clr.w 36(%a2)	| pl_2(D)->holding_cube
	lea (24,%sp),%sp	|,
.L49:
	move.w #-229,10(%a2)	|, pl_2(D)->dy
	move.l #-3664,%d3	|, D.3429
	move.w #-229,%d2	|, D.3432
.L47:
	move.w 8(%a2),%d1	| pl_2(D)->dx, D.3424
	move.w %d1,%d0	| D.3424,
	ext.l %d0	| D.3429
	lsl.l #4,%d0	|, D.3429
	add.l (%a2),%d0	| pl_2(D)->x, D.3428
	move.l %d0,(%a2)	| D.3428, pl_2(D)->x
	add.l 4(%a2),%d3	| pl_2(D)->y, D.3428
	move.l %d3,4(%a2)	| D.3428, pl_2(D)->y
	moveq #10,%d4	|,
	asr.l %d4,%d3	|, D.3428
	asr.l %d4,%d0	|, D.3428
	asr.w #6,%d1	|, D.3424
	move.w %d0,%d5	|, px
	sub.w %d1,%d5	| D.3424, px
	tst.w %d2	| D.3432
	jlt .L53	|
.L229:
	move.l %d3,%d2	| D.3428, D.3429
	and.l #65535,%d2	|, D.3429
	move.w %d5,%d4	| px, D.3425
	addq.w #3,%d4	|, D.3425
	and.l #65535,%d4	|, D.3429
	move.l %d2,-(%sp)	| D.3429,
	move.l %d4,-(%sp)	| D.3429,
	jsr (%a3)	| tmp411
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L54	|
	and.w #-8,%d3	|, py
	subq.w #1,%d3	|, py
	and.l #65535,%d3	|, D.3429
	move.l %d3,%d0	| D.3429, D.3429
	moveq #10,%d1	|,
	lsl.l %d1,%d0	|, D.3429
	move.l %d0,4(%a2)	| D.3429, pl_2(D)->y
	clr.w 10(%a2)	| pl_2(D)->dy
	moveq #8,%d2	|, D.3430
	lea map_collision,%a4	|, tmp425
	subq.w #4,%d5	|, D.3425
	and.l #65535,%d5	|, D.3429
.L55:
	move.l %d3,-(%sp)	| D.3429,
	move.l %d4,-(%sp)	| D.3429,
	jsr (%a3)	| tmp411
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L58	|
	add.l #-1024,4(%a2)	|, pl_2(D)->y
	subq.l #1,%d2	|, D.3430
	jne .L55	|
.L60:
	tst.w 10(%a2)	| pl_2(D)->dy
	jlt .L219	|
	move.l %a2,-(%sp)	| pl,
	lea (player_eval_grounded.part.1),%a4	|, tmp331
	jsr (%a4)	| tmp331
	addq.l #4,%sp	|,
	tst.w 12(%a2)	| pl_2(D)->grounded
	jeq .L65	|
.L209:
	move.l 4(%a2),%d3	| pl_2(D)->y, D.3433
.L57:
	moveq #10,%d0	|,
	asr.l %d0,%d3	|, D.3428
	move.l (%a2),%d2	| pl_2(D)->x, D.3428
	asr.l %d0,%d2	|, D.3428
	move.w 8(%a2),%d0	| pl_2(D)->dx, D.3424
	jle .L70	|
.L230:
	move.w %d2,%d4	|, D.3425
	addq.w #4,%d4	|, D.3425
	and.l #65535,%d4	|, D.3429
	move.w %d3,%d0	|, D.3425
	add.w #-18,%d0	|, D.3425
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l %d4,-(%sp)	| D.3429,
	lea map_collision,%a4	|, tmp352
	jsr (%a3)	| tmp411
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L71	|
.L73:
	and.w #-8,%d2	|, px
	addq.w #3,%d2	|, px
	moveq #10,%d1	|,
	lsl.l %d1,%d2	|, D.3429
	and.l #67107840,%d2	|, D.3429
	move.l %d2,(%a2)	| D.3429, pl_2(D)->x
	clr.w 8(%a2)	| pl_2(D)->dx
.L72:
	move.w 10(%a2),%d0	| pl_2(D)->dy, D.3424
	jlt .L220	|
.L77:
	move.l %a2,-(%sp)	| pl,
	jsr (player_eval_grounded.part.1)	|
	addq.l #4,%sp	|,
	tst.w 12(%a2)	| pl_2(D)->grounded
	jeq .L221	|
.L81:
	move.w 36(%a2),%d0	| pl_2(D)->holding_cube, D.3425
	jeq .L83	|
	move.b 46(%a2),%d1	| pl_2(D)->input, D.3426
	btst #5,%d1	|, D.3426
	jeq .L83	|
	btst #5,47(%a2)	|, pl_2(D)->input_prev
	jne .L83	|
	btst #1,%d1	|, D.3426
	jeq .L84	|
	tst.w 14(%a2)	| pl_2(D)->direction
	jne .L139	|
	moveq #1,%d1	|, D.3429
	move.w #-128,%a0	|, D.3429
	move.l %a0,-(%sp)	| D.3429,
	move.l %d1,-(%sp)	| D.3429,
	pea 2.w	|
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l 4(%a2),%d0	| pl_2(D)->y, D.3428
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3428
	add.w #-23,%d0	|, D.3425
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l (%a2),%d0	| pl_2(D)->x, D.3428
	asr.l %d1,%d0	|, D.3428
	move.w %d0,-(%sp)	| D.3428,
	clr.w -(%sp)	|
	jsr cube_spawn	|
	clr.w 36(%a2)	| pl_2(D)->holding_cube
	move.w #2,32(%a2)	|, pl_2(D)->action_cnt
	move.w #8,24(%a2)	|, pl_2(D)->throw_cnt
	lea (24,%sp),%sp	|,
.L83:
	tst.w sram+56	| sram.have_phantom
	jeq .L222	|
	move.w cp_restore_cnt,%d0	| cp_restore_cnt, cp_restore_cnt.15
	addq.w #1,%d0	|, cp_restore_cnt.15
	move.w %d0,cp_restore_cnt	| cp_restore_cnt.15, cp_restore_cnt
	cmp.w #249,%d0	|, cp_restore_cnt.15
	jls .L92	|
	clr.w cp_restore_cnt	| cp_restore_cnt
	move.w 44(%a2),%d0	| pl_2(D)->cp, D.3425
	cmp.w #30,%d0	|, D.3425
	jeq .L92	|
	addq.w #1,%d0	|, D.3425
	move.w %d0,44(%a2)	| D.3425, pl_2(D)->cp
.L92:
	tst.l 28(%a2)	| *pl_2(D)
	jne .L90	|
	tst.w 32(%a2)	| pl_2(D)->action_cnt
	jne .L90	|
	tst.w sram+60	| sram.have_cheap_phantom
	jeq .L144	|
	moveq #2,%d1	|, cube_price
	move.w 36(%a2),%d0	| pl_2(D)->holding_cube, D.3425
	jne .L99	|
.L97:
	move.w 44(%a2),%a0	| pl_2(D)->cp, D.3425
	cmp.w %a0,%d1	| D.3425, cube_price
	jhi .L99	|
	btst #5,46(%a2)	|, pl_2(D)->input
	jeq .L100	|
	move.w 40(%a2),%d0	| pl_2(D)->cp_cnt, D.3425
	addq.w #1,%d0	|, D.3425
	move.w %d0,40(%a2)	| D.3425, pl_2(D)->cp_cnt
.L101:
	tst.w sram+58	| sram.have_fast_phantom
	jeq .L145	|
	move.w #30,%a1	|, cube_spawn_period
.L102:
	cmp.w %a1,%d0	| cube_spawn_period, D.3425
	jcs .L98	|
	move.w ctype,%d0	| ctype, ctype.18
	cmp.w #768,%d0	|, ctype.18
	jeq .L104	|
	jhi .L105	|
	cmp.w #256,%d0	|, ctype.18
	jeq .L106	|
	cmp.w #512,%d0	|, ctype.18
	jne .L104	|
	move.w #256,ctype	|, ctype
	move.w #256,%d0	|, D.3425
	move.w %d0,36(%a2)	| D.3425, pl_2(D)->holding_cube
	clr.w 40(%a2)	| pl_2(D)->cp_cnt
	sub.w %d1,%a0	| cube_price, D.3425
	move.w %a0,44(%a2)	| D.3425, pl_2(D)->cp
.L90:
	tst.w 10(%a2)	| pl_2(D)->dy
	jlt .L223	|
.L111:
	move.l %a2,-(%sp)	| pl,
	jsr (player_eval_grounded.part.1)	|
	addq.l #4,%sp	|,
.L112:
	tst.w 38(%a2)	| pl_2(D)->invuln_cnt
	jeq .L113	|
	move.w system_osc,%d0	| system_osc, D.3425
	and.w #7,%d0	|, D.3425
	cmp.w #3,%d0	|, D.3425
	jhi .L114	|
.L113:
	move.w 12(%a2),%d0	| pl_2(D)->grounded, D.3425
	jeq .L115	|
	move.w 18(%a2),%d1	| pl_2(D)->anim_cnt, D.3425
	addq.w #1,%d1	|, D.3425
	cmp.w #24,%d1	|, D.3425
	jeq .L115	|
	move.w %d1,18(%a2)	| D.3425, pl_2(D)->anim_cnt
	tst.w 24(%a2)	| pl_2(D)->throw_cnt
	jne .L224	|
.L118:
	tst.w 22(%a2)	| pl_2(D)->throwdown_cnt
	jne .L225	|
	tst.w 26(%a2)	| pl_2(D)->kick_cnt
	jne .L226	|
	tst.w 28(%a2)	| pl_2(D)->lift_cnt
	jne .L227	|
	tst.w 30(%a2)	| pl_2(D)->hurt_cnt
	jne .L228	|
	tst.w %d0	| D.3425
	jeq .L123	|
	move.b 46(%a2),%d0	| pl_2(D)->input, D.3426
	and.b #12,%d0	|, D.3426
	jne .L124	|
	clr.w 20(%a2)	| pl_2(D)->anim_frame
.L125:
	cmp.w #5,40(%a2)	|, pl_2(D)->cp_cnt
	jhi .L129	|
	tst.w 36(%a2)	| pl_2(D)->holding_cube
	jeq .L114	|
.L129:
	addq.w #8,20(%a2)	|, pl_2(D)->anim_frame
.L114:
	move.l %a2,-(%sp)	| pl,
	jsr player_dma_setup	|
	move.w 22(%a2),%d0	| pl_2(D)->throwdown_cnt, D.3425
	addq.l #4,%sp	|,
	jeq .L131	|
	subq.w #1,%d0	|, D.3425
	move.w %d0,22(%a2)	| D.3425, pl_2(D)->throwdown_cnt
.L131:
	move.w 24(%a2),%d0	| pl_2(D)->throw_cnt, D.3425
	jeq .L132	|
	subq.w #1,%d0	|, D.3425
	move.w %d0,24(%a2)	| D.3425, pl_2(D)->throw_cnt
.L132:
	move.w 28(%a2),%d0	| pl_2(D)->lift_cnt, D.3425
	jeq .L133	|
	subq.w #1,%d0	|, D.3425
	move.w %d0,28(%a2)	| D.3425, pl_2(D)->lift_cnt
.L133:
	move.w 30(%a2),%d0	| pl_2(D)->hurt_cnt, D.3425
	jeq .L134	|
	subq.w #1,%d0	|, D.3425
	move.w %d0,30(%a2)	| D.3425, pl_2(D)->hurt_cnt
.L134:
	move.w 38(%a2),%d0	| pl_2(D)->invuln_cnt, D.3425
	jeq .L135	|
	subq.w #1,%d0	|, D.3425
	move.w %d0,38(%a2)	| D.3425, pl_2(D)->invuln_cnt
.L135:
	move.w 32(%a2),%d0	| pl_2(D)->action_cnt, D.3425
	jeq .L136	|
	subq.w #1,%d0	|, D.3425
	move.w %d0,32(%a2)	| D.3425, pl_2(D)->action_cnt
.L136:
	move.w 34(%a2),%d0	| pl_2(D)->cubejump_disable, D.3425
	jeq .L31	|
	subq.w #1,%d0	|, D.3425
	move.w %d0,34(%a2)	| D.3425, pl_2(D)->cubejump_disable
.L31:
	movem.l (%sp)+,#7228	|,
	rts
.L48:
	move.w 10(%a2),%d2	| pl_2(D)->dy, D.3432
	move.w %d2,%d3	| D.3432,
	ext.l %d3	| D.3429
	lsl.l #4,%d3	|, D.3429
	lea map_collision,%a3	|, tmp411
	move.w 8(%a2),%d1	| pl_2(D)->dx, D.3424
	move.w %d1,%d0	| D.3424,
	ext.l %d0	| D.3429
	lsl.l #4,%d0	|, D.3429
	add.l (%a2),%d0	| pl_2(D)->x, D.3428
	move.l %d0,(%a2)	| D.3428, pl_2(D)->x
	add.l 4(%a2),%d3	| pl_2(D)->y, D.3428
	move.l %d3,4(%a2)	| D.3428, pl_2(D)->y
	moveq #10,%d4	|,
	asr.l %d4,%d3	|, D.3428
	asr.l %d4,%d0	|, D.3428
	asr.w #6,%d1	|, D.3424
	move.w %d0,%d5	|, px
	sub.w %d1,%d5	| D.3424, px
	tst.w %d2	| D.3432
	jge .L229	|
.L53:
	move.w %d3,%d2	|, D.3425
	add.w #-19,%d2	|, D.3425
	and.l #65535,%d2	|, D.3429
	move.l %d2,-(%sp)	| D.3429,
	move.w %d5,%d0	| px, D.3425
	addq.w #3,%d0	|, D.3425
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	jsr (%a3)	| tmp411
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L67	|
	and.l #65535,%d3	|, D.3429
	addq.l #4,%d3	|, D.3429
	asr.l #3,%d3	|, D.3429
	lsl.w #3,%d3	|, py
	addq.w #3,%d3	|, py
	and.l #65535,%d3	|, D.3429
	moveq #10,%d4	|,
	lsl.l %d4,%d3	|, D.3429
	move.l %d3,4(%a2)	| D.3429, pl_2(D)->y
	cmp.w #-32,10(%a2)	|, pl_2(D)->dy
	jge .L57	|
.L68:
	move.w #-32,10(%a2)	|, pl_2(D)->dy
	moveq #10,%d0	|,
	asr.l %d0,%d3	|, D.3428
	move.l (%a2),%d2	| pl_2(D)->x, D.3428
	asr.l %d0,%d2	|, D.3428
	move.w 8(%a2),%d0	| pl_2(D)->dx, D.3424
	jgt .L230	|
.L70:
	tst.w %d0	| D.3424
	jeq .L72	|
	move.w %d2,%d4	|, D.3425
	subq.w #5,%d4	|, D.3425
	and.l #65535,%d4	|, D.3429
	move.w %d3,%d0	|, D.3425
	add.w #-18,%d0	|, D.3425
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l %d4,-(%sp)	| D.3429,
	lea map_collision,%a4	|, tmp365
	jsr (%a3)	| tmp411
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L75	|
.L76:
	and.w #-8,%d2	|, px
	addq.w #5,%d2	|, px
	moveq #10,%d4	|,
	lsl.l %d4,%d2	|, D.3429
	and.l #67107840,%d2	|, D.3429
	move.l %d2,(%a2)	| D.3429, pl_2(D)->x
	clr.w 8(%a2)	| pl_2(D)->dx
.L233:
	move.w 10(%a2),%d0	| pl_2(D)->dy, D.3424
	jge .L77	|
.L220:
	clr.w 12(%a2)	| pl_2(D)->grounded
	btst #6,46(%a2)	|, pl_2(D)->input
	jne .L78	|
.L79:
	add.w #17,%d0	|, D.3424
	move.w %d0,10(%a2)	| D.3424, pl_2(D)->dy
	cmp.w #512,%d0	|, D.3424
	jle .L81	|
	move.w #512,10(%a2)	|, pl_2(D)->dy
	jra .L81	|
.L58:
	move.l %d3,-(%sp)	| D.3429,
	move.l %d5,-(%sp)	| D.3429,
	jsr (%a4)	| tmp425
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L60	|
	add.l #-1024,4(%a2)	|, pl_2(D)->y
	subq.l #1,%d2	|, D.3430
	jne .L55	|
	jra .L60	|
.L71:
	move.w %d3,%d0	|, D.3425
	subq.w #3,%d0	|, D.3425
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l %d4,-(%sp)	| D.3429,
	jsr (%a4)	| tmp352
	addq.l #8,%sp	|,
	tst.w %d0	|
	jne .L73	|
	add.w #-11,%d3	|, D.3425
	move.w %d3,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l %d4,-(%sp)	| D.3429,
	jsr (%a4)	| tmp352
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L72	|
	and.w #-8,%d2	|, px
	addq.w #3,%d2	|, px
	moveq #10,%d1	|,
	lsl.l %d1,%d2	|, D.3429
	and.l #67107840,%d2	|, D.3429
	move.l %d2,(%a2)	| D.3429, pl_2(D)->x
	clr.w 8(%a2)	| pl_2(D)->dx
	jra .L72	|
.L115:
	clr.w 18(%a2)	| pl_2(D)->anim_cnt
	tst.w 24(%a2)	| pl_2(D)->throw_cnt
	jeq .L118	|
.L224:
	move.w #22,20(%a2)	|, pl_2(D)->anim_frame
	jra .L114	|
.L222:
	clr.w 40(%a2)	| pl_2(D)->cp_cnt
	tst.w 10(%a2)	| pl_2(D)->dy
	jge .L111	|
.L223:
	clr.w 12(%a2)	| pl_2(D)->grounded
	jra .L112	|
.L219:
	move.l 4(%a2),%d3	| pl_2(D)->y, D.3428
	add.l #8192,%d3	|, D.3428
	move.l %d3,4(%a2)	| D.3428, pl_2(D)->y
.L64:
	clr.w 12(%a2)	| pl_2(D)->grounded
	jra .L57	|
.L216:
	move.b 46(%a2),47(%a2)	| pl_2(D)->input, pl_2(D)->input_prev
	clr.l -(%sp)	|
	jsr pad_read	|
	move.b %d0,46(%a2)	| D.3426, pl_2(D)->input
	addq.l #4,%sp	|,
	tst.w 16(%a2)	| pl_2(D)->control_disabled
	jne .L33	|
	move.w 8(%a2),%d1	| pl_2(D)->dx, D.3424
	btst #3,%d0	|, D.3426
	jeq .L35	|
	add.w #9,%d1	|, D.3424
	move.w %d1,8(%a2)	| D.3424, pl_2(D)->dx
	clr.w 14(%a2)	| pl_2(D)->direction
.L36:
	move.b %d0,%d2	| D.3426, D.3426
	and.b #12,%d2	|, D.3426
	tst.w %d1	| D.3432
	jle .L38	|
.L232:
	tst.b %d2	| D.3426
	jne .L39	|
	add.w #-9,%d1	|, D.3424
	move.w %d1,%d3	| D.3424, D.3424
	asr.w #6,%d3	|, D.3424
	cmp.w #-1,%d3	|, D.3424
	jeq .L43	|
.L207:
	move.w %d1,8(%a2)	| D.3424, pl_2(D)->dx
.L39:
	move.w %d1,%d3	| D.3432, D.3427
	addq.w #5,%d3	|, D.3427
	cmp.w #10,%d3	|, D.3427
	jls .L41	|
	cmp.w #108,%d1	|, D.3432
	jgt .L231	|
.L44:
	cmp.w #-108,%d1	|, D.3432
	jge .L33	|
	move.w #-108,8(%a2)	|, pl_2(D)->dx
	jra .L33	|
.L41:
	tst.b %d2	| D.3426
	jeq .L43	|
	move.w 8(%a2),%d1	| pl_2(D)->dx, D.3432
	move.b 46(%a2),%d0	| pl_2(D)->input, D.3426
	cmp.w #108,%d1	|, D.3432
	jle .L44	|
.L231:
	move.w #108,8(%a2)	|, pl_2(D)->dx
	jra .L33	|
.L35:
	btst #2,%d0	|, D.3426
	jeq .L36	|
	add.w #-9,%d1	|, D.3424
	move.w %d1,8(%a2)	| D.3424, pl_2(D)->dx
	move.w #1,14(%a2)	|, pl_2(D)->direction
	move.b %d0,%d2	| D.3426, D.3426
	and.b #12,%d2	|, D.3426
	tst.w %d1	| D.3432
	jgt .L232	|
.L38:
	tst.w %d1	| D.3432
	jeq .L41	|
	tst.b %d2	| D.3426
	jne .L39	|
	add.w #9,%d1	|, D.3424
	tst.w %d1	| D.3424
	jle .L207	|
.L43:
	clr.w 8(%a2)	| pl_2(D)->dx
	move.b 46(%a2),%d0	| pl_2(D)->input, D.3426
	jra .L33	|
.L221:
	move.w 10(%a2),%d0	| pl_2(D)->dy, D.3432
	btst #6,46(%a2)	|, pl_2(D)->input
	jeq .L79	|
	tst.w %d0	| D.3432
	jge .L79	|
.L78:
	add.w #9,%d0	|, D.3432
	move.w %d0,10(%a2)	| D.3432, pl_2(D)->dy
	jra .L81	|
.L217:
	lea map_collision,%a3	|, tmp411
	move.w #-229,10(%a2)	|, pl_2(D)->dy
	move.l #-3664,%d3	|, D.3429
	move.w #-229,%d2	|, D.3432
	jra .L47	|
.L54:
	move.l %d2,-(%sp)	| D.3429,
	move.w %d5,%d0	| px, D.3425
	subq.w #4,%d0	|, D.3425
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	jsr map_collision	|
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L209	|
	and.w #-8,%d3	|, py
	subq.w #1,%d3	|, py
	and.l #65535,%d3	|, D.3429
	move.l %d3,%d0	| D.3429, D.3429
	moveq #10,%d1	|,
	lsl.l %d1,%d0	|, D.3429
	move.l %d0,4(%a2)	| D.3429, pl_2(D)->y
	clr.w 10(%a2)	| pl_2(D)->dy
	moveq #8,%d2	|, D.3430
	lea map_collision,%a4	|, tmp425
	subq.w #4,%d5	|, D.3425
	and.l #65535,%d5	|, D.3429
	jra .L55	|
.L226:
	move.w #23,20(%a2)	|, pl_2(D)->anim_frame
	jra .L114	|
.L144:
	moveq #4,%d1	|, cube_price
	move.w 36(%a2),%d0	| pl_2(D)->holding_cube, D.3425
	jeq .L97	|
.L99:
	move.w 40(%a2),%d0	| pl_2(D)->cp_cnt, D.3425
.L98:
	cmp.w #5,%d0	|, D.3425
	jls .L90	|
	btst #0,system_osc+1	|, system_osc
	jeq .L90	|
	clr.l -(%sp)	|
	move.l 4(%a2),%d0	| pl_2(D)->y, D.3428
	moveq #10,%d4	|,
	asr.l %d4,%d0	|, D.3428
	add.w #-32,%d0	|, D.3425
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l (%a2),%d0	| pl_2(D)->x, D.3428
	asr.l %d4,%d0	|, D.3428
	move.w %d0,-(%sp)	| D.3428,
	clr.w -(%sp)	|
	jsr particle_spawn	|
	lea (12,%sp),%sp	|,
	jra .L90	|
.L84:
	btst #0,%d1	|, D.3426
	jne .L140	|
	move.w 14(%a2),%d2	| pl_2(D)->direction, D.3425
	btst #3,%d1	|, D.3426
	jeq .L87	|
	tst.w %d2	| D.3425
	jne .L87	|
	moveq #4,%d1	|, D.3429
	move.w #-64,%a0	|, D.3429
	move.l %a0,-(%sp)	| D.3429,
	move.l %d1,-(%sp)	| D.3429,
	pea 2.w	|
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l 4(%a2),%d0	| pl_2(D)->y, D.3428
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3428
	add.w #-23,%d0	|, D.3425
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l (%a2),%d0	| pl_2(D)->x, D.3428
	asr.l %d1,%d0	|, D.3428
	move.w %d0,-(%sp)	| D.3428,
	clr.w -(%sp)	|
	jsr cube_spawn	|
	clr.w 36(%a2)	| pl_2(D)->holding_cube
	move.w #2,32(%a2)	|, pl_2(D)->action_cnt
	move.w #8,24(%a2)	|, pl_2(D)->throw_cnt
	lea (24,%sp),%sp	|,
	jra .L83	|
.L65:
	move.l 4(%a2),%d3	| pl_2(D)->y, D.3428
	add.l #8192,%d3	|, D.3428
	move.l %d3,4(%a2)	| D.3428, pl_2(D)->y
	tst.w 10(%a2)	| pl_2(D)->dy
	jlt .L64	|
	move.l %a2,-(%sp)	| pl,
	jsr (%a4)	| tmp331
	move.l 4(%a2),%d3	| pl_2(D)->y, D.3433
	addq.l #4,%sp	|,
	jra .L57	|
.L225:
	move.w #7,20(%a2)	|, pl_2(D)->anim_frame
	jra .L114	|
.L227:
	move.w #5,20(%a2)	|, pl_2(D)->anim_frame
	jra .L114	|
.L140:
	moveq #0,%d1	| D.3429
	move.w #-320,%a0	|, D.3429
	move.l %a0,-(%sp)	| D.3429,
	move.l %d1,-(%sp)	| D.3429,
	pea 2.w	|
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l 4(%a2),%d0	| pl_2(D)->y, D.3428
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3428
	add.w #-23,%d0	|, D.3425
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l (%a2),%d0	| pl_2(D)->x, D.3428
	asr.l %d1,%d0	|, D.3428
	move.w %d0,-(%sp)	| D.3428,
	clr.w -(%sp)	|
	jsr cube_spawn	|
	clr.w 36(%a2)	| pl_2(D)->holding_cube
	move.w #2,32(%a2)	|, pl_2(D)->action_cnt
	move.w #8,24(%a2)	|, pl_2(D)->throw_cnt
	lea (24,%sp),%sp	|,
	jra .L83	|
.L139:
	moveq #-1,%d1	|, D.3429
	move.w #-128,%a0	|, D.3429
	move.l %a0,-(%sp)	| D.3429,
	move.l %d1,-(%sp)	| D.3429,
	pea 2.w	|
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l 4(%a2),%d0	| pl_2(D)->y, D.3428
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3428
	add.w #-23,%d0	|, D.3425
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l (%a2),%d0	| pl_2(D)->x, D.3428
	asr.l %d1,%d0	|, D.3428
	move.w %d0,-(%sp)	| D.3428,
	clr.w -(%sp)	|
	jsr cube_spawn	|
	clr.w 36(%a2)	| pl_2(D)->holding_cube
	move.w #2,32(%a2)	|, pl_2(D)->action_cnt
	move.w #8,24(%a2)	|, pl_2(D)->throw_cnt
	lea (24,%sp),%sp	|,
	jra .L83	|
.L123:
	move.w #4,20(%a2)	|, pl_2(D)->anim_frame
	jra .L125	|
.L87:
	btst #2,%d1	|, D.3426
	jeq .L88	|
	cmp.w #1,%d2	|, D.3425
	jeq .L142	|
.L88:
	tst.w %d2	| D.3425
	jne .L143	|
	moveq #2,%d1	|, D.3429
	move.w #-128,%a0	|, D.3429
	move.l %a0,-(%sp)	| D.3429,
	move.l %d1,-(%sp)	| D.3429,
	pea 2.w	|
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l 4(%a2),%d0	| pl_2(D)->y, D.3428
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3428
	add.w #-23,%d0	|, D.3425
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l (%a2),%d0	| pl_2(D)->x, D.3428
	asr.l %d1,%d0	|, D.3428
	move.w %d0,-(%sp)	| D.3428,
	clr.w -(%sp)	|
	jsr cube_spawn	|
	clr.w 36(%a2)	| pl_2(D)->holding_cube
	move.w #2,32(%a2)	|, pl_2(D)->action_cnt
	move.w #8,24(%a2)	|, pl_2(D)->throw_cnt
	lea (24,%sp),%sp	|,
	jra .L83	|
.L100:
	clr.w 40(%a2)	| pl_2(D)->cp_cnt
	jra .L101	|
.L67:
	move.l %d2,-(%sp)	| D.3429,
	subq.w #4,%d5	|, D.3425
	move.w %d5,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	jsr map_collision	|
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L209	|
	and.l #65535,%d3	|, D.3429
	addq.l #4,%d3	|, D.3429
	asr.l #3,%d3	|, D.3429
	lsl.w #3,%d3	|, py
	addq.w #3,%d3	|, py
	and.l #65535,%d3	|, D.3429
	moveq #10,%d4	|,
	lsl.l %d4,%d3	|, D.3429
	move.l %d3,4(%a2)	| D.3429, pl_2(D)->y
	cmp.w #-32,10(%a2)	|, pl_2(D)->dy
	jlt .L68	|
	jra .L57	|
.L75:
	move.w %d3,%d0	|, D.3425
	subq.w #3,%d0	|, D.3425
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l %d4,-(%sp)	| D.3429,
	jsr (%a4)	| tmp365
	addq.l #8,%sp	|,
	tst.w %d0	|
	jne .L76	|
	add.w #-11,%d3	|, D.3425
	move.w %d3,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l %d4,-(%sp)	| D.3429,
	jsr (%a4)	| tmp365
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L72	|
	and.w #-8,%d2	|, px
	addq.w #5,%d2	|, px
	moveq #10,%d4	|,
	lsl.l %d4,%d2	|, D.3429
	and.l #67107840,%d2	|, D.3429
	move.l %d2,(%a2)	| D.3429, pl_2(D)->x
	clr.w 8(%a2)	| pl_2(D)->dx
	jra .L233	|
.L142:
	moveq #-4,%d1	|, D.3429
	move.w #-64,%a0	|, D.3429
	move.l %a0,-(%sp)	| D.3429,
	move.l %d1,-(%sp)	| D.3429,
	pea 2.w	|
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l 4(%a2),%d0	| pl_2(D)->y, D.3428
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3428
	add.w #-23,%d0	|, D.3425
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l (%a2),%d0	| pl_2(D)->x, D.3428
	asr.l %d1,%d0	|, D.3428
	move.w %d0,-(%sp)	| D.3428,
	clr.w -(%sp)	|
	jsr cube_spawn	|
	clr.w 36(%a2)	| pl_2(D)->holding_cube
	move.w #2,32(%a2)	|, pl_2(D)->action_cnt
	move.w #8,24(%a2)	|, pl_2(D)->throw_cnt
	lea (24,%sp),%sp	|,
	jra .L83	|
.L228:
	move.w #6,20(%a2)	|, pl_2(D)->anim_frame
	jra .L114	|
.L145:
	move.w #60,%a1	|, cube_spawn_period
	jra .L102	|
.L143:
	moveq #-2,%d1	|, D.3429
	move.w #-128,%a0	|, D.3429
	move.l %a0,-(%sp)	| D.3429,
	move.l %d1,-(%sp)	| D.3429,
	pea 2.w	|
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l 4(%a2),%d0	| pl_2(D)->y, D.3428
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3428
	add.w #-23,%d0	|, D.3425
	move.w %d0,-(%sp)	| D.3425,
	clr.w -(%sp)	|
	move.l (%a2),%d0	| pl_2(D)->x, D.3428
	asr.l %d1,%d0	|, D.3428
	move.w %d0,-(%sp)	| D.3428,
	clr.w -(%sp)	|
	jsr cube_spawn	|
	clr.w 36(%a2)	| pl_2(D)->holding_cube
	move.w #2,32(%a2)	|, pl_2(D)->action_cnt
	move.w #8,24(%a2)	|, pl_2(D)->throw_cnt
	lea (24,%sp),%sp	|,
	jra .L83	|
.L124:
	move.w 18(%a2),%d0	| pl_2(D)->anim_cnt, D.3425
	cmp.w #5,%d0	|, D.3425
	jls .L128	|
	cmp.w #11,%d0	|, D.3425
	jhi .L127	|
	move.w #3,20(%a2)	|, pl_2(D)->anim_frame
	jra .L125	|
.L218:
	addq.w #8,%d1	|, back_x
	jra .L51	|
.L105:
	cmp.w #1024,%d0	|, ctype.18
	jeq .L108	|
	cmp.w #2048,%d0	|, ctype.18
	jne .L104	|
	move.w #768,ctype	|, ctype
	move.w #768,%d0	|, D.3425
	move.w %d0,36(%a2)	| D.3425, pl_2(D)->holding_cube
	clr.w 40(%a2)	| pl_2(D)->cp_cnt
	sub.w %d1,%a0	| cube_price, D.3425
	move.w %a0,44(%a2)	| D.3425, pl_2(D)->cp
	jra .L90	|
.L128:
	move.w #2,20(%a2)	|, pl_2(D)->anim_frame
	jra .L125	|
.L104:
	move.w #512,ctype	|, ctype
	move.w #512,%d0	|, D.3425
	move.w %d0,36(%a2)	| D.3425, pl_2(D)->holding_cube
	clr.w 40(%a2)	| pl_2(D)->cp_cnt
	sub.w %d1,%a0	| cube_price, D.3425
	move.w %a0,44(%a2)	| D.3425, pl_2(D)->cp
	jra .L90	|
.L106:
	move.w #1024,ctype	|, ctype
	move.w #1024,%d0	|, D.3425
	move.w %d0,36(%a2)	| D.3425, pl_2(D)->holding_cube
	clr.w 40(%a2)	| pl_2(D)->cp_cnt
	sub.w %d1,%a0	| cube_price, D.3425
	move.w %a0,44(%a2)	| D.3425, pl_2(D)->cp
	jra .L90	|
.L108:
	move.w #2048,ctype	|, ctype
	move.w #2048,%d0	|, D.3425
	move.w %d0,36(%a2)	| D.3425, pl_2(D)->holding_cube
	clr.w 40(%a2)	| pl_2(D)->cp_cnt
	sub.w %d1,%a0	| cube_price, D.3425
	move.w %a0,44(%a2)	| D.3425, pl_2(D)->cp
	jra .L90	|
.L127:
	cmp.w #17,%d0	|, D.3425
	jls .L128	|
	move.w #1,20(%a2)	|, pl_2(D)->anim_frame
	jra .L125	|
	.size	player_run, .-player_run
	.local	cp_restore_cnt
	.comm	cp_restore_cnt,2,2
	.local	ctype
	.comm	ctype,2,2
	.local	lyle_dma_len
	.comm	lyle_dma_len,2,2
	.local	lyle_dma_dest
	.comm	lyle_dma_dest,2,2
	.local	lyle_dma_src
	.comm	lyle_dma_src,4,2
	.ident	"GCC: (GNU) 4.8.2"
