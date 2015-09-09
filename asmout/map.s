	.file	"map.c"
| GNU C (GCC) version 4.8.2 (m68k-elf)
|	compiled by GNU C version 4.9.3, GMP version 5.0.5, MPFR version 2.4.2, MPC version 0.8.2
| GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
| options passed:  -I /opt/toolchains/gen/m68k-elf/include
| -I /opt/toolchains/gen/m68k-elf/m68k-elf/include -I src -I res -I inc
| src/map.c -mcpu=68000 -O2 -std=c99 -fomit-frame-pointer -fno-builtin
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
	.globl	map_dma_queue
	.type	map_dma_queue, @function
map_dma_queue:
	move.l %a2,-(%sp)	|,
	move.l %d2,-(%sp)	|,
	move.l 16(%sp),%d2	| dest, dest
	move.l 20(%sp),%a1	| len, len
	move.w map_dma_queue_depth,%d0	| map_dma_queue_depth, map_dma_queue_depth.0
	cmp.w #96,%d0	|, map_dma_queue_depth.0
	jeq .L1	|
	moveq #0,%d1	| D.2915
	move.w %d0,%d1	| map_dma_queue_depth.0, D.2915
	move.l %d1,%a0	| D.2915, tmp40
	add.l %d1,%a0	| D.2915, tmp40
	move.l %a0,%a2	| tmp40, tmp41
	add.l #map_dma_src_queue,%a2	|, tmp41
	move.l 12(%sp),(%a2,%a0.l)	| src, map_dma_src_queue
	lea map_dma_dest_queue,%a2	|, tmp42
	move.w %d2,(%a0,%a2.l)	| dest, map_dma_dest_queue
	lea map_dma_len_queue,%a2	|, tmp44
	move.w %a1,(%a0,%a2.l)	| len, map_dma_len_queue
	addq.w #1,%d0	|, map_dma_queue_depth.0
	move.w %d0,map_dma_queue_depth	| map_dma_queue_depth.0, map_dma_queue_depth
.L1:
	move.l (%sp)+,%d2	|,
	move.l (%sp)+,%a2	|,
	rts
	.size	map_dma_queue, .-map_dma_queue
	.align	2
	.globl	map_load_tileset
	.type	map_load_tileset, @function
map_load_tileset:
	move.l %a2,-(%sp)	|,
	move.l %d2,-(%sp)	|,
	move.b 15(%sp),%d0	| num, num
	cmp.b #1,%d0	|, num
	jeq .L10	|
	cmp.b #2,%d0	|, num
	jne .L14	|
	move.l #gfx_inside1,%d0	|, tsrc_ptr
	move.l #pal_inside1,%d2	|, psrc_ptr
	pea 4096.w	|
	clr.l -(%sp)	|
	move.l %d0,-(%sp)	| tsrc_ptr,
	clr.l -(%sp)	|
	lea VDP_doDMA,%a2	|, tmp35
	jsr (%a2)	| tmp35
	pea 16.w	|
	clr.l -(%sp)	|
	move.l %d2,-(%sp)	| psrc_ptr,
	pea 1.w	|
	jsr (%a2)	| tmp35
	clr.w map_dma_queue_depth	| map_dma_queue_depth
	clr.w map_dma_h_len	| map_dma_h_len
	clr.w map_dma_h_len+2	| map_dma_h_len
	lea (32,%sp),%sp	|,
	move.l (%sp)+,%d2	|,
	move.l (%sp)+,%a2	|,
	rts
.L14:
	move.l #gfx_outside1,%d0	|, tsrc_ptr
	move.l #pal_outside1,%d2	|, psrc_ptr
	pea 4096.w	|
	clr.l -(%sp)	|
	move.l %d0,-(%sp)	| tsrc_ptr,
	clr.l -(%sp)	|
	lea VDP_doDMA,%a2	|, tmp35
	jsr (%a2)	| tmp35
	pea 16.w	|
	clr.l -(%sp)	|
	move.l %d2,-(%sp)	| psrc_ptr,
	pea 1.w	|
	jsr (%a2)	| tmp35
	clr.w map_dma_queue_depth	| map_dma_queue_depth
	clr.w map_dma_h_len	| map_dma_h_len
	clr.w map_dma_h_len+2	| map_dma_h_len
	lea (32,%sp),%sp	|,
	move.l (%sp)+,%d2	|,
	move.l (%sp)+,%a2	|,
	rts
