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
	.type	player_eval_grounded.part.0, @function
player_eval_grounded.part.0:
	movem.l #12336,-(%sp)	|,
	move.l 20(%sp),%a2	| pl, pl
	move.l (%a2),%d2	| pl_1(D)->x, D.3351
	moveq #10,%d0	|,
	asr.l %d0,%d2	|, D.3351
	move.l 4(%a2),%d0	| pl_1(D)->y, D.3351
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3351
	addq.w #1,%d0	|, D.3352
	moveq #0,%d3	| D.3353
	move.w %d0,%d3	| D.3352, D.3353
	move.l %d3,-(%sp)	| D.3353,
	move.w %d2,%d0	|, D.3352
	addq.w #3,%d0	|, D.3352
	move.w %d0,-(%sp)	| D.3352,
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
	move.l %d3,-(%sp)	| D.3353,
	subq.w #4,%d2	|, D.3352
	move.w %d2,-(%sp)	| D.3352,
	clr.w -(%sp)	|
	jsr (%a3)	| tmp53
	addq.l #8,%sp	|,
	tst.w %d0	|
	jne .L4	|
	clr.w 12(%a2)	| pl_1(D)->grounded
	movem.l (%sp)+,#3084	|,
	rts
	.size	player_eval_grounded.part.0, .-player_eval_grounded.part.0
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
	and.l #65535,%d0	|, D.3361
	lsl.l #5,%d0	|, D.3361
	add.l #gfx_lyle,%d0	|, D.3361
	move.l %d0,lyle_dma_src	| D.3361, lyle_dma_src
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
	and.l #65535,%d0	|, D.3361
	lsl.l #5,%d0	|, D.3361
	add.l #gfx_lyle,%d0	|, D.3361
	move.l %d0,lyle_dma_src	| D.3361, lyle_dma_src
	move.w #24576,lyle_dma_dest	|, lyle_dma_dest
	rts
	.size	player_dma_setup, .-player_dma_setup
	.align	2
	.globl	player_dma
	.type	player_dma, @function
player_dma:
	moveq #0,%d0	| D.3366
	move.w lyle_dma_len,%d0	| lyle_dma_len, D.3366
	move.l %d0,-(%sp)	| D.3366,
	move.w lyle_dma_dest,%d0	| lyle_dma_dest, D.3366
	move.l %d0,-(%sp)	| D.3366,
	move.l lyle_dma_src,-(%sp)	| lyle_dma_src,
	clr.l -(%sp)	|
	jsr VDP_doDMA	|
	lea (16,%sp),%sp	|,
	rts
	.size	player_dma, .-player_dma
	.align	2
	.globl	player_input
	.type	player_input, @function
player_input:
	move.l %a2,-(%sp)	|,
	move.l 8(%sp),%a2	| pl, pl
	tst.w 16(%a2)	| pl_3(D)->control_disabled
	jeq .L20	|
	clr.b 46(%a2)	| pl_3(D)->input
	clr.b 47(%a2)	| pl_3(D)->input_prev
	move.l (%sp)+,%a2	|,
	rts
.L20:
	move.b 46(%a2),47(%a2)	| pl_3(D)->input, pl_3(D)->input_prev
	clr.l -(%sp)	|
	jsr pad_read	|
	move.b %d0,46(%a2)	|, pl_3(D)->input
	addq.l #4,%sp	|,
	move.l (%sp)+,%a2	|,
	rts
	.size	player_input, .-player_input
	.align	2
	.globl	player_cp
	.type	player_cp, @function
player_cp:
	move.l %d2,-(%sp)	|,
	move.l 8(%sp),%a0	| pl, pl
	tst.w sram+56	| sram.have_phantom
	jeq .L53	|
	move.w cp_restore_cnt,%d0	| cp_restore_cnt, cp_restore_cnt.8
	addq.w #1,%d0	|, cp_restore_cnt.8
	move.w %d0,cp_restore_cnt	| cp_restore_cnt.8, cp_restore_cnt
	cmp.w #249,%d0	|, cp_restore_cnt.8
	jls .L26	|
	clr.w cp_restore_cnt	| cp_restore_cnt
	move.w 44(%a0),%d0	| pl_10(D)->cp, D.3373
	cmp.w #30,%d0	|, D.3373
	jeq .L26	|
	addq.w #1,%d0	|, D.3373
	move.w %d0,44(%a0)	| D.3373, pl_10(D)->cp
.L26:
	tst.l 28(%a0)	| *pl_10(D)
	jne .L22	|
	tst.w 32(%a0)	| pl_10(D)->action_cnt
	jne .L22	|
	tst.w sram+60	| sram.have_cheap_phantom
	jeq .L44	|
	moveq #2,%d1	|, cube_price
	move.w 36(%a0),%d0	| pl_10(D)->holding_cube, D.3373
	jne .L33	|
.L31:
	move.w 44(%a0),%a1	| pl_10(D)->cp, D.3373
	cmp.w %a1,%d1	| D.3373, cube_price
	jhi .L33	|
	btst #5,46(%a0)	|, pl_10(D)->input
	jeq .L34	|
	move.w 40(%a0),%d0	| pl_10(D)->cp_cnt, D.3373
	addq.w #1,%d0	|, D.3373
	move.w %d0,40(%a0)	| D.3373, pl_10(D)->cp_cnt
.L35:
	tst.w sram+58	| sram.have_fast_phantom
	jeq .L45	|
	moveq #30,%d2	|, cube_spawn_period
.L36:
	cmp.w %d2,%d0	| cube_spawn_period, D.3373
	jcs .L32	|
	move.w ctype,%d0	| ctype, ctype.11
	cmp.w #512,%d0	|, ctype.11
	jeq .L38	|
	jls .L54	|
	cmp.w #1024,%d0	|, ctype.11
	jeq .L41	|
.L42:
	move.w #512,ctype	|, ctype
	move.w #512,%d0	|, D.3373
	move.w %d0,36(%a0)	| D.3373, pl_10(D)->holding_cube
	clr.w 40(%a0)	| pl_10(D)->cp_cnt
	sub.w %d1,%a1	| cube_price, D.3373
	move.w %a1,44(%a0)	| D.3373, pl_10(D)->cp
