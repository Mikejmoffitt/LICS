;---------------------------------------------------------
;
;       Sega startup code for the Sozobon C compiler
;       Written by Paul W. Lee
;       Modified from Charles Coty's code
;       VBL support added by Haroldo de Oliveira Pinheiro
;
;---------------------------------------------------------

	org    $0

	dc.l $0,$200
	dc.l INT,INT,INT,INT,INT,INT,INT     ;
	dc.l INT,INT,INT,INT,INT,INT,INT,INT ; $24
	dc.l INT,INT,INT,INT,INT,INT,INT,INT ; $44
	dc.l INT,INT,INT,HBL,INT,VBL,INT,INT ; $64
	dc.l INT,INT,INT,INT,INT,INT,INT,INT ; $84
	dc.l INT,INT,INT,INT,INT,INT,INT,INT ; $A4
	dc.l INT,INT,INT,INT,INT,INT,INT,INT ; $C4
	dc.l INT,INT,INT,INT,INT,INT,INT     ;

	dc.b 'SEGA GENESIS (C) 1998 KANEDA    '
	dc.b 'ULTIMATE TETRIS BY HAROLDO O. PINHEIRO          '
	dc.b 'ULTIMATE TETRIS BY HAROLDO O. PINHEIRO          '
	dc.b 'GM 00000005-00',$a5,$fb
	dc.b 'JD              ',$00,$00,$00,$00,$00,$02,$00,$00
	dc.b $00,$ff,$00,$00,$ff,$ff,$ff,$ff,'               '
	dc.b '                        '
	dc.b '                         '
	dc.b 'JUE             '

	tst.l   $a10008
	bne     SkipJoyDetect                               
	tst.w   $a1000c
SkipJoyDetect:
	bne     SkipSetup
	lea     Table,a5                       
	movem.w (a5)+,d5-d7
	movem.l (a5)+,a0-a4                       
	move.b  -$10ff(a1),d0          ;Check Version Number                      
	andi.b  #$0f,d0                             
	beq     WrongVersion                                   
	move.l  #$53454741,$2f00(a1)   ;Sega Security Code (SEGA)   
WrongVersion:
	move.w  (a4),d0
	moveq   #$00,d0                                
	movea.l d0,a6                                  
	move    a6,usp                                
	moveq   #$17,d1                ; Set VDP registers
FillLoop:                           
	move.b  (a5)+,d5
	move.w  d5,(a4)                              
	add.w   d7,d5                                 
	dbra    d1,FillLoop                           
	move.l  (a5)+,(a4)                            
	move.w  d0,(a3)                                 
	move.w  d7,(a1)                                 
	move.w  d7,(a2)                                 
L0250:
	btst    d0,(a1)
	bne     L0250                                   
	moveq   #$25,d2                ; Put initial vaules into a00000                
Filla:                                 
	move.b  (a5)+,(a0)+
	dbra    d2,Filla
	move.w  d0,(a2)                                 
	move.w  d0,(a1)                                 
	move.w  d7,(a2)                                 
L0262:
	move.l  d0,-(a6)
	dbra    d6,L0262                            
	move.l  (a5)+,(a4)                              
	move.l  (a5)+,(a4)                              
	moveq   #$1f,d3                ; Put initial values into c00000                  
Filc0:                             
	move.l  d0,(a3)
	dbra    d3,Filc0
	move.l  (a5)+,(a4)                              
	moveq   #$13,d4                ; Put initial values into c00000                 
Fillc1:                            
	move.l  d0,(a3)
	dbra    d4,Fillc1
	moveq   #$03,d5                ; Put initial values into c00011                 
Fillc2:                            
	move.b  (a5)+,$0011(a3)        
	dbra    d5,Fillc2                            
	move.w  d0,(a2)                                 
	movem.l (a6),d0-d7/a0-a6                    
	move    #$2700,sr                           
SkipSetup:
	bra     Continue
Table:
	dc.w    $8000, $3fff, $0100, $00a0, $0000, $00a1, $1100, $00a1
	dc.w    $1200, $00c0, $0000, $00c0, $0004, $0414, $302c, $0754
	dc.w    $0000, $0000, $0000, $812b, $0001, $0100, $00ff, $ff00                                   
	dc.w    $0080, $4000, $0080, $af01, $d91f, $1127, $0021, $2600
	dc.w    $f977, $edb0, $dde1, $fde1, $ed47, $ed4f, $d1e1, $f108                                   
	dc.w    $d9c1, $d1e1, $f1f9, $f3ed, $5636, $e9e9, $8104, $8f01                
	dc.w    $c000, $0000, $4000, $0010, $9fbf, $dfff                                