.L10:
	move.l #gfx_outside2,%d0	|, tsrc_ptr
	move.l #pal_outside2,%d2	|, psrc_ptr
	pea 4096.w	|
	clr.l -(%sp)	|
	move.l %d0,-(%sp)	| tsrc_ptr,
	clr.l -(%sp)	|
	lea VDP_doDMA,%a2	|, tmp35
	jsr (%a2)	| tmp35
	pea 16.w	|
	clr.l -(%sp)	|
	move.l %d2,-(%sp)	| psrc_ptr,
	pea 1.w	|
	jsr (%a2)	| tmp35
	clr.w map_dma_queue_depth	| map_dma_queue_depth
	clr.w map_dma_h_len	| map_dma_h_len
	clr.w map_dma_h_len+2	| map_dma_h_len
	lea (32,%sp),%sp	|,
	move.l (%sp)+,%d2	|,
	move.l (%sp)+,%a2	|,
	rts
	.size	map_load_tileset, .-map_load_tileset
	.align	2
	.globl	map_by_id
	.type	map_by_id, @function
map_by_id:
	move.b 7(%sp),%d0	| num, num
	cmp.b mapdata_roomzero+33.l,%d0	| MEM[(struct map_file *)&mapdata_roomzero].id, num
	jeq .L21	|
	lea maplist+4,%a1	|, ivtmp.32
.L18:
	move.l (%a1)+,%a0	| MEM[base: _14, offset: 0B], tf
	cmp.w #0,%a0	| tf
	jeq .L25	|
	cmp.b 33(%a0),%d0	| tf_5->id, num
	jne .L18	|
	move.l %a0,%d0	| tf, D.2932
	rts
.L25:
	clr.l 4(%sp)	|
	jra map_by_id	|
.L21:
	move.l #mapdata_roomzero,%d0	|, D.2932
	rts
	.size	map_by_id, .-map_by_id
	.align	2
	.globl	map_draw_horizontal
	.type	map_draw_horizontal, @function
map_draw_horizontal:
	movem.l #15422,-(%sp)	|,
	move.l 40(%sp),%d1	| cam_x, cam_x
	move.l 44(%sp),%d2	| cam_y, cam_y
	move.w 50(%sp),%d5	| right_side, right_side
	move.l state,%a0	| state.current_room, state.current_room
	clr.w %d4	| map_width
	move.b 35(%a0),%d4	| _12->w, map_width
	muls.w #80,%d4	|, map_width
	move.w %d1,%d0	|, src_xcomp
	lsr.w #3,%d0	|, src_xcomp
	add.w %d0,%d0	| src_xcomp, src_xcomp
	tst.w %d5	| right_side
	jeq .L34	|
	move.w #80,%a0	|, iftmp.3
.L28:
	lsr.l #3,%d1	|, D.2967
	moveq #63,%d3	|,
	and.l %d3,%d1	|, D.2967
	move.l %d2,%d3	| cam_y, D.2967
	lsl.l #3,%d3	|, D.2967
	and.l #1984,%d3	|, D.2967
	add.l %d1,%d3	| D.2967, D.2967
	add.w %d3,%d3	| D.2967, dma_dest
	lsr.w #3,%d2	|, src_ycomp
	muls.w %d4,%d2	| map_width, src_ycomp
	and.l #65535,%d2	|, D.2969
	move.l state+4,%a3	| state.current_map, D.2969
	add.l %d2,%a3	| D.2969, D.2969
	add.w %a0,%d0	| iftmp.3, src_xcomp
	and.l #65535,%d0	|, D.2969
	add.l %d0,%a3	| D.2969, dma_src
	tst.w %d5	| right_side
	jeq .L29	|
	moveq #23,%d0	|,
	cmp.l %d1,%d0	| D.2967,
	jge .L39	|
	add.w #-48,%d3	|, dma_dest
.L29:
	lea VDP_getAPlanAddress,%a5	|, tmp131
	jsr (%a5)	| tmp131
	add.w %d3,%d0	| dma_dest,
	move.w %d0,map_dma_h_dest	|, map_dma_h_dest
	clr.w map_dma_h_len	| map_dma_h_len
	clr.w map_dma_h_len+2	| map_dma_h_len
	and.l #65535,%d4	|, D.2969
	lea map_dma_h_src_queue,%a2	|, ivtmp.58
	clr.w %d0	| D.2966
	clr.w %d5	| current_dma
	moveq #0,%d1	| D.2967
	lea map_dma_h_len,%a4	|, tmp130
	lea map_dma_h_dest,%a6	|, tmp132