.L22:
	move.l (%sp)+,%d2	|,
	rts
.L53:
	clr.w 40(%a0)	| pl_10(D)->cp_cnt
	move.l (%sp)+,%d2	|,
	rts
.L44:
	moveq #4,%d1	|, cube_price
	move.w 36(%a0),%d0	| pl_10(D)->holding_cube, D.3373
	jeq .L31	|
.L33:
	move.w 40(%a0),%d0	| pl_10(D)->cp_cnt, D.3373
.L32:
	cmp.w #5,%d0	|, D.3373
	jls .L22	|
	btst #0,system_osc+1	|, system_osc
	jeq .L22	|
	clr.l -(%sp)	|
	move.l 4(%a0),%d0	| pl_10(D)->y, D.3376
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3376
	add.w #-32,%d0	|, D.3373
	move.w %d0,-(%sp)	| D.3373,
	clr.w -(%sp)	|
	move.l (%a0),%d0	| pl_10(D)->x, D.3376
	asr.l %d1,%d0	|, D.3376
	move.w %d0,-(%sp)	| D.3376,
	clr.w -(%sp)	|
	jsr particle_spawn	|
	lea (12,%sp),%sp	|,
	move.l (%sp)+,%d2	|,
	rts
.L34:
	clr.w 40(%a0)	| pl_10(D)->cp_cnt
	jra .L35	|
.L41:
	move.w #2048,ctype	|, ctype
	move.w #2048,%d0	|, D.3373
	move.w %d0,36(%a0)	| D.3373, pl_10(D)->holding_cube
	clr.w 40(%a0)	| pl_10(D)->cp_cnt
	sub.w %d1,%a1	| cube_price, D.3373
	move.w %a1,44(%a0)	| D.3373, pl_10(D)->cp
	jra .L22	|
.L45:
	moveq #60,%d2	|, cube_spawn_period
	jra .L36	|
.L54:
	cmp.w #256,%d0	|, ctype.11
	jne .L42	|
	move.w #1024,ctype	|, ctype
	move.w #1024,%d0	|, D.3373
	move.w %d0,36(%a0)	| D.3373, pl_10(D)->holding_cube
	clr.w 40(%a0)	| pl_10(D)->cp_cnt
	sub.w %d1,%a1	| cube_price, D.3373
	move.w %a1,44(%a0)	| D.3373, pl_10(D)->cp
	jra .L22	|
.L38:
	move.w #256,ctype	|, ctype
	move.w #256,%d0	|, D.3373
	move.w %d0,36(%a0)	| D.3373, pl_10(D)->holding_cube
	clr.w 40(%a0)	| pl_10(D)->cp_cnt
	sub.w %d1,%a1	| cube_price, D.3373
	move.w %a1,44(%a0)	| D.3373, pl_10(D)->cp
	jra .L22	|
	.size	player_cp, .-player_cp
	.align	2
	.globl	player_accel
	.type	player_accel, @function
player_accel:
	move.l %d2,-(%sp)	|,
	move.l 8(%sp),%a0	| pl, pl
	tst.w 16(%a0)	| pl_6(D)->control_disabled
	jne .L56	|
	move.b 46(%a0),%d1	| pl_6(D)->input, D.3382
	move.w 8(%a0),%d0	| pl_6(D)->dx, D.3383
	btst #3,%d1	|, D.3382
	jeq .L59	|
	add.w #9,%d0	|, D.3383
	move.w %d0,8(%a0)	| D.3383, pl_6(D)->dx
	clr.w 14(%a0)	| pl_6(D)->direction
.L60:
	and.b #12,%d1	|, D.3382
	tst.w %d0	| D.3385
	jle .L62	|
.L82:
	tst.b %d1	| D.3382
	jne .L63	|
	add.w #-9,%d0	|, D.3383
	move.w %d0,%d2	| D.3383, D.3383
	asr.w #6,%d2	|, D.3383
	cmp.w #-1,%d2	|, D.3383
	jeq .L67	|
.L79:
	move.w %d0,8(%a0)	| D.3383, pl_6(D)->dx
.L63:
	move.w %d0,%d2	| D.3385, D.3384
	addq.w #5,%d2	|, D.3384
	cmp.w #10,%d2	|, D.3384
	jls .L65	|
	cmp.w #108,%d0	|, D.3385
	jgt .L81	|
.L68:
	cmp.w #-108,%d0	|, D.3385
	jge .L56	|
	move.w #-108,8(%a0)	|, pl_6(D)->dx
.L56:
	move.l (%sp)+,%d2	|,
	rts
.L59:
	btst #2,%d1	|, D.3382
	jeq .L60	|
	add.w #-9,%d0	|, D.3383
	move.w %d0,8(%a0)	| D.3383, pl_6(D)->dx
	move.w #1,14(%a0)	|, pl_6(D)->direction
	and.b #12,%d1	|, D.3382
	tst.w %d0	| D.3385
	jgt .L82	|
.L62:
	tst.w %d0	| D.3385
	jeq .L65	|
	tst.b %d1	| D.3382
	jne .L63	|
	add.w #9,%d0	|, D.3383
	tst.w %d0	| D.3383
	jle .L79	|
.L67:
	clr.w 8(%a0)	| pl_6(D)->dx
	move.l (%sp)+,%d2	|,
	rts
.L65:
	tst.b %d1	| D.3382
	jeq .L67	|
	move.w 8(%a0),%d0	| pl_6(D)->dx, D.3385
	cmp.w #108,%d0	|, D.3385
	jle .L68	|
.L81:
	move.w #108,8(%a0)	|, pl_6(D)->dx
	move.l (%sp)+,%d2	|,
	rts
	.size	player_accel, .-player_accel
	.align	2
	.globl	player_eval_grounded
	.type	player_eval_grounded, @function
player_eval_grounded:
	move.l 4(%sp),%a0	| pl, pl
	tst.w 10(%a0)	| pl_2(D)->dy
	jlt .L89	|
	move.l %a0,4(%sp)	| pl,
	jra (player_eval_grounded.part.0)	|
