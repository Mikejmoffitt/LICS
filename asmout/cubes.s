	.file	"cubes.c"
| GNU C (GCC) version 4.8.2 (m68k-elf)
|	compiled by GNU C version 4.9.3, GMP version 5.0.5, MPFR version 2.4.2, MPC version 0.8.2
| GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
| options passed:  -I /opt/toolchains/gen/m68k-elf/include
| -I /opt/toolchains/gen/m68k-elf/m68k-elf/include -I src -I res -I inc
| src/cubes.c -mcpu=68000 -O2 -std=c99 -fomit-frame-pointer -fno-builtin
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
	.globl	cube_dma_tiles
	.type	cube_dma_tiles, @function
cube_dma_tiles:
	pea 1024.w	|
	pea 25888.w	|
	pea gfx_cubes	|
	clr.l -(%sp)	|
	jsr VDP_doDMA	|
	lea (16,%sp),%sp	|,
	rts
	.size	cube_dma_tiles, .-cube_dma_tiles
	.align	2
	.globl	cubes_init
	.type	cubes_init, @function
cubes_init:
	lea cubes,%a0	|, ivtmp.19
.L4:
	clr.w (%a0)	| MEM[base: _12, offset: 0B]
	lea (12,%a0),%a0	|, ivtmp.19
	cmp.l #cubes+384,%a0	|, ivtmp.19
	jne .L4	|
	rts
	.size	cubes_init, .-cubes_init
	.align	2
	.globl	cubes_run
	.type	cubes_run, @function
cubes_run:
	movem.l #8254,-(%sp)	|,
	lea cubes,%a3	|, ivtmp.30
	lea cubes+8,%a4	|, ivtmp.33
	lea cubes+2,%a5	|, ivtmp.34
	lea cubes+4,%a2	|, ivtmp.35
	lea cubes+6,%a6	|, ivtmp.36
	move.l #map_collision,%d2	|, tmp142
.L22:
	move.w (%a3),%d0	| MEM[base: _64, offset: 0B], D.2871
	jeq .L9	|
	cmp.w #3,%d0	|, D.2871
	jls .L10	|
	move.w (%a4),%d0	| MEM[base: _137, offset: 0B], D.2872
	jle .L11	|
	subq.w #1,%d0	|, D.2872
	move.w %d0,(%a4)	| D.2872, MEM[base: _137, offset: 0B]
.L9:
	lea (12,%a3),%a3	|, ivtmp.30
	lea (12,%a4),%a4	|, ivtmp.33
	lea (12,%a5),%a5	|, ivtmp.34
	lea (12,%a2),%a2	|, ivtmp.35
	lea (12,%a6),%a6	|, ivtmp.36
	cmp.l #cubes+388,%a2	|, ivtmp.35
	jne .L22	|
.L32:
	movem.l (%sp)+,#31748	|,
	rts
.L10:
	cmp.w #1,%d0	|, D.2871
	jeq .L9	|
	moveq #0,%d0	| D.2873
	move.w (%a2),%d0	| MEM[base: _128, offset: 0B], D.2873
	move.l %d0,-(%sp)	| D.2873,
	move.w (%a5),%d0	| MEM[base: _129, offset: 0B], D.2871
	subq.w #8,%d0	|, D.2871
	move.w %d0,-(%sp)	| D.2871,
	clr.w -(%sp)	|
	move.l %d2,%a0	| tmp142,
	jsr (%a0)	|
	addq.l #8,%sp	|,
	tst.w %d0	|
	jeq .L13	|
.L16:
	move.w 6(%a2),%d0	| MEM[base: _128, offset: 6B], D.2871
	cmp.w #768,%d0	|, D.2871
	jeq .L15	|
	clr.w (%a6)	| MEM[base: _125, offset: 0B]
	move.w #7,(%a4)	|, MEM[base: _126, offset: 0B]
	cmp.w #1024,%d0	|, D.2871
	jeq .L30	|
	move.w #4,(%a3)	|, MEM[base: _64, offset: 0B]
.L18:
	clr.w %d0	| D.2871
.L20:
	add.w (%a5),%d0	| MEM[base: _129, offset: 0B], D.2871
	move.w %d0,(%a5)	| D.2871, MEM[base: _129, offset: 0B]
	move.l state,%a1	| state.current_room, D.2877
	and.l #65535,%d0	|, D.2873
	move.l %d0,%a0	| D.2873, D.2873
	subq.l #8,%a0	|, D.2873
	moveq #0,%d1	| D.2873
	move.b 35(%a1),%d1	| _92->w, D.2873
	move.l %d1,%d0	| D.2873, tmp137
	add.l %d1,%d0	| D.2873, tmp137
	add.l %d0,%d0	| tmp137, tmp138
	add.l %d1,%d0	| D.2873, tmp140
	lsl.l #6,%d0	|, tmp140
	cmp.l %a0,%d0	| D.2873, tmp140
	jge .L31	|
	clr.w (%a3)	| MEM[base: _64, offset: 0B]
	move.w #-32,(%a5)	|, MEM[base: _129, offset: 0B]
	move.w #-32,(%a2)	|, MEM[base: _128, offset: 0B]