.L33:
	move.w (%a3),(%a2)+	| *dma_src.4_48, MEM[base: _67, offset: 0B]
	add.l %d4,%a3	| D.2969, dma_src
	move.l %d1,%a0	| D.2967, tmp121
	add.l %d1,%a0	| D.2967, tmp121
	addq.w #1,%d0	|, D.2966
	move.w %d0,(%a4,%a0.l)	| D.2966, map_dma_h_len
	move.w %d3,%d2	| dma_dest, dma_dest
	add.w #128,%d2	|, dma_dest
	cmp.w #4095,%d2	|, dma_dest
	jhi .L40	|
.L31:
	cmp.l #map_dma_h_src_queue+64,%a2	|, ivtmp.58
	jeq .L27	|
.L41:
	move.l %d1,%d0	| D.2967, tmp128
	add.l %d1,%d0	| D.2967, tmp128
	move.w (%a4,%d0.l),%d0	| map_dma_h_len, D.2966
	move.w %d2,%d3	| dma_dest, dma_dest
	move.w (%a3),(%a2)+	| *dma_src.4_48, MEM[base: _67, offset: 0B]
	add.l %d4,%a3	| D.2969, dma_src
	move.l %d1,%a0	| D.2967, tmp121
	add.l %d1,%a0	| D.2967, tmp121
	addq.w #1,%d0	|, D.2966
	move.w %d0,(%a4,%a0.l)	| D.2966, map_dma_h_len
	move.w %d3,%d2	| dma_dest, dma_dest
	add.w #128,%d2	|, dma_dest
	cmp.w #4095,%d2	|, dma_dest
	jls .L31	|
.L40:
	add.w #-4096,%d2	|, dma_dest
	addq.w #1,%d5	|, current_dma
	jsr (%a5)	| tmp131
	moveq #0,%d1	| D.2967
	move.w %d5,%d1	| current_dma, D.2967
	move.l %d1,%a0	| D.2967, tmp125
	add.l %d1,%a0	| D.2967, tmp125
	add.w %d2,%d0	| dma_dest,
	move.w %d0,(%a6,%a0.l)	|, map_dma_h_dest
	cmp.l #map_dma_h_src_queue+64,%a2	|, ivtmp.58
	jne .L41	|
.L27:
	movem.l (%sp)+,#31804	|,
	rts
.L34:
	sub.l %a0,%a0	| iftmp.3
	jra .L28	|
.L39:
	add.w #80,%d3	|, dma_dest
	lea VDP_getAPlanAddress,%a5	|, tmp131
	jsr (%a5)	| tmp131
	add.w %d3,%d0	| dma_dest,
	move.w %d0,map_dma_h_dest	|, map_dma_h_dest
	clr.w map_dma_h_len	| map_dma_h_len
	clr.w map_dma_h_len+2	| map_dma_h_len
	and.l #65535,%d4	|, D.2969
	lea map_dma_h_src_queue,%a2	|, ivtmp.58
	clr.w %d0	| D.2966
	clr.w %d5	| current_dma
	moveq #0,%d1	| D.2967
	lea map_dma_h_len,%a4	|, tmp130
	lea map_dma_h_dest,%a6	|, tmp132
	jra .L33	|
	.size	map_draw_horizontal, .-map_draw_horizontal
	.align	2
	.globl	map_draw_vertical
	.type	map_draw_vertical, @function