.L89:
	clr.w 12(%a0)	| pl_2(D)->grounded
	rts
	.size	player_eval_grounded, .-player_eval_grounded
	.align	2
	.globl	player_jump
	.type	player_jump, @function
player_jump:
	movem.l #14368,-(%sp)	|,
	move.l 20(%sp),%a2	| pl, pl
	move.w 12(%a2),%d0	| pl_7(D)->grounded, D.3391
	jeq .L92	|
	move.w #2,34(%a2)	|, pl_7(D)->cubejump_disable
.L92:
	btst #6,46(%a2)	|, pl_7(D)->input
	jeq .L91	|
	btst #6,47(%a2)	|, pl_7(D)->input_prev
	jne .L91	|
	tst.w %d0	| D.3391
	jne .L94	|
	tst.w 36(%a2)	| pl_7(D)->holding_cube
	jeq .L91	|
	tst.w sram+52	| sram.have_jump
	jeq .L91	|
	tst.w 34(%a2)	| pl_7(D)->cubejump_disable
	jne .L91	|
	move.w #20,22(%a2)	|, pl_7(D)->throwdown_cnt
	move.l (%a2),%d2	| pl_7(D)->x, D.3393
	moveq #10,%d0	|,
	asr.l %d0,%d2	|, D.3393
	move.w %d2,%d3	| D.3393, cx
	move.w %d2,%d1	|, back_x
	cmp.w #1,14(%a2)	|, pl_7(D)->direction
	jeq .L108	|
	add.w #-9,%d1	|, back_x
.L96:
	move.l 4(%a2),%d0	| pl_7(D)->y, D.3393
	moveq #10,%d4	|,
	asr.l %d4,%d0	|, D.3393
	move.w %d0,-(%sp)	| D.3393,
	clr.w -(%sp)	|
	move.w %d1,-(%sp)	| back_x,
	clr.w -(%sp)	|
	jsr map_collision	|
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L97	|
	and.l #65535,%d2	|, D.3394
	move.l %d2,%d3	| D.3394, D.3394
	addq.l #4,%d3	|, D.3394
	asr.l #3,%d3	|, D.3394
	lsl.w #3,%d3	|, cx
.L97:
	pea 256.w	|
	clr.l -(%sp)	|
	pea 2.w	|
	moveq #0,%d0	| D.3394
	move.w 36(%a2),%d0	| pl_7(D)->holding_cube, D.3394
	move.l %d0,-(%sp)	| D.3394,
	move.l 4(%a2),%d0	| pl_7(D)->y, D.3393
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3393
	add.w #-12,%d0	|, D.3391
	move.w %d0,-(%sp)	| D.3391,
	clr.w -(%sp)	|
	move.w %d3,-(%sp)	| cx,
	clr.w -(%sp)	|
	jsr cube_spawn	|
	clr.w 36(%a2)	| pl_7(D)->holding_cube
	lea (24,%sp),%sp	|,
.L94:
	move.w #-229,10(%a2)	|, pl_7(D)->dy
.L91:
	movem.l (%sp)+,#1052	|,
	rts
.L108:
	addq.w #8,%d1	|, back_x
	jra .L96	|
	.size	player_jump, .-player_jump
	.align	2
	.globl	player_toss_cubes
	.type	player_toss_cubes, @function
player_toss_cubes:
	move.l %a2,-(%sp)	|,
	move.l %d2,-(%sp)	|,
	move.l 12(%sp),%a2	| pl, pl
	move.w 36(%a2),%d0	| pl_7(D)->holding_cube, D.3397
	jeq .L110	|
	move.b 46(%a2),%d1	| pl_7(D)->input, D.3398
	btst #5,%d1	|, D.3398
	jeq .L110	|
	btst #5,47(%a2)	|, pl_7(D)->input_prev
	jne .L110	|
	btst #1,%d1	|, D.3398
	jeq .L112	|
	tst.w 14(%a2)	| pl_7(D)->direction
	jne .L117	|
	moveq #1,%d1	|, D.3399
	move.w #-128,%a0	|, D.3399
	move.l %a0,-(%sp)	| D.3399,
	move.l %d1,-(%sp)	| D.3399,
	pea 2.w	|
	move.w %d0,-(%sp)	| D.3397,
	clr.w -(%sp)	|
	move.l 4(%a2),%d0	| pl_7(D)->y, D.3400
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3400
	add.w #-23,%d0	|, D.3397
	move.w %d0,-(%sp)	| D.3397,
	clr.w -(%sp)	|
	move.l (%a2),%d0	| pl_7(D)->x, D.3400
	asr.l %d1,%d0	|, D.3400
	move.w %d0,-(%sp)	| D.3400,
	clr.w -(%sp)	|
	jsr cube_spawn	|
	clr.w 36(%a2)	| pl_7(D)->holding_cube
	move.w #2,32(%a2)	|, pl_7(D)->action_cnt
	move.w #8,24(%a2)	|, pl_7(D)->throw_cnt
	lea (24,%sp),%sp	|,
.L110:
	move.l (%sp)+,%d2	|,
	move.l (%sp)+,%a2	|,
	rts
.L112:
	btst #0,%d1	|, D.3398
	jne .L118	|
	move.w 14(%a2),%d2	| pl_7(D)->direction, D.3397
	btst #3,%d1	|, D.3398
	jeq .L115	|
	tst.w %d2	| D.3397
	jne .L115	|
	moveq #4,%d1	|, D.3399
	move.w #-64,%a0	|, D.3399
	move.l %a0,-(%sp)	| D.3399,
	move.l %d1,-(%sp)	| D.3399,
	pea 2.w	|
	move.w %d0,-(%sp)	| D.3397,
	clr.w -(%sp)	|
	move.l 4(%a2),%d0	| pl_7(D)->y, D.3400
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3400
	add.w #-23,%d0	|, D.3397
	move.w %d0,-(%sp)	| D.3397,
	clr.w -(%sp)	|
	move.l (%a2),%d0	| pl_7(D)->x, D.3400
	asr.l %d1,%d0	|, D.3400
	move.w %d0,-(%sp)	| D.3400,
	clr.w -(%sp)	|
	jsr cube_spawn	|
	clr.w 36(%a2)	| pl_7(D)->holding_cube
	move.w #2,32(%a2)	|, pl_7(D)->action_cnt
	move.w #8,24(%a2)	|, pl_7(D)->throw_cnt
	lea (24,%sp),%sp	|,
	jra .L110	|