.L34:
	lea (12,%a3),%a3	|, ivtmp.30
	lea (12,%a4),%a4	|, ivtmp.33
	lea (12,%a5),%a5	|, ivtmp.34
	lea (12,%a2),%a2	|, ivtmp.35
	lea (12,%a6),%a6	|, ivtmp.36
	cmp.l #cubes+388,%a2	|, ivtmp.35
	jne .L22	|
	jra .L32	|
.L13:
	moveq #0,%d0	| D.2873
	move.w (%a2),%d0	| MEM[base: _128, offset: 0B], D.2873
	move.l %d0,-(%sp)	| D.2873,
	move.w (%a5),%d0	| MEM[base: _129, offset: 0B], D.2871
	addq.w #7,%d0	|, D.2871
	move.w %d0,-(%sp)	| D.2871,
	clr.w -(%sp)	|
	move.l %d2,%a0	| tmp142,
	jsr (%a0)	|
	addq.l #8,%sp	|,
	tst.w %d0	|
	jne .L16	|
	move.w (%a2),%d0	| MEM[base: _128, offset: 0B], D.2871
	add.w #-15,%d0	|, D.2871
	move.w %d0,-(%sp)	| D.2871,
	clr.w -(%sp)	|
	move.w (%a5),%d0	| MEM[base: _129, offset: 0B], D.2871
	subq.w #8,%d0	|, D.2871
	move.w %d0,-(%sp)	| D.2871,
	clr.w -(%sp)	|
	move.l %d2,%a0	| tmp142,
	jsr (%a0)	|
	addq.l #8,%sp	|,
	tst.w %d0	|
	jne .L16	|
	move.w (%a2),%d0	| MEM[base: _128, offset: 0B], D.2871
	add.w #-15,%d0	|, D.2871
	move.w %d0,-(%sp)	| D.2871,
	clr.w -(%sp)	|
	move.w (%a5),%d0	| MEM[base: _129, offset: 0B], D.2871
	addq.w #7,%d0	|, D.2871
	move.w %d0,-(%sp)	| D.2871,
	clr.w -(%sp)	|
	move.l %d2,%a0	| tmp142,
	jsr (%a0)	|
	addq.l #8,%sp	|,
	tst.w %d0	|
	jne .L16	|
.L15:
	cmp.w #2,(%a3)	|, MEM[base: _64, offset: 0B]
	jeq .L33	|
	move.w (%a6),%d0	|* ivtmp.36, D.2871
	jra .L20	|
.L11:
	clr.w (%a3)	| MEM[base: _64, offset: 0B]
	lea (12,%a3),%a3	|, ivtmp.30
	lea (12,%a4),%a4	|, ivtmp.33
	lea (12,%a5),%a5	|, ivtmp.34
	lea (12,%a2),%a2	|, ivtmp.35
	lea (12,%a6),%a6	|, ivtmp.36
	cmp.l #cubes+388,%a2	|, ivtmp.35
	jne .L22	|
	jra .L32	|
.L31:
	moveq #0,%d0	|
	move.w (%a2),%d0	| MEM[base: _128, offset: 0B],
	move.l %d0,%a0	|, D.2873
	lea (-15,%a0),%a0	|, D.2873
	clr.w %d0	| D.2870
	move.b 37(%a1),%d0	| _92->h, D.2870
	mulu.w #240,%d0	|, D.2873
	cmp.l %a0,%d0	| D.2873, D.2873
	jge .L9	|
	clr.w (%a3)	| MEM[base: _64, offset: 0B]
	move.w #-32,(%a5)	|, MEM[base: _129, offset: 0B]
	move.w #-32,(%a2)	|, MEM[base: _128, offset: 0B]
	jra .L34	|
.L33:
	move.w (%a4),%d0	| MEM[base: _118, offset: 0B], D.2872
	move.w %d0,%d1	| D.2872, D.2872
	asr.w #6,%d1	|, D.2872
	add.w %d1,(%a2)	| D.2872, MEM[base: _128, offset: 0B]
	add.w #12,%d0	|, D.2872
	move.w %d0,(%a4)	| D.2872, MEM[base: _118, offset: 0B]
	move.w (%a6),%d0	|* ivtmp.36, D.2871
	jra .L20	|