map_draw_vertical:
	movem.l #16184,-(%sp)	|,
	move.l 40(%sp),%d1	| cam_x, cam_x
	move.l 44(%sp),%d2	| cam_y, cam_y
	move.w 50(%sp),%a0	| bottom_side, bottom_side
	move.l state,%a1	| state.current_room, state.current_room
	clr.w %d4	| map_width
	move.b 35(%a1),%d4	| _18->w, map_width
	muls.w #80,%d4	|, map_width
	move.w %d1,%d0	|, plot_x
	and.w #511,%d0	|, plot_x
	lsr.w #3,%d0	|, plot_x
	move.w %d2,%d3	|, plot_y
	and.w #255,%d3	|, plot_y
	lsr.w #3,%d3	|, plot_y
	lsr.w #3,%d2	|, src_ycomp
	muls.w %d4,%d2	| map_width, src_ycomp
	moveq #0,%d5	| D.2989
	move.w %d2,%d5	| src_ycomp, D.2989
	add.l state+4,%d5	| state.current_map, D.2989
	lsr.w #3,%d1	|, src_xcomp
	add.w %d1,%d1	| src_xcomp, src_xcomp
	and.l #65535,%d1	|, D.2989
	add.l %d1,%d5	| D.2989, dma_src_0
	moveq #0,%d2	| D.2990
	move.w %d0,%d2	| plot_x, D.2990
	moveq #22,%d1	|,
	cmp.l %d2,%d1	| D.2990,
	jge .L44	|
	moveq #64,%d7	|, dma_len_0
	sub.w %d0,%d7	| plot_x, dma_len_0
	moveq #0,%d1	| D.2990
	move.w %d3,%d1	| plot_y, D.2990
	lsl.l #6,%d1	|, D.2990
	add.l %d1,%d2	| D.2990, D.2990
	add.w %d2,%d2	| D.2990, dma_dest_0
	move.w %d0,%d6	| plot_x, dma_len_1
	add.w #-23,%d6	|, dma_len_1
	moveq #0,%d0	| dma_len_0
	move.w %d7,%d0	| dma_len_0, dma_len_0
	add.l %d0,%d0	| dma_len_0, D.2990
	move.l %d0,%a3	| D.2990, dma_src_1
	add.l %d5,%a3	| dma_src_0, dma_src_1
	lsl.w #7,%d3	|, dma_dest_1
	move.w %a0,%d0	| bottom_side,
	jne .L69	|
.L46:
	lea VDP_getAPlanAddress,%a2	|, tmp145
	jsr (%a2)	| tmp145
	move.w map_dma_queue_depth,%d4	| map_dma_queue_depth, map_dma_queue_depth.0
	cmp.w #96,%d4	|, map_dma_queue_depth.0
	jeq .L51	|
	moveq #0,%d1	| D.2990
	move.w %d4,%d1	| map_dma_queue_depth.0, D.2990
	move.l %d1,%a0	| D.2990, tmp130
	add.l %d1,%a0	| D.2990, tmp130
	move.l %a0,%a1	| tmp130, tmp131
	add.l #map_dma_src_queue,%a1	|, tmp131
	move.l %d5,(%a1,%a0.l)	| dma_src_0, map_dma_src_queue
	lea map_dma_dest_queue,%a1	|, tmp132
	add.w %d2,%d0	| dma_dest_0, D.2987
	move.w %d0,(%a0,%a1.l)	| D.2987, map_dma_dest_queue
	lea map_dma_len_queue,%a1	|, tmp134
	move.w %d7,(%a0,%a1.l)	| dma_len_0, map_dma_len_queue
	addq.w #1,%d4	|, map_dma_queue_depth.0
	move.w %d4,map_dma_queue_depth	| map_dma_queue_depth.0, map_dma_queue_depth
.L51:
	tst.w %d6	| dma_len_1
	jne .L70	|
.L43:
	movem.l (%sp)+,#7420	|,
	rts
.L44:
	and.l #65535,%d3	|, D.2990
	lsl.l #6,%d3	|, D.2990
	add.l %d3,%d2	| D.2990, D.2990
	add.w %d2,%d2	| D.2990, dma_dest_0
	sub.l %a3,%a3	| dma_src_1
	clr.w %d3	| dma_dest_1
	clr.w %d6	| dma_len_1
	moveq #41,%d7	|, dma_len_0
	move.w %a0,%d0	| bottom_side,
	jeq .L46	|
	jra .L69	|
.L70:
	jsr (%a2)	| tmp145
	add.w %d3,%d0	| dma_dest_1, D.2987
	move.w map_dma_queue_depth,%d1	| map_dma_queue_depth, map_dma_queue_depth.0
	cmp.w #96,%d1	|, map_dma_queue_depth.0
	jeq .L43	|
	moveq #0,%d2	| D.2990
	move.w %d1,%d2	| map_dma_queue_depth.0, D.2990
	move.l %d2,%a0	| D.2990, tmp138
	add.l %d2,%a0	| D.2990, tmp138
	move.l %a0,%a1	| tmp138, tmp139
	add.l #map_dma_src_queue,%a1	|, tmp139
	move.l %a3,(%a1,%a0.l)	| dma_src_1, map_dma_src_queue
	lea map_dma_dest_queue,%a1	|, tmp140
	move.w %d0,(%a0,%a1.l)	| D.2987, map_dma_dest_queue
	lea map_dma_len_queue,%a1	|, tmp142
	move.w %d6,(%a0,%a1.l)	| dma_len_1, map_dma_len_queue
	addq.w #1,%d1	|, map_dma_queue_depth.0
	move.w %d1,map_dma_queue_depth	| map_dma_queue_depth.0, map_dma_queue_depth
	movem.l (%sp)+,#7420	|,
	rts