.L117:
	moveq #-1,%d1	|, D.3399
	move.w #-128,%a0	|, D.3399
	move.l %a0,-(%sp)	| D.3399,
	move.l %d1,-(%sp)	| D.3399,
	pea 2.w	|
	move.w %d0,-(%sp)	| D.3397,
	clr.w -(%sp)	|
	move.l 4(%a2),%d0	| pl_7(D)->y, D.3400
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3400
	add.w #-23,%d0	|, D.3397
	move.w %d0,-(%sp)	| D.3397,
	clr.w -(%sp)	|
	move.l (%a2),%d0	| pl_7(D)->x, D.3400
	asr.l %d1,%d0	|, D.3400
	move.w %d0,-(%sp)	| D.3400,
	clr.w -(%sp)	|
	jsr cube_spawn	|
	clr.w 36(%a2)	| pl_7(D)->holding_cube
	move.w #2,32(%a2)	|, pl_7(D)->action_cnt
	move.w #8,24(%a2)	|, pl_7(D)->throw_cnt
	lea (24,%sp),%sp	|,
	jra .L110	|
.L118:
	moveq #0,%d1	| D.3399
	move.w #-320,%a0	|, D.3399
	move.l %a0,-(%sp)	| D.3399,
	move.l %d1,-(%sp)	| D.3399,
	pea 2.w	|
	move.w %d0,-(%sp)	| D.3397,
	clr.w -(%sp)	|
	move.l 4(%a2),%d0	| pl_7(D)->y, D.3400
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3400
	add.w #-23,%d0	|, D.3397
	move.w %d0,-(%sp)	| D.3397,
	clr.w -(%sp)	|
	move.l (%a2),%d0	| pl_7(D)->x, D.3400
	asr.l %d1,%d0	|, D.3400
	move.w %d0,-(%sp)	| D.3400,
	clr.w -(%sp)	|
	jsr cube_spawn	|
	clr.w 36(%a2)	| pl_7(D)->holding_cube
	move.w #2,32(%a2)	|, pl_7(D)->action_cnt
	move.w #8,24(%a2)	|, pl_7(D)->throw_cnt
	lea (24,%sp),%sp	|,
	jra .L110	|
.L115:
	btst #2,%d1	|, D.3398
	jeq .L116	|
	cmp.w #1,%d2	|, D.3397
	jeq .L120	|
.L116:
	tst.w %d2	| D.3397
	jne .L121	|
	moveq #2,%d1	|, D.3399
	move.w #-128,%a0	|, D.3399
	move.l %a0,-(%sp)	| D.3399,
	move.l %d1,-(%sp)	| D.3399,
	pea 2.w	|
	move.w %d0,-(%sp)	| D.3397,
	clr.w -(%sp)	|
	move.l 4(%a2),%d0	| pl_7(D)->y, D.3400
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3400
	add.w #-23,%d0	|, D.3397
	move.w %d0,-(%sp)	| D.3397,
	clr.w -(%sp)	|
	move.l (%a2),%d0	| pl_7(D)->x, D.3400
	asr.l %d1,%d0	|, D.3400
	move.w %d0,-(%sp)	| D.3400,
	clr.w -(%sp)	|
	jsr cube_spawn	|
	clr.w 36(%a2)	| pl_7(D)->holding_cube
	move.w #2,32(%a2)	|, pl_7(D)->action_cnt
	move.w #8,24(%a2)	|, pl_7(D)->throw_cnt
	lea (24,%sp),%sp	|,
	jra .L110	|
.L120:
	moveq #-4,%d1	|, D.3399
	move.w #-64,%a0	|, D.3399
	move.l %a0,-(%sp)	| D.3399,
	move.l %d1,-(%sp)	| D.3399,
	pea 2.w	|
	move.w %d0,-(%sp)	| D.3397,
	clr.w -(%sp)	|
	move.l 4(%a2),%d0	| pl_7(D)->y, D.3400
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3400
	add.w #-23,%d0	|, D.3397
	move.w %d0,-(%sp)	| D.3397,
	clr.w -(%sp)	|
	move.l (%a2),%d0	| pl_7(D)->x, D.3400
	asr.l %d1,%d0	|, D.3400
	move.w %d0,-(%sp)	| D.3400,
	clr.w -(%sp)	|
	jsr cube_spawn	|
	clr.w 36(%a2)	| pl_7(D)->holding_cube
	move.w #2,32(%a2)	|, pl_7(D)->action_cnt
	move.w #8,24(%a2)	|, pl_7(D)->throw_cnt
	lea (24,%sp),%sp	|,
	jra .L110	|
.L121:
	moveq #-2,%d1	|, D.3399
	move.w #-128,%a0	|, D.3399
	move.l %a0,-(%sp)	| D.3399,
	move.l %d1,-(%sp)	| D.3399,
	pea 2.w	|
	move.w %d0,-(%sp)	| D.3397,
	clr.w -(%sp)	|
	move.l 4(%a2),%d0	| pl_7(D)->y, D.3400
	moveq #10,%d1	|,
	asr.l %d1,%d0	|, D.3400
	add.w #-23,%d0	|, D.3397
	move.w %d0,-(%sp)	| D.3397,
	clr.w -(%sp)	|
	move.l (%a2),%d0	| pl_7(D)->x, D.3400
	asr.l %d1,%d0	|, D.3400
	move.w %d0,-(%sp)	| D.3400,
	clr.w -(%sp)	|
	jsr cube_spawn	|
	clr.w 36(%a2)	| pl_7(D)->holding_cube
	move.w #2,32(%a2)	|, pl_7(D)->action_cnt
	move.w #8,24(%a2)	|, pl_7(D)->throw_cnt
	lea (24,%sp),%sp	|,
	jra .L110	|
	.size	player_toss_cubes, .-player_toss_cubes
	.align	2
	.globl	player_lift_cubes
	.type	player_lift_cubes, @function
