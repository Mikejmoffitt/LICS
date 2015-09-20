	.align 2
	.global pad_read
	.type	pad_read, @function
pad_read:
	move.l	d2,-(sp)
	move.l	8(sp), d0
	cmpi.w	#1, d0	/* Check for pad presence */
	bhi		no_pad
	add.w	d0, d0 /* Double d0 to get offset by 2 for each pad to read */
	addi.l	#0xA10003, d0 /* Add VDP gamepad 0 address */ 
	movea.l	d0, a0 /* Put calculated read address in a0 */
	bsr.b	get_input /* Read one phase */
	move.w	d0, d1
	andi.w	#0x0C00, d0
	bne.b	no_pad
	bsr.b	get_input
	bsr.b	get_input /* Second quick time to get 6-button pad state */
	move.w	d0, d2
	bsr.b	get_input
	andi.w	#0x0F00, d0
	cmpi.w	#0x0F00, d0
	beq.b	common /* Detected 6-button pad */
	move.w	#0x010F, d2 /* Clean out 6-button pad stuff, it was a 3-button */

common:
	lsl.b	#4, d2 /* Move MXYZ from nybble 0 to nybble 1, leave top */
	lsl.w	#4, d2 /* Move MXYZ from nybble 1 into nybble 2 */
	andi.w	#0x303F, d1
	move.b	d1, d2
	lsr.w	#6, d1
	or.w	d1, d2
	eori.w	#0x1FFF, d2
	move.w	d2, d0
	move.l	(sp)+, d2
	rts

no_pad:
	move.w	#0xF00, d0 /* No controller, want to return all zeros */
	move.l	(sp)+, d2
	rts

get_input:
	move.b	#0x00, (a0) /* Clear flag to select first button set */
	nop /* Give A/B selector a little time to switch */
	nop
	move.b	(a0), d0 /* Read from VDP controler port into d0 */
	move.b	#0x40, (a0) /* Write bit 7 to select second set */
	lsl.w	#8, d0 /* Shift prior read up 8 bits to make room */
	move.b	(a0), d0 /* Read next phase into lower byte */
	rts