.L69:
	move.w %d4,%d1	| map_width,
	mulu.w #28,%d1	|,
	move.l %d1,%a2	|, D.2989
	add.l %d1,%d5	| D.2989, dma_src_0
	lea VDP_getScreenHeight,%a4	|, tmp144
	jsr (%a4)	| tmp144
	cmp.w #240,%d0	|,
	jeq .L47	|
	add.w #3584,%d2	|, dma_dest_0
.L48:
	cmp.w #4095,%d2	|, dma_dest_0
	jls .L49	|
	and.w #4095,%d2	|, dma_dest_0
.L49:
	tst.w %d6	| dma_len_1
	jeq .L46	|
	jsr (%a4)	| tmp144
	cmp.w #240,%d0	|,
	jeq .L71	|
.L50:
	add.l %a2,%a3	| D.2989, dma_src_1
	move.w %d3,%d0	| dma_dest_1, dma_dest_1
	add.w #3584,%d0	|, dma_dest_1
	cmp.w #4095,%d0	|, dma_dest_1
	jls .L55	|
	add.w #-512,%d3	|, dma_dest_1
	jra .L46	|
.L47:
	moveq #0,%d0	| map_width
	move.w %d4,%d0	| map_width, map_width
	add.l %d0,%d0	| map_width, D.2990
	add.l %d0,%d5	| D.2990, dma_src_0
	add.w #3840,%d2	|, dma_dest_0
	jra .L48	|
.L71:
	and.l #65535,%d4	|, map_width
	add.l %d4,%d4	| map_width, D.2990
	add.l %d4,%a3	| D.2990, dma_src_1
	add.w #256,%d3	|, dma_dest_1
	jra .L50	|
.L55:
	move.w %d0,%d3	| dma_dest_1, dma_dest_1
	jra .L46	|
	.size	map_draw_vertical, .-map_draw_vertical
	.align	2
	.globl	map_draw_full
	.type	map_draw_full, @function
map_draw_full:
	subq.l #8,%sp	|,
	movem.l #16190,-(%sp)	|,
	move.l 56(%sp),%d5	| cam_x, cam_x
	move.l 60(%sp),%d6	| cam_y, cam_y
	move.l state,%a0	| state.current_room, state.current_room
	clr.w %d4	| map_width
	move.b 35(%a0),%d4	| _5->w, map_width
	muls.w #80,%d4	|, map_width
	move.w %d5,%d0	|, tmp93
	lsr.w #3,%d0	|, tmp93
	add.w %d0,%d0	| tmp93, tmp93
	move.w %d6,%d1	|, src_ycomp
	lsr.w #3,%d1	|, src_ycomp
	muls.w %d4,%d1	| map_width, src_ycomp
	and.w #511,%d5	|, plot_x
	lsr.w #3,%d5	|, plot_x
	and.w #255,%d6	|, plot_y
	lsr.w #3,%d6	|, plot_y
	moveq #0,%d2	| D.3007
	move.w %d5,%d2	| plot_x, D.3007
	moveq #22,%d3	|,
	cmp.l %d2,%d3	| D.3007,
	jge .L74	|
	move.b #64,%d3	|,
	sub.w %d5,%d3	| plot_x,
	move.w %d3,50(%sp)	|, %sfp
	moveq #0,%d3	| D.3009
	move.w %d1,%d3	| src_ycomp, D.3009
	add.l state+4,%d3	| state.current_map, D.3009
	and.l #65535,%d0	|, D.3009
	add.l %d0,%d3	| D.3009, dma_src$0
	moveq #0,%d0	| D.3007
	move.w %d6,%d0	| plot_y, D.3007
	lsl.l #6,%d0	|, D.3007
	move.l %d2,%a1	| D.3007, D.3007
	add.l %d0,%a1	| D.3007, D.3007
	move.w %a1,%d7	|, dma_dest$0
	add.w %a1,%d7	| D.3007, dma_dest$0
	add.w #-23,%d5	|, dma_len$1
	moveq #0,%d0	| dma_len$0
	move.w 50(%sp),%d0	| %sfp, dma_len$0
	add.l %d0,%d0	| dma_len$0, D.3007
	add.l %d3,%d0	| dma_src$0, D.3007
	move.l %d0,46(%sp)	| D.3007, %sfp
	lsl.w #7,%d6	|, dma_dest$1
	and.l #65535,%d4	|, D.3009
	moveq #30,%d2	|, D.3010
	lea VDP_getAPlanAddress,%a2	|, tmp133
	lea map_dma_src_queue,%a5	|, tmp134
	lea map_dma_dest_queue,%a4	|, tmp135
	lea map_dma_len_queue,%a3	|, tmp136
