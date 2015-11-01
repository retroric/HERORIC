
;
; HERO  graphics assembly routines
;


#define HIRES_ADDRESS $A000


#define XY_POINT_ADDR $82  ; computed HIRES address for setting point at coordinates (X,Y)


;
; Using the TEXT screen adress, and the two
; values defined above, we compute the adress
; of the first byte that will be accessed when
; writing the message
;
#define DISPLAY_ADRESS ($BB80+X_POS+Y_POS*40)

;
; Génération du tableau d'adresses HIRES de début des cellules 6x6
; Il y a 40 cellules de 6 pixels (= 240 colonnes) sur 33 rangées de 6 pixels (= 198 lignes)
; ==> Il faut generer les adresses de  40x33 = 1320 cellules
; ==> Il faut donc 132. x 2 = 2640 octets car chaque adresse fait 2 octets (poids faible + poids fort)

Lstart_gen_hires_6x6cell_addr_table       ; begin generating starting addresses for HIRES lines
	lda #>HIRES_ADDRESS	 		; Poids faible adresse HIRES pour cellule courante (init with HIRES start address Low-order byte = #$00)
	sta TmpHiresAddr
	lda #<HIRES_ADDRESS			; Poids fort adresse HIRES pour cellule courante  ((init with HIRES start address High-order byte = #$A0)
	sta TmpHiresAddr+1
	
	lda #>CellAdrrTabletartAddr
	sta TmpTabHiresCellStartAddr
	lda #<CellAdrrTabletartAddr
	sta TmpTabHiresCellStartAddr+1
	
	ldx #0						; Index pour sauvegarde poids faible et poids fort adresse cellule courante
								; Pour chaque rangée de cellule, X varie de 0 à 79 (40 cellules de 0 à 39 * 2 octets pour l'adresse) 
Lgen_current_hires_cell_addr
	lda TmpHiresAddr
	sta (TmpTabHiresCellStartAddr),x
	inx
	sty CellAsddrTableStartAddr,x
	inx
	clc
	adc #$28			   ; add 40 bytes to compute address of next HIRES line
	bcc Lnext_hires_line   ; if no carry from addition, proceed direct to increment Y line counter
	iny				       ; propagate the carry of the addition to the high byte of the HIRES address in Y
Lnext_hires_line
	iny				; increment HIRES line counter
	bne Lgen_current_hires_line_addr ; cycle 256 times (only 200 times necessary, but this simplifies coding)
	rts


	
;
; The message has to be read from the stack
;
_SimplePrint

	; Initialise message adress using the stack parameter
	; this uses self-modifying code
	; (the $0123 is replaced by message adress)
	ldy #0
	lda (sp),y
	sta read+1
	iny
	lda (sp),y
	sta read+2

	; Initialise display adress
	; this uses self-modifying code
	; (the $0123 is replaced by display adress)
	lda #<DISPLAY_ADRESS
	sta write+1
	lda #>DISPLAY_ADRESS
	sta write+2

	; Start at the first character
	ldx #0
loop_char

	; Read the character, exit if it's a 0
read
	lda $0123,x
	beq end_loop_char

	; Write the character on screen
write
	sta $0123,x

	; Next character, and loop
	inx
	jmp loop_char  

	; Finished !
end_loop_char
	rts

	
	
;
; ==== Routines copied from Kong
; ==== (V:\EMULATION\ORIC\OSDK\OSDK_1_5\sample\assembly\game_4kkong)
;
handle_keyboard
	;//
	;// Handle keyboard

	ldx $208			; $208: pattern ligne/colonne dernière touche pressée [OricANu p. 55]
	cpx #56
	bne key_pressed
	stx last_key_press
	jmp end_keyboard
key_pressed
	cpx	last_key_press
	beq end_keyboard
	stx last_key_press
	jsr HandleKeys

end_keyboard
	rts
	
	
HandleKeys
.(
	ldx #0
loop_scan
	lda KeyboardRouter_ScanCode,x
	beq end_of_scan
	cmp last_key_press
	beq execute_key

	inx
	jmp loop_scan

execute_key
	lda KeyboardRouter_AddrLow,x
	sta pl_dst
	lda KeyboardRouter_AddrHigh,x
	sta ph_dst
	jmp (ptr_dst)

end_of_scan
	rts
.)

	
	
	
;
; Align the content of BSS section to a byte boundary
; (so as not to penalize memory accesses using indexed addressing that must not span page boundaries )
;

_bss_align	.dsb 256-(*&255)

_BssStart_

TmpHiresAddr	.dsb2
TmpTabHiresCellStartAddr .dsb2

HiresLineStartAddrHighTab	.dsb 256		; high byte of the address at the start of every HIRES line
HiresLineStartAddrLowTab	.dsb 256		; low byte  of the address at the start of every HIRES line
HiresCharCellPosCountTab	.dsb 256		; number of character cell positions across the line, for each possible value of X
HiresBitPosTimes16Tab		.dsb 256		; bit position within the byte for each possible value of X, multiplied by 16

PixelBitPositions
	.byt 20
	.byt 10
	.byt 08
	.byt 04
	.byt 02
	.byt 01
	
; == HERO - Hovering, Looking Right ==	ink attribute byte + 2 bytes for 12 pixel x 24 lines (12x24 sprite) 
SpriteHeroHoveringRight
	.BYT #03,#7F,#70
	.BYT #03,#46,#40
	.BYT #03,#46,#40
	.BYT #01,#46,#4F
	.BYT #05,#46,#70
	.BYT #01,#46,#70
	.BYT #01,#46,#7F
	.BYT #01,#46,#4F
	.BYT #06,#5E,#5C
	.BYT #06,#5F,#7F
	.BYT #06,#5F,#4F
	.BYT #06,#5F,#4F
	.BYT #06,#4F,#4C
	.BYT #06,#4F,#43
	.BYT #07,#43,#7C
	.BYT #07,#40,#7C
	.BYT #07,#40,#7C
	.BYT #07,#40,#70
	.BYT #07,#40,#70
	.BYT #07,#43,#70
	.BYT #07,#43,#70
	.BYT #07,#43,#40
	.BYT #07,#43,#40
	.BYT #07,#43,#40


_BssEnd_