.L30:
	move.w #6,(%a3)	|, MEM[base: _64, offset: 0B]
	jra .L18	|
	.size	cubes_run, .-cubes_run
	.align	2
	.globl	cube_draw_single
	.type	cube_draw_single, @function
cube_draw_single:
	move.l 4(%sp),%a0	| x, x
	move.l 8(%sp),%a1	| y, y
	move.w 14(%sp),%d0	| type, D.2881
	and.w #3840,%d0	|, D.2881
	cmp.w #768,%d0	|, D.2881
	jeq .L38	|
	jls .L47	|
	move.l #58161,%d1	|, D.2882
	cmp.w #1024,%d0	|, D.2881
	jeq .L42	|
	move.b #45,%d1	|,
	cmp.w #2048,%d0	|, D.2881
	jne .L37	|
.L42:
	move.l %d1,-(%sp)	| D.2882,
	pea 5.w	|
	move.w %a1,%d0	|, D.2883
	sub.w state+10,%d0	| state.cam_y, D.2883
	move.w %d0,%a1	| D.2883,
	move.l %a1,-(%sp)	|,
	move.w %a0,%d0	|, D.2883
	sub.w state+8,%d0	| state.cam_x, D.2883
	move.w %d0,%a0	| D.2883,
	move.l %a0,-(%sp)	|,
	jsr sprite_put	|
	lea (16,%sp),%sp	|,
	rts
.L37:
	move.l #58153,%d1	|, D.2882
	move.l %d1,-(%sp)	| D.2882,
	pea 5.w	|
	move.w %a1,%d0	|, D.2883
	sub.w state+10,%d0	| state.cam_y, D.2883
	move.w %d0,%a1	| D.2883,
	move.l %a1,-(%sp)	|,
	move.w %a0,%d0	|, D.2883
	sub.w state+8,%d0	| state.cam_x, D.2883
	move.w %d0,%a0	| D.2883,
	move.l %a0,-(%sp)	|,
	jsr sprite_put	|
	lea (16,%sp),%sp	|,
	rts
.L47:
	cmp.w #512,%d0	|, D.2881
	jne .L37	|
	move.w system_osc,%d1	| system_osc, frame
	and.w #12,%d1	|, frame
	add.w #-7367,%d1	|, frame
	and.l #65535,%d1	|, D.2882
	move.l %d1,-(%sp)	| D.2882,
	pea 5.w	|
	move.w %a1,%d0	|, D.2883
	sub.w state+10,%d0	| state.cam_y, D.2883
	move.w %d0,%a1	| D.2883,
	move.l %a1,-(%sp)	|,
	move.w %a0,%d0	|, D.2883
	sub.w state+8,%d0	| state.cam_x, D.2883
	move.w %d0,%a0	| D.2883,
	move.l %a0,-(%sp)	|,
	jsr sprite_put	|
	lea (16,%sp),%sp	|,
	rts
.L38:
	move.l #58165,%d1	|, D.2882
	move.l %d1,-(%sp)	| D.2882,
	pea 5.w	|
	move.w %a1,%d0	|, D.2883
	sub.w state+10,%d0	| state.cam_y, D.2883
	move.w %d0,%a1	| D.2883,
	move.l %a1,-(%sp)	|,
	move.w %a0,%d0	|, D.2883
	sub.w state+8,%d0	| state.cam_x, D.2883
	move.w %d0,%a0	| D.2883,
	move.l %a0,-(%sp)	|,
	jsr sprite_put	|
	lea (16,%sp),%sp	|,
	rts
	.size	cube_draw_single, .-cube_draw_single
	.align	2
	.globl	cubes_draw
	.type	cubes_draw, @function
cubes_draw:
	movem.l #60,-(%sp)	|,
	lea cubes+2,%a2	|, ivtmp.50
	lea cubes+4,%a3	|, ivtmp.51
	lea cube_draw_single,%a5	|, tmp85
	lea particle_spawn,%a4	|, tmp86
.L55:
	move.w -2(%a2),%d0	| MEM[base: _40, offset: 4294967294B], D.2902
	jeq .L50	|
	cmp.w #4,%d0	|, D.2902
	jeq .L61	|
	cmp.w #5,%d0	|, D.2902
	jeq .L62	|
	cmp.w #6,%d0	|, D.2902
	jeq .L63	|
	move.w (%a2),%d1	| MEM[base: _40, offset: 0B], D.2902
	move.w (%a3),%a1	| MEM[base: _50, offset: 0B], D.2902
	move.w state+10,%a0	| state.cam_y, D.2902
	moveq #7,%d0	|, D.2904
	sub.w state+8,%d0	| state.cam_x, D.2904
	add.w %d1,%d0	| D.2902, D.2904
	cmp.w #334,%d0	|, D.2904
	jhi .L50	|
	lea (-15,%a1),%a1	|, D.2904
	move.w %a1,%d0	| D.2904, cy
	sub.w %a0,%d0	| D.2902, cy
	cmp.w #-15,%d0	|, cy
	jlt .L50	|
	cmp.w #239,%d0	|, cy
	jle .L64	|
