	.file	"particle.c"
| GNU C (GCC) version 4.8.2 (m68k-elf)
|	compiled by GNU C version 4.9.3, GMP version 5.0.5, MPFR version 2.4.2, MPC version 0.8.2
| GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
| options passed:  -I /opt/toolchains/gen/m68k-elf/include
| -I /opt/toolchains/gen/m68k-elf/m68k-elf/include -I src -I res -I inc
| src/particle.c -mcpu=68000 -O2 -std=c99 -fomit-frame-pointer -fno-builtin
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
	.globl	particles_dma_tiles
	.type	particles_dma_tiles, @function
particles_dma_tiles:
	pea 1232.w	|
	pea 27936.w	|
	pea gfx_particles	|
	clr.l -(%sp)	|
	jsr VDP_doDMA	|
	lea (16,%sp),%sp	|,
	rts
	.size	particles_dma_tiles, .-particles_dma_tiles
	.align	2
	.globl	particles_init
	.type	particles_init, @function
particles_init:
	lea particles+218,%a0	|, ivtmp.22
.L4:
	clr.w (%a0)	| MEM[base: _2, offset: 0B]
	lea (-14,%a0),%a0	|, ivtmp.22
	cmp.l #particles-6,%a0	|, ivtmp.22
	jne .L4	|
	rts
	.size	particles_init, .-particles_init
	.align	2
	.globl	particles_run
	.type	particles_run, @function
particles_run:
	movem.l #15408,-(%sp)	|,
	move.w system_osc,%d4	| system_osc, D.2821
	and.w #1,%d4	|, D.2821
	lea particles+218,%a0	|, ivtmp.33
	lea particles+220,%a1	|, ivtmp.36
	lea particles+210,%a3	|, ivtmp.38
	lea particles+212,%a2	|, ivtmp.39
.L13:
	tst.w (%a0)	| MEM[base: _59, offset: 0B]
	jeq .L8	|
	tst.w 4(%a0)	| MEM[base: _59, offset: 4B]
	jeq .L10	|
	move.w -4(%a0),%d3	| MEM[base: _59, offset: 4294967292B], D.2822
	move.w %d3,%d1	| D.2822, D.2821
	moveq #15,%d0	|,
	lsr.w %d0,%d1	|, D.2821
	add.w %d3,%d1	| D.2822, D.2821
	asr.w #1,%d1	|, D.2821
	add.w (%a3),%d1	| MEM[base: _68, offset: 0B], D.2821
	move.w %d1,(%a3)	| D.2821, MEM[base: _68, offset: 0B]
	move.w -2(%a0),%d2	| MEM[base: _59, offset: 4294967294B], D.2822
	move.w %d2,%d0	| D.2822, D.2821
	moveq #15,%d5	|,
	lsr.w %d5,%d0	|, D.2821
	add.w %d2,%d0	| D.2822, D.2821
	asr.w #1,%d0	|, D.2821
	add.w (%a2),%d0	| MEM[base: _71, offset: 0B], D.2821
	move.w %d0,(%a2)	| D.2821, MEM[base: _71, offset: 0B]
	tst.w %d4	| D.2821
	jeq .L10	|
	and.w #1,%d3	|, D.2821
	add.w %d3,%d1	| D.2821, D.2821
	move.w %d1,(%a3)	| D.2821, MEM[base: _68, offset: 0B]
	and.w #1,%d2	|, D.2821
	add.w %d2,%d0	| D.2821, D.2821
	move.w %d0,(%a2)	| D.2821, MEM[base: _71, offset: 0B]
.L10:
	addq.w #1,(%a1)	|, MEM[base: _62, offset: 0B]
	subq.w #1,(%a0)	|, MEM[base: _59, offset: 0B]
.L8:
	lea (-14,%a0),%a0	|, ivtmp.33
	lea (-14,%a1),%a1	|, ivtmp.36
	lea (-14,%a3),%a3	|, ivtmp.38
	lea (-14,%a2),%a2	|, ivtmp.39
	cmp.l #particles-6,%a0	|, ivtmp.33
	jne .L13	|
	movem.l (%sp)+,#3132	|,
	rts
	.size	particles_run, .-particles_run
	.align	2
	.globl	particles_draw
	.type	particles_draw, @function