player_lift_cubes:
	rts
	.size	player_lift_cubes, .-player_lift_cubes
	.align	2
	.globl	player_special_counters
	.type	player_special_counters, @function
player_special_counters:
	move.l 4(%sp),%a0	| pl, pl
	move.w 22(%a0),%d0	| pl_9(D)->throwdown_cnt, D.3405
	jeq .L135	|
	subq.w #1,%d0	|, D.3405
	move.w %d0,22(%a0)	| D.3405, pl_9(D)->throwdown_cnt
.L135:
	move.w 24(%a0),%d0	| pl_9(D)->throw_cnt, D.3405
	jeq .L136	|
	subq.w #1,%d0	|, D.3405
	move.w %d0,24(%a0)	| D.3405, pl_9(D)->throw_cnt
.L136:
	move.w 28(%a0),%d0	| pl_9(D)->lift_cnt, D.3405
	jeq .L137	|
	subq.w #1,%d0	|, D.3405
	move.w %d0,28(%a0)	| D.3405, pl_9(D)->lift_cnt
.L137:
	move.w 30(%a0),%d0	| pl_9(D)->hurt_cnt, D.3405
	jeq .L138	|
	subq.w #1,%d0	|, D.3405
	move.w %d0,30(%a0)	| D.3405, pl_9(D)->hurt_cnt
.L138:
	move.w 38(%a0),%d0	| pl_9(D)->invuln_cnt, D.3405
	jeq .L139	|
	subq.w #1,%d0	|, D.3405
	move.w %d0,38(%a0)	| D.3405, pl_9(D)->invuln_cnt
.L139:
	move.w 32(%a0),%d0	| pl_9(D)->action_cnt, D.3405
	jeq .L140	|
	subq.w #1,%d0	|, D.3405
	move.w %d0,32(%a0)	| D.3405, pl_9(D)->action_cnt
.L140:
	move.w 34(%a0),%d0	| pl_9(D)->cubejump_disable, D.3405
	jeq .L134	|
	subq.w #1,%d0	|, D.3405
	move.w %d0,34(%a0)	| D.3405, pl_9(D)->cubejump_disable
.L134:
	rts
	.size	player_special_counters, .-player_special_counters
	.align	2
	.globl	player_move
	.type	player_move, @function
player_move:
	movem.l #15416,-(%sp)	|,
	move.l 32(%sp),%a2	| pl, pl
	move.w 8(%a2),%d2	| pl_4(D)->dx, D.3422
	move.w %d2,%d0	| D.3422,
	ext.l %d0	| D.3423
	lsl.l #4,%d0	|, D.3423
	add.l (%a2),%d0	| pl_4(D)->x, D.3421
	move.l %d0,(%a2)	| D.3421, pl_4(D)->x
	move.w 10(%a2),%d1	| pl_4(D)->dy, D.3422
	move.w %d1,%d3	| D.3422,
	ext.l %d3	| D.3423
	lsl.l #4,%d3	|, D.3423
	add.l 4(%a2),%d3	| pl_4(D)->y, D.3421
	move.l %d3,4(%a2)	| D.3421, pl_4(D)->y
	moveq #10,%d4	|,
	asr.l %d4,%d3	|, D.3421
	asr.l %d4,%d0	|, D.3421
	asr.w #6,%d2	|, D.3422
	move.w %d0,%d5	|, px
	sub.w %d2,%d5	| D.3422, px
	tst.w %d1	| D.3422
	jlt .L166	|
	move.l %d3,%d2	| D.3421, D.3423
	and.l #65535,%d2	|, D.3423
	move.w %d5,%d4	| px, D.3424
	addq.w #3,%d4	|, D.3424
	and.l #65535,%d4	|, D.3423
	move.l %d2,-(%sp)	| D.3423,
	move.l %d4,-(%sp)	| D.3423,
	lea map_collision,%a3	|, tmp212
	jsr (%a3)	| tmp212
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L167	|
	and.w #-8,%d3	|, py
	subq.w #1,%d3	|, py
	and.l #65535,%d3	|, D.3423
	move.l %d3,%d0	| D.3423, D.3423
	moveq #10,%d1	|,
	lsl.l %d1,%d0	|, D.3423
	move.l %d0,4(%a2)	| D.3423, pl_4(D)->y
	clr.w 10(%a2)	| pl_4(D)->dy
	moveq #8,%d2	|, D.3427
	lea map_collision,%a4	|, tmp214
	subq.w #4,%d5	|, D.3424
	and.l #65535,%d5	|, D.3423
.L168:
	move.l %d3,-(%sp)	| D.3423,
	move.l %d4,-(%sp)	| D.3423,
	jsr (%a3)	| tmp212
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L171	|
	add.l #-1024,4(%a2)	|, pl_4(D)->y
	subq.l #1,%d2	|, D.3427
	jne .L168	|
.L173:
	tst.w 10(%a2)	| pl_4(D)->dy
	jlt .L212	|
	move.l %a2,-(%sp)	| pl,
	lea (player_eval_grounded.part.0),%a4	|, tmp165
	jsr (%a4)	| tmp165
	addq.l #4,%sp	|,
	tst.w 12(%a2)	| pl_4(D)->grounded
	jeq .L178	|
.L205:
	move.l 4(%a2),%d3	| pl_4(D)->y, D.3429
