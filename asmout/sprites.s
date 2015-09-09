	.file	"sprites.c"
| GNU C (GCC) version 4.8.2 (m68k-elf)
|	compiled by GNU C version 4.9.3, GMP version 5.0.5, MPFR version 2.4.2, MPC version 0.8.2
| GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
| options passed:  -I /opt/toolchains/gen/m68k-elf/include
| -I /opt/toolchains/gen/m68k-elf/m68k-elf/include -I src -I res -I inc
| src/sprites.c -mcpu=68000 -O2 -std=c99 -fomit-frame-pointer -fno-builtin
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
	.globl	sprites_init
	.type	sprites_init, @function
sprites_init:
	jsr VDP_getSpriteListAddress	|
	move.w %d0,sprite_addr	|, sprite_addr
	clr.b next_spr	| next_spr
	rts
	.size	sprites_init, .-sprites_init
	.align	2
	.globl	sprites_dma
	.type	sprites_dma, @function
sprites_dma:
	move.l 4(%sp),%d0	| spr_num, spr_num
	add.w %d0,%d0	| spr_num, D.2601
	add.w %d0,%d0	| D.2601, D.2601
	move.w %d0,-(%sp)	| D.2601,
	clr.w -(%sp)	|
	moveq #0,%d0	| D.2602
	move.w sprite_addr,%d0	| sprite_addr, D.2602
	move.l %d0,-(%sp)	| D.2602,
	pea sprite_table	|
	clr.l -(%sp)	|
	jsr VDP_doDMA	|
	lea (16,%sp),%sp	|,
	rts
	.size	sprites_dma, .-sprites_dma
	.align	2
	.globl	sprites_clamp_link
	.type	sprites_clamp_link, @function
sprites_clamp_link:
	moveq #0,%d0	| D.2605
	move.b 7(%sp),%d0	| num, D.2605
	move.l %d0,%a0	| D.2605, tmp39
	add.l %d0,%a0	| D.2605, tmp39
	add.l %a0,%a0	| tmp39, D.2605
	add.l %a0,%a0	| D.2605, tmp41
	add.l #sprite_table,%a0	|, addr
	and.w #-256,2(%a0)	|, MEM[(short unsigned int *)addr_4 + 2B]
	rts
	.size	sprites_clamp_link, .-sprites_clamp_link
	.align	2
	.globl	sprites_dma_simple
	.type	sprites_dma_simple, @function
sprites_dma_simple:
	move.b next_spr,%d0	| next_spr, next_spr.3
	jne .L10	|
	rts
.L10:
	move.b %d0,%d1	| next_spr.3, D.2610
	subq.b #1,%d1	|, D.2610
	and.l #255,%d1	|, D.2612
	move.l %d1,%a0	| D.2612, tmp46
	add.l %d1,%a0	| D.2612, tmp46
	add.l %a0,%a0	| tmp46, D.2612
	add.l %a0,%a0	| D.2612, tmp48
	add.l #sprite_table,%a0	|, addr
	and.w #-256,2(%a0)	|, MEM[(short unsigned int *)addr_16 + 2B]
	and.w #255,%d0	|, D.2611
	add.w %d0,%d0	| D.2611, D.2611
	add.w %d0,%d0	| D.2611, D.2611
	move.l %d0,%d1	|,
	and.l #1023,%d1	|,
	move.l %d1,-(%sp)	|,
	moveq #0,%d0	| D.2612
	move.w sprite_addr,%d0	| sprite_addr, D.2612
	move.l %d0,-(%sp)	| D.2612,
	pea sprite_table	|
	clr.l -(%sp)	|
	jsr VDP_doDMA	|
	clr.b next_spr	| next_spr
	lea (16,%sp),%sp	|,
	rts
	.size	sprites_dma_simple, .-sprites_dma_simple
	.align	2
	.globl	sprite_set
	.type	sprite_set, @function