particles_draw:
	movem.l #14384,-(%sp)	|,
	lea particles+218,%a2	|, ivtmp.50
	lea sprite_put,%a3	|, tmp78
.L29:
	tst.w (%a2)	| MEM[base: _1, offset: 0B]
	jeq .L20	|
	move.w 4(%a2),%d0	| MEM[base: _1, offset: 4B], D.2840
	cmp.w #2,%d0	|, D.2840
	jeq .L22	|
	cmp.w #3,%d0	|, D.2840
	jeq .L23	|
	cmp.w #1,%d0	|, D.2840
	jeq .L24	|
	move.w (%a2),%d0	| MEM[base: _1, offset: 0B], D.2840
	moveq #5,%d4	|, size
	moveq #8,%d1	|, cent
	cmp.w #16,%d0	|, D.2840
	jhi .L33	|
	move.w #873,%a1	|, gfx
	cmp.w #12,%d0	|, D.2840
	jhi .L25	|
	move.w #877,%a1	|, gfx
	cmp.w #8,%d0	|, D.2840
	jls .L43	|
.L25:
	move.w -8(%a2),%d0	| MEM[base: _1, offset: 4294967288B], D.2841
	sub.w state+8,%d0	| state.cam_x, D.2841
	sub.w %d1,%d0	| cent, D.2841
	move.w -6(%a2),%d2	| MEM[base: _1, offset: 4294967290B], D.2840
	move.w state+10,%a0	| state.cam_y, D.2840
	move.w %d0,%d3	| D.2841, D.2841
	add.w #32,%d3	|, D.2841
	cmp.w #368,%d3	|, D.2841
	jhi .L20	|
	sub.w %a0,%d2	| D.2840, ty
	sub.w %d1,%d2	| cent, ty
	cmp.w #-32,%d2	|, ty
	jlt .L20	|
	cmp.w #256,%d2	|, ty
	jle .L44	|
.L20:
	lea (-14,%a2),%a2	|, ivtmp.50
	cmp.l #particles-6,%a2	|, ivtmp.50
	jne .L29	|
.L45:
	movem.l (%sp)+,#3100	|,
	rts
.L44:
	lea (-8192,%a1),%a1	|, D.2840
	move.w %a1,-(%sp)	| D.2840,
	clr.w -(%sp)	|
	moveq #15,%d1	|,
	and.l %d4,%d1	| size,
	move.l %d1,-(%sp)	|,
	move.w %d2,%a0	| ty,
	move.l %a0,-(%sp)	|,
	move.w %d0,%a0	| D.2841,
	move.l %a0,-(%sp)	|,
	jsr (%a3)	| tmp78
	lea (16,%sp),%sp	|,
	lea (-14,%a2),%a2	|, ivtmp.50
	cmp.l #particles-6,%a2	|, ivtmp.50
	jne .L29	|
	jra .L45	|
.L22:
	move.w (%a2),%d0	| MEM[base: _1, offset: 0B], D.2840
	moveq #5,%d4	|, size
	moveq #8,%d1	|, cent
	move.w #905,%a1	|, gfx
	cmp.w #12,%d0	|, D.2840
	jhi .L25	|
	move.w #909,%a1	|, gfx
	cmp.w #8,%d0	|, D.2840
	jhi .L25	|
	move.w #913,%a1	|, gfx
	cmp.w #4,%d0	|, D.2840
	jhi .L25	|
	move.w #917,%a1	|, gfx
	jra .L25	|
.L24:
	move.w (%a2),%d0	| MEM[base: _1, offset: 0B], D.2840
	moveq #5,%d4	|, size
	moveq #8,%d1	|, cent
	move.w #889,%a1	|, gfx
	cmp.w #12,%d0	|, D.2840
	jhi .L25	|
	move.w #893,%a1	|, gfx
	cmp.w #8,%d0	|, D.2840
	jhi .L25	|
	move.w #897,%a1	|, gfx
	cmp.w #4,%d0	|, D.2840
	jhi .L25	|
	move.w #901,%a1	|, gfx
	jra .L25	|