.L170:
	moveq #10,%d0	|,
	asr.l %d0,%d3	|, D.3421
	move.l (%a2),%d2	| pl_4(D)->x, D.3421
	asr.l %d0,%d2	|, D.3421
	move.w 8(%a2),%d0	| pl_4(D)->dx, D.3422
	jle .L183	|
	move.w %d2,%d4	|, D.3424
	addq.w #4,%d4	|, D.3424
	and.l #65535,%d4	|, D.3423
	move.w %d3,%d0	|, D.3424
	add.w #-18,%d0	|, D.3424
	move.w %d0,-(%sp)	| D.3424,
	clr.w -(%sp)	|
	move.l %d4,-(%sp)	| D.3423,
	lea map_collision,%a4	|, tmp186
	jsr (%a3)	| tmp212
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L184	|
.L186:
	and.w #-8,%d2	|, px
	addq.w #3,%d2	|, px
	moveq #10,%d1	|,
	lsl.l %d1,%d2	|, D.3423
	and.l #67107840,%d2	|, D.3423
	move.l %d2,(%a2)	| D.3423, pl_4(D)->x
	clr.w 8(%a2)	| pl_4(D)->dx
.L185:
	move.w 10(%a2),%d0	| pl_4(D)->dy, D.3422
	jlt .L213	|
	move.l %a2,-(%sp)	| pl,
	jsr (player_eval_grounded.part.0)	|
	addq.l #4,%sp	|,
	tst.w 12(%a2)	| pl_4(D)->grounded
	jeq .L214	|
.L165:
	movem.l (%sp)+,#7228	|,
	rts
.L171:
	move.l %d3,-(%sp)	| D.3423,
	move.l %d5,-(%sp)	| D.3423,
	jsr (%a4)	| tmp214
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L173	|
	add.l #-1024,4(%a2)	|, pl_4(D)->y
	subq.l #1,%d2	|, D.3427
	jne .L168	|
	jra .L173	|
.L184:
	move.w %d3,%d0	|, D.3424
	subq.w #3,%d0	|, D.3424
	move.w %d0,-(%sp)	| D.3424,
	clr.w -(%sp)	|
	move.l %d4,-(%sp)	| D.3423,
	jsr (%a4)	| tmp186
	addq.l #8,%sp	|,
	tst.w %d0	|
	jne .L186	|
	add.w #-11,%d3	|, D.3424
	move.w %d3,-(%sp)	| D.3424,
	clr.w -(%sp)	|
	move.l %d4,-(%sp)	| D.3423,
	jsr (%a4)	| tmp186
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L185	|
	and.w #-8,%d2	|, px
	addq.w #3,%d2	|, px
	moveq #10,%d1	|,
	lsl.l %d1,%d2	|, D.3423
	and.l #67107840,%d2	|, D.3423
	move.l %d2,(%a2)	| D.3423, pl_4(D)->x
	clr.w 8(%a2)	| pl_4(D)->dx
	jra .L185	|
.L213:
	clr.w 12(%a2)	| pl_4(D)->grounded
	btst #6,46(%a2)	|, pl_4(D)->input
	jne .L191	|
.L192:
	add.w #17,%d0	|, D.3422
	move.w %d0,10(%a2)	| D.3422, pl_4(D)->dy
	cmp.w #512,%d0	|, D.3422
	jle .L165	|
	move.w #512,10(%a2)	|, pl_4(D)->dy
	movem.l (%sp)+,#7228	|,
	rts
.L183:
	tst.w %d0	| D.3422
	jeq .L185	|
	move.w %d2,%d4	|, D.3424
	subq.w #5,%d4	|, D.3424
	and.l #65535,%d4	|, D.3423
	move.w %d3,%d0	|, D.3424
	add.w #-18,%d0	|, D.3424
	move.w %d0,-(%sp)	| D.3424,
	clr.w -(%sp)	|
	move.l %d4,-(%sp)	| D.3423,
	lea map_collision,%a4	|, tmp199
	jsr (%a3)	| tmp212
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L188	|
.L189:
	and.w #-8,%d2	|, px
	addq.w #5,%d2	|, px
	moveq #10,%d4	|,
	lsl.l %d4,%d2	|, D.3423
	and.l #67107840,%d2	|, D.3423
	move.l %d2,(%a2)	| D.3423, pl_4(D)->x
	clr.w 8(%a2)	| pl_4(D)->dx
	jra .L185	|
.L212:
	move.l 4(%a2),%d3	| pl_4(D)->y, D.3421
	add.l #8192,%d3	|, D.3421
	move.l %d3,4(%a2)	| D.3421, pl_4(D)->y
.L177:
	clr.w 12(%a2)	| pl_4(D)->grounded
	jra .L170	|
.L214:
	move.w 10(%a2),%d0	| pl_4(D)->dy, D.3428
	btst #6,46(%a2)	|, pl_4(D)->input
	jeq .L192	|
	tst.w %d0	| D.3428
	jge .L192	|
.L191:
	add.w #9,%d0	|, D.3428
	move.w %d0,10(%a2)	| D.3428, pl_4(D)->dy
	movem.l (%sp)+,#7228	|,
	rts
.L167:
	move.l %d2,-(%sp)	| D.3423,
	move.w %d5,%d0	| px, D.3424
	subq.w #4,%d0	|, D.3424
	move.w %d0,-(%sp)	| D.3424,
	clr.w -(%sp)	|
	jsr (%a3)	| tmp212
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L205	|
	and.w #-8,%d3	|, py
	subq.w #1,%d3	|, py
	and.l #65535,%d3	|, D.3423
	move.l %d3,%d0	| D.3423, D.3423
	moveq #10,%d1	|,
	lsl.l %d1,%d0	|, D.3423
	move.l %d0,4(%a2)	| D.3423, pl_4(D)->y
	clr.w 10(%a2)	| pl_4(D)->dy
	moveq #8,%d2	|, D.3427
	lea map_collision,%a4	|, tmp214
	subq.w #4,%d5	|, D.3424
	and.l #65535,%d5	|, D.3423
	jra .L168	|
.L178:
	move.l 4(%a2),%d3	| pl_4(D)->y, D.3421
	add.l #8192,%d3	|, D.3421
	move.l %d3,4(%a2)	| D.3421, pl_4(D)->y
	tst.w 10(%a2)	| pl_4(D)->dy
	jlt .L177	|
	move.l %a2,-(%sp)	| pl,
	jsr (%a4)	| tmp165
	move.l 4(%a2),%d3	| pl_4(D)->y, D.3429
	addq.l #4,%sp	|,
	jra .L170	|