.L92:
	move.w %d7,%a6	| dma_dest$0, dma_dest$0
	jsr (%a2)	| tmp133
	move.w map_dma_queue_depth,%d7	| map_dma_queue_depth, map_dma_queue_depth.0
	cmp.w #96,%d7	|, map_dma_queue_depth.0
	jeq .L76	|
	moveq #0,%d1	| D.3007
	move.w %d7,%d1	| map_dma_queue_depth.0, D.3007
	add.l %d1,%d1	| D.3007, tmp119
	move.l %d1,%a1	| tmp119, tmp120
	add.l %d1,%a1	| tmp119, tmp120
	move.l %d3,(%a1,%a5.l)	| ivtmp.85, map_dma_src_queue
	add.w %a6,%d0	| dma_dest$0, D.3006
	move.w %d0,(%a4,%d1.l)	| D.3006, map_dma_dest_queue
	move.w 50(%sp),(%a3,%d1.l)	| %sfp, map_dma_len_queue
	addq.w #1,%d7	|, map_dma_queue_depth.0
	move.w %d7,map_dma_queue_depth	| map_dma_queue_depth.0, map_dma_queue_depth
.L76:
	move.w %a6,%d7	| dma_dest$0, dma_dest$0
	add.w #128,%d7	|, dma_dest$0
	cmp.w #4095,%d7	|, dma_dest$0
	jls .L77	|
	add.w #-4096,%d7	|, dma_dest$0
.L77:
	tst.w %d5	| dma_len$1
	jne .L94	|
	subq.l #1,%d2	|, D.3010
	add.l %d4,%d3	| D.3009, ivtmp.85
	tst.l %d2	| D.3010
	jne .L92	|
.L73:
	movem.l (%sp)+,#31996	|,
	addq.l #8,%sp	|,
	rts
.L94:
	jsr (%a2)	| tmp133
	move.w map_dma_queue_depth,%a0	| map_dma_queue_depth, map_dma_queue_depth.0
	moveq #96,%d1	|,
	cmp.w %a0,%d1	| map_dma_queue_depth.0,
	jeq .L79	|
	moveq #0,%d1	| D.3007
	move.w %a0,%d1	| map_dma_queue_depth.0, D.3007
	add.l %d1,%d1	| D.3007, tmp127
	move.l %d1,%a1	| tmp127, tmp128
	add.l %d1,%a1	| tmp127, tmp128
	move.l 46(%sp),(%a1,%a5.l)	| %sfp, map_dma_src_queue
	add.w %d6,%d0	| dma_dest$1, D.3006
	move.w %d0,(%a4,%d1.l)	| D.3006, map_dma_dest_queue
	move.w %d5,(%a3,%d1.l)	| dma_len$1, map_dma_len_queue
	addq.w #1,%a0	|, map_dma_queue_depth.0
	move.w %a0,map_dma_queue_depth	| map_dma_queue_depth.0, map_dma_queue_depth
.L79:
	add.l %d4,46(%sp)	| D.3009, %sfp
	move.w %d6,%d0	| dma_dest$1, dma_dest$1
	add.w #128,%d0	|, dma_dest$1
	cmp.w #4095,%d0	|, dma_dest$1
	jls .L82	|
	add.w #-3968,%d6	|, dma_dest$1
	subq.l #1,%d2	|, D.3010
	add.l %d4,%d3	| D.3009, ivtmp.85
	tst.l %d2	| D.3010
	jne .L92	|
	jra .L73	|
.L82:
	move.w %d0,%d6	| dma_dest$1, dma_dest$1
	subq.l #1,%d2	|, D.3010
	add.l %d4,%d3	| D.3009, ivtmp.85
	tst.l %d2	| D.3010
	jne .L92	|
	jra .L73	|