.L23:
	move.w (%a2),%d0	| MEM[base: _1, offset: 0B], D.2840
	cmp.w #18,%d0	|, D.2840
	jhi .L41	|
	move.w %d0,%d1	| D.2840, D.2840
	add.w #-13,%d1	|, D.2840
	cmp.w #2,%d1	|, D.2840
	jls .L41	|
	subq.w #3,%d1	|, D.2840
	cmp.w #2,%d1	|, D.2840
	jhi .L26	|
.L27:
	moveq #5,%d4	|, size
	moveq #8,%d1	|, cent
	move.w #921,%a1	|, gfx
	jra .L25	|
.L41:
	moveq #10,%d4	|, size
	moveq #12,%d1	|, cent
	move.w #925,%a1	|, gfx
	jra .L25	|
.L43:
	move.w #881,%a1	|, gfx
	cmp.w #4,%d0	|, D.2840
	jhi .L25	|
.L33:
	move.w #885,%a1	|, gfx
	jra .L25	|
.L26:
	subq.w #7,%d0	|, D.2840
	cmp.w #2,%d0	|, D.2840
	jls .L27	|
	moveq #15,%d4	|, size
	moveq #16,%d1	|, cent
	move.w #934,%a1	|, gfx
	jra .L25	|
	.size	particles_draw, .-particles_draw
	.align	2
	.globl	particle_spawn
	.type	particle_spawn, @function
particle_spawn:
	movem.l #14368,-(%sp)	|,
	move.l 20(%sp),%d4	| x, x
	move.l 24(%sp),%a1	| y, y
	move.l 28(%sp),%d1	| type, type
	move.w %d1,%d3	| type, type
	lea particles+218,%a0	|, ivtmp.64
	moveq #15,%d0	|, i
.L61:
	tst.w (%a0)	| MEM[base: _66, offset: 0B]
	jeq .L68	|
	subq.l #1,%d0	|, i
	lea (-14,%a0),%a0	|, ivtmp.64
	moveq #-1,%d2	|,
	cmp.l %d0,%d2	| i,
	jne .L61	|
.L47:
	movem.l (%sp)+,#1052	|,
	rts
.L68:
	lea particles,%a2	|, tmp209
	add.l %d0,%d0	| i, tmp210
	move.l %d0,%d2	| tmp210, tmp74
	lsl.l #3,%d2	|, tmp74
	sub.l %d0,%d2	| tmp210, tmp74
	move.w %d4,(%a2,%d2.l)	| x, particles[i_2].x
	move.l %d2,%a0	| tmp74, tmp80
	add.l %a2,%a0	|, tmp80
	move.w %a1,2(%a0)	| y, particles[i_2].y
	move.w %d1,12(%a0)	| type, particles[i_2].type
	cmp.w #2,%d3	|, type
	jhi .L50	|
	cmp.w #1,%d3	|, type
	jcc .L69	|
	move.w #20,8(%a0)	|, particles[i_2].active
	clr.w 10(%a0)	| particles[i_2].anim_cnt
	move.w 12582920,%d2	| MEM[(volatile short unsigned int *)12582920B], D.2855
	move.l %d0,%d1	| tmp210, tmp129
	lsl.l #3,%d1	|, tmp129
	sub.l %d0,%d1	| tmp210, tmp129
	move.w (%a2,%d1.l),%d0	| particles[i_2].x, D.2855
	subq.w #8,%d0	|, D.2855
	add.w system_osc,%d2	| system_osc, D.2855
	and.w #15,%d2	|, D.2855
	add.w %d2,%d0	| D.2855, D.2855
	move.w %d0,(%a2,%d1.l)	| D.2855, particles[i_2].x
	move.w 12582920,%d0	| MEM[(volatile short unsigned int *)12582920B], D.2855
	move.l %d1,%a0	| tmp129, tmp145
	add.l #particles+2,%a0	|, tmp145
	move.w (%a0),%d1	| particles[i_2].y, D.2855
	subq.w #8,%d1	|, D.2855
	and.w #15,%d0	|, D.2855
	add.w %d0,%d1	| D.2855, D.2855
	move.w %d1,(%a0)	| D.2855,
.L71:
	movem.l (%sp)+,#1052	|,
	rts
