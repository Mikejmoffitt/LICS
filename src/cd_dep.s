        .data
        .long   0
        .global vblank_vector
vblank_vector:        
		.global gen_lvl2
gen_lvl2:
        movem.l d0/a0,-(sp)
        lea     0xA12000,a0
        move.w  (a0),d0
        ori.w   #0x0100,d0
        move.w  d0,(a0)
        movem.l (sp)+,d0/a0
        rte

        .align  2

| short set_sr(short new_sr);
| set SR, return previous SR
| entry: arg = SR value
| exit:  d0 = previous SR value
        .global set_sr
set_sr:
        moveq   #0,d0
        move.w  sr,d0
        move.l  4(sp),d1
        move.w  d1,sr
        rts

		.global segacd_gen_lvl2
segacd_gen_lvl2:
        lea     0xA12000,a0
        move.w  (a0),d0
        ori.w   #0x0100,d0
        move.w  d0,(a0)
        rte