.L74:
	moveq #0,%d3	| D.3009
	move.w %d1,%d3	| src_ycomp, D.3009
	add.l state+4,%d3	| state.current_map, D.3009
	and.l #65535,%d0	|, D.3009
	add.l %d0,%d3	| D.3009, dma_src$0
	and.l #65535,%d6	|, D.3007
	lsl.l #6,%d6	|, D.3007
	move.l %d2,%a1	| D.3007, D.3007
	add.l %d6,%a1	| D.3007, D.3007
	move.w %a1,%d7	|, dma_dest$0
	add.w %a1,%d7	| D.3007, dma_dest$0
	clr.l 46(%sp)	| %sfp
	clr.w %d6	| dma_dest$1
	clr.w %d5	| dma_len$1
	move.w #41,50(%sp)	|, %sfp
	and.l #65535,%d4	|, D.3009
	moveq #30,%d2	|, D.3010
	lea VDP_getAPlanAddress,%a2	|, tmp133
	lea map_dma_src_queue,%a5	|, tmp134
	lea map_dma_dest_queue,%a4	|, tmp135
	lea map_dma_len_queue,%a3	|, tmp136
	jra .L92	|
	.size	map_draw_full, .-map_draw_full
	.align	2
	.globl	map_draw_diffs
	.type	map_draw_diffs, @function
map_draw_diffs:
	move.l %d3,-(%sp)	|,
	move.l %d2,-(%sp)	|,
	move.l 12(%sp),%d2	| moved, moved
	move.w 18(%sp),%d3	| dx, dx
	move.w 22(%sp),%d0	| dy, dy
	tst.b state+155	| state.fresh_room
	jne .L107	|
	btst #1,%d2	|, moved
	jne .L108	|
	btst #0,%d2	|, moved
	jne .L109	|
.L96:
	move.l (%sp)+,%d2	|,
	move.l (%sp)+,%d3	|,
	rts
.L109:
	tst.w %d3	| dx
	sgt %d0	| tmp75
	ext.w %d0	| tmp76
	ext.l %d0	| tmp74
	neg.l %d0	| tmp74
	move.l %d0,20(%sp)	| tmp74,
	moveq #0,%d0	| state.cam_y
	move.w state+10,%d0	| state.cam_y, state.cam_y
	move.l %d0,16(%sp)	| state.cam_y,
	move.w state+8,%d0	| state.cam_x, state.cam_x
	move.l %d0,12(%sp)	| state.cam_x,
	move.l (%sp)+,%d2	|,
	move.l (%sp)+,%d3	|,
	jra map_draw_horizontal	|
.L108:
	tst.w %d0	| dy
	sgt %d0	| tmp65
	ext.w %d0	| tmp66
	ext.l %d0	| tmp64
	neg.l %d0	| tmp64
	move.l %d0,-(%sp)	| tmp64,
	moveq #0,%d0	| D.3018
	move.w state+10,%d0	| state.cam_y, D.3018
	move.l %d0,-(%sp)	| D.3018,
	move.w state+8,%d0	| state.cam_x, D.3018
	move.l %d0,-(%sp)	| D.3018,
	jsr map_draw_vertical	|
	lea (12,%sp),%sp	|,
	btst #0,%d2	|, moved
	jeq .L96	|
	jra .L109	|
.L107:
	moveq #0,%d0	| D.3018
	move.w state+10,%d0	| state.cam_y, D.3018
	move.l %d0,-(%sp)	| D.3018,
	move.w state+8,%d0	| state.cam_x, D.3018
	move.l %d0,-(%sp)	| D.3018,
	jsr map_draw_full	|
	clr.b state+155	| state.fresh_room
	addq.l #8,%sp	|,
	move.l (%sp)+,%d2	|,
	move.l (%sp)+,%d3	|,
	rts
	.size	map_draw_diffs, .-map_draw_diffs
	.align	2
	.globl	map_dma
	.type	map_dma, @function
map_dma:
	movem.l #8252,-(%sp)	|,
	tst.w map_dma_queue_depth	| map_dma_queue_depth
	jeq .L115	|
	lea map_dma_len_queue,%a4	|, ivtmp.93
	lea map_dma_dest_queue,%a3	|, ivtmp.99
	lea map_dma_src_queue,%a2	|, ivtmp.104
	moveq #0,%d2	| i
	lea VDP_doDMA,%a5	|, tmp79