.L50:
	lea (12,%a2),%a2	|, ivtmp.50
	lea (12,%a3),%a3	|, ivtmp.51
	cmp.l #cubes+386,%a2	|, ivtmp.50
	jne .L55	|
.L65:
	movem.l (%sp)+,#15360	|,
	rts
.L64:
	moveq #0,%d0	| D.2903
	move.w 8(%a2),%d0	| MEM[base: _40, offset: 8B], D.2903
	move.l %d0,-(%sp)	| D.2903,
	move.w %a1,-(%sp)	| D.2904,
	clr.w -(%sp)	|
	subq.w #8,%d1	|, D.2902
	move.w %d1,-(%sp)	| D.2902,
	clr.w -(%sp)	|
	jsr (%a5)	| tmp85
	lea (12,%sp),%sp	|,
	lea (12,%a2),%a2	|, ivtmp.50
	lea (12,%a3),%a3	|, ivtmp.51
	cmp.l #cubes+386,%a2	|, ivtmp.50
	jne .L55	|
	jra .L65	|
.L61:
	pea 1.w	|
	moveq #0,%d0	| D.2903
	move.w (%a3),%d0	|* ivtmp.51, D.2903
	move.l %d0,-(%sp)	| D.2903,
	move.w (%a2),%d0	| MEM[base: _40, offset: 0B], D.2903
	move.l %d0,-(%sp)	| D.2903,
	jsr (%a4)	| tmp86
	lea (12,%sp),%sp	|,
.L66:
	lea (12,%a2),%a2	|, ivtmp.50
	lea (12,%a3),%a3	|, ivtmp.51
	cmp.l #cubes+386,%a2	|, ivtmp.50
	jne .L55	|
	jra .L65	|
.L62:
	pea 2.w	|
	moveq #0,%d0	| D.2903
	move.w (%a3),%d0	|* ivtmp.51, D.2903
	move.l %d0,-(%sp)	| D.2903,
	move.w (%a2),%d0	| MEM[base: _40, offset: 0B], D.2903
	move.l %d0,-(%sp)	| D.2903,
	jsr (%a4)	| tmp86
	lea (12,%sp),%sp	|,
	jra .L66	|
.L63:
	pea 3.w	|
	moveq #0,%d0	| D.2903
	move.w (%a3),%d0	|* ivtmp.51, D.2903
	move.l %d0,-(%sp)	| D.2903,
	move.w (%a2),%d0	| MEM[base: _40, offset: 0B], D.2903
	move.l %d0,-(%sp)	| D.2903,
	jsr (%a4)	| tmp86
	lea (12,%sp),%sp	|,
	jra .L66	|
	.size	cubes_draw, .-cubes_draw
	.align	2
	.globl	cube_spawn
	.type	cube_spawn, @function
cube_spawn:
	movem.l #14336,-(%sp)	|,
	move.l 16(%sp),%d3	| x, x
	move.l 20(%sp),%d2	| y, y
	move.l 24(%sp),%d0	| type, type
	move.l 28(%sp),%d4	| state, state
	move.l 32(%sp),%a1	| dx, dx
	move.l 36(%sp),%d1	| dy, dy
	lea cubes,%a0	|, ivtmp.60
.L71:
	tst.w (%a0)	| MEM[base: c_4, offset: 0B]
	jeq .L75	|
	lea (12,%a0),%a0	|, ivtmp.60
	cmp.l #cubes+384,%a0	|, ivtmp.60
	jne .L71	|
	movem.l (%sp)+,#28	|,
	rts
.L75:
	move.w %d4,(%a0)	| state, c_19->state
	move.w %d3,2(%a0)	| x, c_19->x
	move.w %d2,4(%a0)	| y, c_19->y
	move.w %a1,6(%a0)	| dx, c_19->dx
	move.w %d1,8(%a0)	| dy, c_19->dy
	move.w %d0,10(%a0)	| type, c_19->type
	movem.l (%sp)+,#28	|,
	rts
	.size	cube_spawn, .-cube_spawn
	.local	cubes
	.comm	cubes,384,2
	.ident	"GCC: (GNU) 4.8.2"
