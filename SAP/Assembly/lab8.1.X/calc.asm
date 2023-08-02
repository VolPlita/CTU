; Program start - after reset
.org 0x1000 
.include "printlib.inc"

.org 0
    jmp start
; Start of program - main program
.org 0x100
start:
    call init_disp
    ldi R16, 5	;5 ;4;4 ;16
    ldi R17, 10 ;10;4;4 ;16
    ldi R18, 58  ;58;4;20;8
    
    ;ldi R19, 3 ; const
    ldi R21, 8 ; const
    
    mov SP, R16
    ; R16 * 4
    lsl R16
    BRVS overflow
    lsl R16
    BRVS overflow
    ; R17 * 3
    mov R19, R17
    lsl R17
    BRVS overflow
    add R17, R19
    BRVS overflow
    ; R16 + R17
    add R16, R17
    BRVS overflow
    ; R16 - R18
;    cp R18, R16
;    BRLT negative
    sub R16, R18
    BRVS overflow
    mov R20, R16
    ; / 8
    asr R20
    BRCS underflow
    asr R20
    BRCS underflow
    asr R20
    BRCS underflow
;    BRCS overflow
;    BRMI overflow
    jmp second
    
overflow:
    ldi R16, 'O'
    ldi R17, 0
    call show_char
    ldi R16, 'V'
    ldi R17, 1
    call show_char
    ldi R16, 'E'
    ldi R17, 2
    call show_char
    ldi R16, 'R'
    ldi R17, 3
    call show_char
    ldi R16, 'F'
    ldi R17, 4
    call show_char
    ldi R16, 'L'
    ldi R17, 5
    call show_char
    ldi R16, 'O'
    ldi R17, 6
    call show_char
    ldi R16, 'W'
    ldi R17, 7
    call show_char
    jmp end
   
carry:
    jmp end
    
negative:
    jmp end

underflow:
    jmp end
    
; Show second char
second:
    mov R23, R20
    andi R23, 0x0F
    mov R16, R23
    cpi R16, 10
    brge letter
    jmp number
    letter:
	ldi R24, 'A'
	ldi R25, 10
	add R16, R24
	sub R16, R25
	ldi R17, 1
	call show_char
	jmp first
    number:
	ldi R24, '0'
	add R16, R24
	ldi R17, 1
	call show_char
	jmp first
	
; Show first char
first:
    lsr R20
    lsr R20
    lsr R20
    lsr R20
    mov R16, R20
    cpi R16, 10
    brge letterf
    jmp numberf
    letterf:
	ldi R24, 'A'
	ldi R25, 10
	add R16, R24
	sub R16, R25
	ldi R17, 0
	call show_char
	jmp end
    numberf:
	ldi R24, '0'
	add R16, R24
	ldi R17, 0
	call show_char
	jmp end

end: jmp end ; Let's stop the program - infinite loop