.L166:
	move.w %d3,%d2	|, D.3424
	add.w #-19,%d2	|, D.3424
	and.l #65535,%d2	|, D.3423
	move.l %d2,-(%sp)	| D.3423,
	move.w %d5,%d0	| px, D.3424
	addq.w #3,%d0	|, D.3424
	move.w %d0,-(%sp)	| D.3424,
	clr.w -(%sp)	|
	lea map_collision,%a3	|, tmp212
	jsr (%a3)	| tmp212
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L180	|
	and.l #65535,%d3	|, D.3423
	addq.l #4,%d3	|, D.3423
	asr.l #3,%d3	|, D.3423
	lsl.w #3,%d3	|, py
	addq.w #3,%d3	|, py
	and.l #65535,%d3	|, D.3423
	moveq #10,%d4	|,
	lsl.l %d4,%d3	|, D.3423
	move.l %d3,4(%a2)	| D.3423, pl_4(D)->y
	cmp.w #-32,10(%a2)	|, pl_4(D)->dy
	jge .L170	|
.L181:
	move.w #-32,10(%a2)	|, pl_4(D)->dy
	jra .L170	|
.L180:
	move.l %d2,-(%sp)	| D.3423,
	subq.w #4,%d5	|, D.3424
	move.w %d5,-(%sp)	| D.3424,
	clr.w -(%sp)	|
	jsr (%a3)	| tmp212
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L205	|
	and.l #65535,%d3	|, D.3423
	addq.l #4,%d3	|, D.3423
	asr.l #3,%d3	|, D.3423
	lsl.w #3,%d3	|, py
	addq.w #3,%d3	|, py
	and.l #65535,%d3	|, D.3423
	moveq #10,%d4	|,
	lsl.l %d4,%d3	|, D.3423
	move.l %d3,4(%a2)	| D.3423, pl_4(D)->y
	cmp.w #-32,10(%a2)	|, pl_4(D)->dy
	jlt .L181	|
	jra .L170	|
.L188:
	move.w %d3,%d0	|, D.3424
	subq.w #3,%d0	|, D.3424
	move.w %d0,-(%sp)	| D.3424,
	clr.w -(%sp)	|
	move.l %d4,-(%sp)	| D.3423,
	jsr (%a4)	| tmp199
	addq.l #8,%sp	|,
	tst.w %d0	|
	jne .L189	|
	add.w #-11,%d3	|, D.3424
	move.w %d3,-(%sp)	| D.3424,
	clr.w -(%sp)	|
	move.l %d4,-(%sp)	| D.3423,
	jsr (%a4)	| tmp199
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L185	|
	and.w #-8,%d2	|, px
	addq.w #5,%d2	|, px
	moveq #10,%d4	|,
	lsl.l %d4,%d2	|, D.3423
	and.l #67107840,%d2	|, D.3423
	move.l %d2,(%a2)	| D.3423, pl_4(D)->x
	clr.w 8(%a2)	| pl_4(D)->dx
	jra .L185	|
	.size	player_move, .-player_move
	.align	2
	.globl	player_calc_anim
	.type	player_calc_anim, @function
player_calc_anim:
	move.l 4(%sp),%a0	| pl, pl
	tst.w 38(%a0)	| pl_5(D)->invuln_cnt
	jeq .L217	|
	move.w system_osc,%d0	| system_osc, D.3432
	and.w #7,%d0	|, D.3432
	cmp.w #3,%d0	|, D.3432
	jhi .L216	|
.L217:
	move.w 12(%a0),%d0	| pl_5(D)->grounded, D.3432
	jeq .L219	|
	move.w 18(%a0),%d1	| pl_5(D)->anim_cnt, D.3432
	addq.w #1,%d1	|, D.3432
	cmp.w #24,%d1	|, D.3432
	jeq .L219	|
	move.w %d1,18(%a0)	| D.3432, pl_5(D)->anim_cnt
	tst.w 24(%a0)	| pl_5(D)->throw_cnt
	jne .L236	|
.L222:
	tst.w 22(%a0)	| pl_5(D)->throwdown_cnt
	jne .L237	|
	tst.w 26(%a0)	| pl_5(D)->kick_cnt
	jne .L238	|
	tst.w 28(%a0)	| pl_5(D)->lift_cnt
	jne .L239	|
	tst.w 30(%a0)	| pl_5(D)->hurt_cnt
	jne .L240	|
	tst.w %d0	| D.3432
	jeq .L227	|
	move.b 46(%a0),%d0	| pl_5(D)->input, D.3433
	and.b #12,%d0	|, D.3433
	jne .L228	|
	clr.w 20(%a0)	| pl_5(D)->anim_frame
.L229:
	cmp.w #5,40(%a0)	|, pl_5(D)->cp_cnt
	jhi .L233	|
	tst.w 36(%a0)	| pl_5(D)->holding_cube
	jeq .L216	|
.L233:
	addq.w #8,20(%a0)	|, pl_5(D)->anim_frame
.L216:
	rts
.L219:
	clr.w 18(%a0)	| pl_5(D)->anim_cnt
	tst.w 24(%a0)	| pl_5(D)->throw_cnt
	jeq .L222	|
.L236:
	move.w #22,20(%a0)	|, pl_5(D)->anim_frame
	rts
.L238:
	move.w #23,20(%a0)	|, pl_5(D)->anim_frame
	rts
.L237:
	move.w #7,20(%a0)	|, pl_5(D)->anim_frame
	rts
.L239:
	move.w #5,20(%a0)	|, pl_5(D)->anim_frame
	rts
.L227:
	move.w #4,20(%a0)	|, pl_5(D)->anim_frame
	jra .L229	|
.L240:
	move.w #6,20(%a0)	|, pl_5(D)->anim_frame
	rts
.L228:
	move.w 18(%a0),%d0	| pl_5(D)->anim_cnt, D.3432
	cmp.w #5,%d0	|, D.3432
	jls .L232	|
	cmp.w #11,%d0	|, D.3432
	jhi .L231	|
	move.w #3,20(%a0)	|, pl_5(D)->anim_frame
	jra .L229	|
.L232:
	move.w #2,20(%a0)	|, pl_5(D)->anim_frame
	jra .L229	|
.L231:
	cmp.w #17,%d0	|, D.3432
	jls .L232	|
	move.w #1,20(%a0)	|, pl_5(D)->anim_frame
	jra .L229	|
	.size	player_calc_anim, .-player_calc_anim
	.align	2
	.globl	player_draw
	.type	player_draw, @function
player_draw:
	move.l %a2,-(%sp)	|,
	move.l %d2,-(%sp)	|,
	move.l 12(%sp),%a2	| pl, pl
	tst.w 38(%a2)	| pl_7(D)->invuln_cnt
	jeq .L243	|
	move.w system_osc,%d0	| system_osc, D.3436
	and.w #7,%d0	|, D.3436
	cmp.w #3,%d0	|, D.3436
	jhi .L242	|
.L243:
	move.w 20(%a2),%d0	| pl_7(D)->anim_frame, D.3436
	cmp.w #15,%d0	|, D.3436
	jls .L254	|
	cmp.w #19,%d0	|, D.3436
	jhi .L247	|
	move.w 14(%a2),%d0	| pl_7(D)->direction, D.3436
	move.w #-4,%a0	|, D.3439
	moveq #-15,%d1	|, D.3439
	move.w #9,%a1	|, D.3437
.L246:
	moveq #11,%d2	|,
	lsl.w %d2,%d0	|, D.3436
	add.w #-7424,%d0	|, D.3436
	move.l %d0,%d2	|,
	and.l #65280,%d2	|,
	move.l %d2,-(%sp)	|,
	move.l %a1,-(%sp)	| D.3437,
	move.l 4(%a2),%d0	| pl_7(D)->y, D.3438
	moveq #10,%d2	|,
	asr.l %d2,%d0	|, D.3438
	sub.w state+10,%d0	| state.cam_y, D.3439
	add.w %d1,%d0	| D.3439, D.3439
	move.w %d0,%a1	| D.3439,
	move.l %a1,-(%sp)	|,
	moveq #-8,%d0	|, D.3439
	sub.w state+8,%d0	| state.cam_x, D.3439
	move.l (%a2),%d1	| pl_7(D)->x, D.3438
	asr.l %d2,%d1	|, D.3438
	add.w %d1,%d0	| D.3438, D.3439
	add.w %a0,%d0	| D.3439, D.3439
	move.w %d0,%a0	| D.3439,
	move.l %a0,-(%sp)	|,
	jsr sprite_put	|
	move.w 36(%a2),%d0	| pl_7(D)->holding_cube, D.3436
	lea (16,%sp),%sp	|,
	jeq .L242	|
	move.w %d0,-(%sp)	| D.3436,
	clr.w -(%sp)	|
	move.l 4(%a2),%d0	| pl_7(D)->y, D.3438
	asr.l %d2,%d0	|, D.3438
	add.w #-38,%d0	|, D.3436
	move.w %d0,-(%sp)	| D.3436,
	clr.w -(%sp)	|
	move.l (%a2),%d0	| pl_7(D)->x, D.3438
	asr.l %d2,%d0	|, D.3438
	subq.w #8,%d0	|, D.3436
	move.w %d0,-(%sp)	| D.3436,
	clr.w -(%sp)	|
	jsr cube_draw_single	|
	lea (12,%sp),%sp	|,
.L242:
	move.l (%sp)+,%d2	|,
	move.l (%sp)+,%a2	|,
	rts
.L254:
	move.w 14(%a2),%d0	| pl_7(D)->direction, D.3436
	sub.l %a0,%a0	| D.3439
	moveq #-23,%d1	|, D.3439
	move.w #6,%a1	|, D.3437
	jra .L246	|
.L247:
	move.w 14(%a2),%d0	| pl_7(D)->direction, D.3436
	jeq .L255	|
	move.w #-8,%a0	|, D.3439
	moveq #-23,%d1	|, D.3439
	move.w #10,%a1	|, D.3437
	jra .L246	|
.L255:
	sub.l %a0,%a0	| D.3439
	moveq #-23,%d1	|, D.3439
	move.w #10,%a1	|, D.3437
	jra .L246	|
	.size	player_draw, .-player_draw
	.align	2
	.globl	player_run
	.type	player_run, @function
player_run:
	move.l %a2,-(%sp)	|,
	move.l 8(%sp),%a2	| pl, pl
	move.l %a2,-(%sp)	| pl,
	jsr player_input	|
	move.l %a2,-(%sp)	| pl,
	jsr player_accel	|
	move.l %a2,-(%sp)	| pl,
	jsr player_jump	|
	move.l %a2,-(%sp)	| pl,
	jsr player_move	|
	move.l %a2,-(%sp)	| pl,
	jsr player_toss_cubes	|
	move.l %a2,-(%sp)	| pl,
	jsr player_cp	|
	lea (24,%sp),%sp	|,
	tst.w 10(%a2)	| pl_2(D)->dy
	jlt .L261	|
	move.l %a2,-(%sp)	| pl,
	jsr (player_eval_grounded.part.0)	|
	addq.l #4,%sp	|,
	move.l %a2,-(%sp)	| pl,
	jsr player_calc_anim	|
	move.l %a2,-(%sp)	| pl,
	jsr player_dma_setup	|
	addq.l #8,%sp	|,
	move.l %a2,8(%sp)	| pl,
	move.l (%sp)+,%a2	|,
	jra player_special_counters	|
.L261:
	clr.w 12(%a2)	| pl_2(D)->grounded
	move.l %a2,-(%sp)	| pl,
	jsr player_calc_anim	|
	move.l %a2,-(%sp)	| pl,
	jsr player_dma_setup	|
	addq.l #8,%sp	|,
	move.l %a2,8(%sp)	| pl,
	move.l (%sp)+,%a2	|,
	jra player_special_counters	|
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