sprite_set:
	movem.l #14368,-(%sp)	|,
	move.l 20(%sp),%d0	| num, num
	move.l 24(%sp),%d1	| x, x
	move.l 28(%sp),%d4	| y, y
	move.l 32(%sp),%d3	| size, size
	move.l 36(%sp),%a1	| attr, attr
	move.l 40(%sp),%d2	| link, link
	cmp.b #79,%d0	|, num
	jhi .L12	|
	and.l #255,%d0	|, D.2615
	add.l %d0,%d0	| D.2615, tmp55
	add.l %d0,%d0	| tmp55, tmp56
	add.l %d0,%d0	| tmp56, tmp57
	move.l %d0,%a0	| tmp57, addr
	add.l #sprite_table,%a0	|, addr
	lea sprite_table,%a2	|, tmp58
	add.w #128,%d4	|,
	move.w %d4,(%a2,%d0.l)	|, sprite_table
	lsl.w #8,%d3	|, D.2616
	and.w #255,%d2	|, D.2616
	add.w %d2,%d3	| D.2616, D.2616
	move.w %d3,2(%a0)	| D.2616, MEM[(short unsigned int *)addr_6 + 2B]
	move.w %a1,4(%a0)	| attr, MEM[(short unsigned int *)addr_6 + 4B]
	add.w #128,%d1	|,
	move.w %d1,6(%a0)	|, MEM[(short unsigned int *)addr_6 + 6B]
.L12:
	movem.l (%sp)+,#1052	|,
	rts
	.size	sprite_set, .-sprite_set
	.align	2
	.globl	sprite_put
	.type	sprite_put, @function
sprite_put:
	movem.l #14368,-(%sp)	|,
	move.l 20(%sp),%d2	| x, x
	move.l 24(%sp),%d4	| y, y
	move.l 28(%sp),%d3	| size, size
	move.l 32(%sp),%a1	| attr, attr
	move.b next_spr,%d0	| next_spr, next_spr.8
	move.b %d0,%d1	| next_spr.8, D.2625
	addq.b #1,%d1	|, D.2625
	cmp.b #79,%d0	|, next_spr.8
	jhi .L17	|
	and.l #255,%d0	|, D.2626
	add.l %d0,%d0	| D.2626, tmp53
	add.l %d0,%d0	| tmp53, tmp54
	add.l %d0,%d0	| tmp54, tmp55
	move.l %d0,%a0	| tmp55, addr
	add.l #sprite_table,%a0	|, addr
	lea sprite_table,%a2	|, tmp56
	add.w #128,%d4	|,
	move.w %d4,(%a2,%d0.l)	|, sprite_table
	lsl.w #8,%d3	|, D.2627
	clr.w %d0	| D.2627
	move.b %d1,%d0	| D.2625, D.2627
	add.w %d0,%d3	| D.2627, D.2627
	move.w %d3,2(%a0)	| D.2627, MEM[(short unsigned int *)addr_12 + 2B]
	move.w %a1,4(%a0)	| attr, MEM[(short unsigned int *)addr_12 + 4B]
	add.w #128,%d2	|,
	move.w %d2,6(%a0)	|, MEM[(short unsigned int *)addr_12 + 6B]
.L17:
	move.b %d1,next_spr	| D.2625, next_spr
	movem.l (%sp)+,#1052	|,
	rts
	.size	sprite_put, .-sprite_put
	.align	2
	.globl	sprites_get_next_sprite
	.type	sprites_get_next_sprite, @function
sprites_get_next_sprite:
	move.b next_spr,%d0	| next_spr,
	rts
	.size	sprites_get_next_sprite, .-sprites_get_next_sprite
	.align	2
	.globl	sprites_get_sprite_addr
	.type	sprites_get_sprite_addr, @function
sprites_get_sprite_addr:
	move.w sprite_addr,%d0	| sprite_addr,
	rts
	.size	sprites_get_sprite_addr, .-sprites_get_sprite_addr
	.align	2
	.globl	sprites_get_table
	.type	sprites_get_table, @function
sprites_get_table:
	move.l #sprite_table,%d0	|,
	rts
	.size	sprites_get_table, .-sprites_get_table
	.local	sprite_table
	.comm	sprite_table,640,2
	.local	sprite_addr
	.comm	sprite_addr,2,2
	.local	next_spr
	.comm	next_spr,1,1
	.ident	"GCC: (GNU) 4.8.2"