.L69:
	move.w #16,8(%a0)	|, particles[i_2].active
	move.l %d0,%d1	| tmp210, tmp206
	lsl.l #3,%d1	|, tmp206
	sub.l %d0,%d1	| tmp210, tmp206
	move.l %d1,%a0	| tmp206, tmp208
	add.l #particles+10,%a0	|, tmp208
	clr.w (%a0)	| particles[i_2].anim_cnt
.L56:
	move.w 12582920,%d2	| MEM[(volatile short unsigned int *)12582920B], D.2856
	move.l %d0,%d1	| tmp210, tmp160
	lsl.l #3,%d1	|, tmp160
	sub.l %d0,%d1	| tmp210, tmp160
	move.l %d1,%a0	| tmp160, tmp161
	add.l #particles,%a0	|, tmp161
	lea (6,%a0),%a1	|, tmp161, tmp162
	add.w system_osc,%d2	| system_osc, D.2856
	and.w #3,%d2	|, D.2856
	move.w %d2,%d1	| D.2856,
	subq.w #2,%d1	|,
	move.w %d1,(%a1)	|, particles[i_2].dy
	move.w 12582920,%d1	| MEM[(volatile short unsigned int *)12582920B], D.2854
	and.w #3,%d1	|, D.2854
	subq.w #2,%d1	|, D.2854
	move.w %d1,4(%a0)	| D.2854, particles[i_2].dx
	move.w %d2,%d3	| D.2856, D.2854
	subq.w #2,%d3	|, D.2854
	jmi .L58	|
	subq.w #1,%d2	|, D.2856
	move.w %d2,(%a1)	| D.2856, particles[i_2].dy
.L58:
	tst.w %d1	| D.2854
	jlt .L47	|
	move.l %d0,%d2	| tmp210, tmp199
	lsl.l #3,%d2	|, tmp199
	sub.l %d0,%d2	| tmp210, tmp199
	move.l %d2,%a0	| tmp199, tmp201
	add.l #particles+4,%a0	|, tmp201
	addq.w #1,%d1	|, D.2854
	move.w %d1,(%a0)	| D.2854,
	movem.l (%sp)+,#1052	|,
	rts
.L50:
	cmp.w #3,%d3	|, type
	jne .L70	|
	move.w #21,8(%a0)	|, particles[i_2].active
	move.l %d0,%d1	| tmp210, tmp206
	lsl.l #3,%d1	|, tmp206
	sub.l %d0,%d1	| tmp210, tmp206
	move.l %d1,%a0	| tmp206, tmp208
	add.l #particles+10,%a0	|, tmp208
	clr.w (%a0)	| particles[i_2].anim_cnt
	jra .L56	|
.L70:
	clr.w 10(%a0)	| particles[i_2].anim_cnt
	tst.w %d3	| type
	jne .L56	|
	move.w 12582920,%d2	| MEM[(volatile short unsigned int *)12582920B], D.2855
	move.l %d0,%d1	| tmp210, tmp129
	lsl.l #3,%d1	|, tmp129
	sub.l %d0,%d1	| tmp210, tmp129
	move.w (%a2,%d1.l),%d0	| particles[i_2].x, D.2855
	subq.w #8,%d0	|, D.2855
	add.w system_osc,%d2	| system_osc, D.2855
	and.w #15,%d2	|, D.2855
	add.w %d2,%d0	| D.2855, D.2855
	move.w %d0,(%a2,%d1.l)	| D.2855, particles[i_2].x
	move.w 12582920,%d0	| MEM[(volatile short unsigned int *)12582920B], D.2855
	move.l %d1,%a0	| tmp129, tmp145
	add.l #particles+2,%a0	|, tmp145
	move.w (%a0),%d1	| particles[i_2].y, D.2855
	subq.w #8,%d1	|, D.2855
	and.w #15,%d0	|, D.2855
	add.w %d0,%d1	| D.2855, D.2855
	move.w %d1,(%a0)	| D.2855,
	jra .L71	|
	.size	particle_spawn, .-particle_spawn
	.local	particles
	.comm	particles,224,2
	.ident	"GCC: (GNU) 4.8.2"