.L116:
	moveq #0,%d0	| D.3046
	move.w (%a4)+,%d0	| MEM[base: _44, offset: 0B], D.3046
	move.l %d0,-(%sp)	| D.3046,
	move.w (%a3)+,%d0	| MEM[base: _45, offset: 0B], D.3046
	move.l %d0,-(%sp)	| D.3046,
	move.l (%a2)+,-(%sp)	| MEM[base: _46, offset: 0B],
	clr.l -(%sp)	|
	jsr (%a5)	| tmp79
	addq.l #1,%d2	|, i
	moveq #0,%d0	| D.3044
	move.w map_dma_queue_depth,%d0	| map_dma_queue_depth, D.3044
	lea (16,%sp),%sp	|,
	cmp.l %d0,%d2	| D.3044, i
	jcs .L116	|
.L115:
	clr.w map_dma_queue_depth	| map_dma_queue_depth
	move.w map_dma_h_len,%d0	| map_dma_h_len, D.3045
	jne .L122	|
	move.w map_dma_h_len+2,%d0	| map_dma_h_len, D.3045
	jne .L123	|
.L117:
	clr.w map_dma_h_len	| map_dma_h_len
	movem.l (%sp)+,#15364	|,
	rts
.L123:
	pea 128.w	|
	move.w %d0,-(%sp)	| D.3045,
	clr.w -(%sp)	|
	moveq #0,%d0	| D.3046
	move.w map_dma_h_dest+2,%d0	| map_dma_h_dest, D.3046
	move.l %d0,-(%sp)	| D.3046,
	move.w map_dma_h_len,%d0	| map_dma_h_len, D.3046
	add.l %d0,%d0	| D.3046, tmp74
	add.l #map_dma_h_src_queue,%d0	|, tmp74
	move.l %d0,-(%sp)	| tmp74,
	clr.l -(%sp)	|
	jsr VDP_doDMAEx	|
	clr.w map_dma_h_len+2	| map_dma_h_len
	lea (20,%sp),%sp	|,
	clr.w map_dma_h_len	| map_dma_h_len
	movem.l (%sp)+,#15364	|,
	rts
.L122:
	pea 128.w	|
	move.w %d0,-(%sp)	| D.3045,
	clr.w -(%sp)	|
	moveq #0,%d0	| D.3046
	move.w map_dma_h_dest,%d0	| map_dma_h_dest, D.3046
	move.l %d0,-(%sp)	| D.3046,
	pea map_dma_h_src_queue	|
	clr.l -(%sp)	|
	jsr VDP_doDMAEx	|
	lea (20,%sp),%sp	|,
	move.w map_dma_h_len+2,%d0	| map_dma_h_len, D.3045
	jeq .L117	|
	jra .L123	|
	.size	map_dma, .-map_dma
	.align	2
	.globl	map_collision
	.type	map_collision, @function
map_collision:
	move.l state,%a0	| state.current_room, state.current_room
	clr.w %d0	| check_addr
	move.b 35(%a0),%d0	| _4->w, check_addr
	muls.w #40,%d0	|, check_addr
	move.l 8(%sp),%d1	| py, py
	lsr.w #3,%d1	|, D.3052
	muls.w %d1,%d0	| D.3052, check_addr
	move.l 4(%sp),%d1	| px, px
	lsr.w #3,%d1	|, D.3052
	add.w %d1,%d0	| D.3052, check_addr
	and.l #65535,%d0	|, check_addr
	move.l state+4,%a0	| state.current_map, state.current_map
	add.l %d0,%a0	| check_addr, state.current_map
	move.w (%a0,%d0.l),%d0	| *m_16,
	and.w #-128,%d0	|,
	rts
	.size	map_collision, .-map_collision
	.section	.rodata
	.align	2
	.type	maplist, @object
	.size	maplist, 20
maplist:
	.long	mapdata_roomzero
	.long	mapdata_startroom
	.long	mapdata_sidesquare
	.long	mapdata_talltest
	.long	0
	.local	map_dma_h_len
	.comm	map_dma_h_len,4,2
	.local	map_dma_h_dest
	.comm	map_dma_h_dest,4,2
	.local	map_dma_h_src_queue
	.comm	map_dma_h_src_queue,96,2
	.local	map_dma_queue_depth
	.comm	map_dma_queue_depth,2,2
	.local	map_dma_len_queue
	.comm	map_dma_len_queue,192,2
	.local	map_dma_dest_queue
	.comm	map_dma_dest_queue,192,2
	.local	map_dma_src_queue
	.comm	map_dma_src_queue,384,2
	.ident	"GCC: (GNU) 4.8.2"