Continue:
	tst.w    $00C00004

	clr.l   a7              ; set stack pointer

	move.w  #$2300,sr       ; user mode

	lea     $ff0000,a0      ; clear Genesis RAM
	moveq   #0,d0
clrram: move.w  #0,(a0)+
	subq.w  #2,d0
	bne     clrram

;----------------------------------------------------------        
;
;       Load driver into the Z80 memory
;
;----------------------------------------------------------        

	move.w  #$100,$a11100     ; halt the Z80
	move.w  #$100,$a11200     ; reset it

	lea     Z80Driver,a0
	lea     $a00000,a1
	move.l  #Z80DriverEnd,d0
	move.l  #Z80Driver,d1
	sub.l   d1,d0
Z80loop:
	move.b  (a0)+,(a1)+
	subq.w  #1,d0
	bne     Z80loop

	move.w  #$0,$a11100       ; enable the Z80

;----------------------------------------------------------        
;
;       Init PSG Driver
;
;----------------------------------------------------------        

	bsr		_init_PSG
	bsr		_init_interr

;----------------------------------------------------------        

	jmp      _main

INT:    
	rte

; --- Do nothing for this demo ---
HBL:
	rte

VBL:
	addq.l   #1,_vtimer
	movem.l  d0-d7/a0-a7, -(a7)
	bsr		 _generate_sound
	bsr		 _poll_interr
	movem.l  (a7)+, d0-d7/a0-a7
	rte

;----------------------------------------------------------        
;
;       Z80 Sound Driver
;
;----------------------------------------------------------        
Z80Driver:
	  dc.b  $c3,$46,$00,$00,$00,$00,$00,$00
	  dc.b  $00,$00,$00,$00,$00,$00,$00,$00
	  dc.b  $00,$00,$00,$00,$00,$00,$00,$00
	  dc.b  $00,$00,$00,$00,$00,$00,$00,$00
	  dc.b  $00,$00,$00,$00,$00,$00,$00,$00
	  dc.b  $00,$00,$00,$00,$00,$00,$00,$00
	  dc.b  $00,$00,$00,$00,$00,$00,$00,$00
	  dc.b  $00,$00,$00,$00,$00,$00,$00,$00
	  dc.b  $00,$00,$00,$00,$00,$00,$f3,$ed
	  dc.b  $56,$31,$00,$20,$3a,$39,$00,$b7
	  dc.b  $ca,$4c,$00,$21,$3a,$00,$11,$40
	  dc.b  $00,$01,$06,$00,$ed,$b0,$3e,$00
	  dc.b  $32,$39,$00,$3e,$b4,$32,$02,$40
	  dc.b  $3e,$c0,$32,$03,$40,$3e,$2b,$32
	  dc.b  $00,$40,$3e,$80,$32,$01,$40,$3a
	  dc.b  $43,$00,$4f,$3a,$44,$00,$47,$3e
	  dc.b  $06,$3d,$c2,$81,$00,$21,$00,$60
	  dc.b  $3a,$41,$00,$07,$77,$3a,$42,$00
	  dc.b  $77,$0f,$77,$0f,$77,$0f,$77,$0f
	  dc.b  $77,$0f,$77,$0f,$77,$0f,$77,$3a
	  dc.b  $40,$00,$6f,$3a,$41,$00,$f6,$80
	  dc.b  $67,$3e,$2a,$32,$00,$40,$7e,$32
	  dc.b  $01,$40,$21,$40,$00,$7e,$c6,$01
	  dc.b  $77,$23,$7e,$ce,$00,$77,$23,$7e
	  dc.b  $ce,$00,$77,$3a,$39,$00,$b7,$c2
	  dc.b  $4c,$00,$0b,$78,$b1,$c2,$7f,$00
	  dc.b  $3a,$45,$00,$b7,$ca,$4c,$00,$3d
	  dc.b  $3a,$45,$00,$06,$ff,$0e,$ff,$c3
	  dc.b  $7f,$00
Z80DriverEnd:
