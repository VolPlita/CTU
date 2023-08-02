.dseg                        ; switch to data memory 1
.org 0x100                   ; from address 0x100 (do not use addresses 0 - 0x100)

flag: .byte 1                ; reserving space for 1 byte

.cseg                        ; switch to program memory
; subroutines for working with the display
.org 0x1000
.include "printlib.inc"

.org 0
    jmp start
    
message: .db "BOOM! YOU ARE NOW A TERRORIST ",0
    
.org 0x16 ; 2
    jmp interrupt
    
.org 0x100
    
;; USER MANUAL:
 ; 1) RIGHT - increase total time by 1 second
 ; 2) LEFT - decrease total time by 1 second
 ; 3) UP - increase total time by 1 minute
 ; 4) DOWN - decrease total time by 1 minute
 ; 5) SELECT - start countdown
 ; Don't press buttons too fast, it may lead to button being interpreted as different button
    
    

;; INITIALIZATION OF BUTTONS      
init_button:
     push r16
     ; enable the AD converter (set the ADEN bit in memory at address ADCSRA without affecting the other bits) 1
     lds r16, ADCSRA
     ori r16, (1<<ADEN); 2
     sts ADCSRA, r16

     ; set reference voltage (0b01<<REFS0)
     ; set output alignment to the left (1<<ADLAR) 3
     ldi r16, (0b01<<REFS0) | (1<<ADLAR); 4
     sts ADMUX, r16

     pop r16
     ret
    
;; START OF THE CODE HERE     
start:
    call init_disp
    call init_button
    
    ldi r18, 0
    ldi r19, 0
    ldi r20, 0
    ldi r21, 0
    ldi r23, 0xF0
    
    ldi r16, '0'
    ldi r17, 4
    call show_char
    ldi r17, 5
    call show_char
    ldi r17, 7
    call show_char
    ldi r17, 8
    call show_char
    ldi r16, ':'
    ldi r17, 6
    call show_char
    
setuploop:    
    call get
    
    lds r16, ADCH
    andi r16, 0xF0
    cpi r16, 0xF0
    breq checkcond
    jmp aftercond
    checkcond:
	cpi r23, 0x10
	breq up
	cpi r23, 0x30
	breq down
	cpi r23, 0x00
	breq right
	cpi r23, 0x60
	breq left
	cpi r23, 0x90
	breq contloopstart
	cpi r23, 0xF0
	breq setuploop
    aftercond:
	mov r23, r16
	call showtime
	jmp setuploop
    
contloopstart: 
    call init_int
    
    ldi r16, 0
    sts flag, r16
contloop:
    lds r30, flag
    cpi r30, 0  
    breq contloop  
                        
    ldi r30, 0          
    sts flag, r30

    ; event triggered 1x per second 4
    call dectime
    call showtime

    jmp contloop
end: jmp end
;; BUTTON OPTIONS  
right:
    call inctime
    jmp aftercond    
left:
    call dectime
    jmp aftercond
    
up:
    call incrminb
    jmp aftercond
    
down:
    call decrminb
    jmp aftercond
    
select:
    call resettimer
    call showtime
    ldi r16, 'S'
    ldi r17, 0
    call show_char
    ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
showtime:
    ldi r22, '0'
    mov r16, r18
    add r16, r22
    ldi r17, 4
    call show_char
    mov r16, r19
    add r16, r22
    ldi r17, 5
    call show_char
    mov r16, r20
    add r16, r22
    ldi r17, 7
    call show_char
    mov r16, r21
    add r16, r22
    ldi r17, 8
    call show_char
    ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
dectime:
    dec r21
    cpi r21, -1
    breq decrc
    ret
decrc:
    ldi r21, 9
    dec r20
    cpi r20, -1
    breq decrb
    ret
decrb:
    ldi r20, 5
    dec r19
    cpi r19, -1
    breq decra
    ret
decra:
    ldi r19, 9
    dec r18
    cpi r18, -1
    breq boom
    ret
boom:
    ldi r16, ' '
    ldi r17, 4
    call show_char
    ldi r17, 5
    call show_char
    ldi r17, 6
    call show_char
    ldi r17, 7
    call show_char
    ldi r17, 8
    call show_char

    ldi r18, 31 ; starting index
    ldi r23, 48 ; forward for line 2
    
    call reset

    ldi r19, 30 ; Length of the message! Update each time the message is changed
    
    ldi r20, 1
    sub r20, r19
    sub r20, r19
    ldi r21, 1 
    sub r21, r19
    
    call display
    jmp end
decrminb:
    dec r19
    cpi r19, -1
    breq decrmina
    ret
decrmina:
    ldi r19, 9
    dec r18
    cpi r18, -1
    breq resettimer
    ret
    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
inctime:
    inc r21
    cpi r21, 10
    breq incrc
    ret
incrc:
    ldi r21, 0
    inc r20
    cpi r20, 6
    breq incrb
    ret   
incrb:
    ldi r21, 0
    ldi r20, 0
    inc r19
    cpi r19, 10
    breq incra
    ret  
incra:
    ldi r21, 0
    ldi r20, 0
    ldi r19, 0
    inc r18
    cpi r18, 6
    breq resettimer
    ret
incrminb:
    inc r19
    cpi r19, 10
    breq incrmina
    ret
incrmina:
    ldi r19, 0
    inc r18
    cpi r18, 6
    breq resettimer
    ret
    
;; GETTING VALUE FROM BUTTON    
get:    
    lds r16, ADCSRA
    ori r16, (1<<ADSC); 2
    sts ADCSRA, r16
    call transferloop
    ret
    
transferloop:
    lds r16, ADCSRA
    andi r16, (1<<ADSC); 2
    brne transferloop
    ret
      
;;;;;;;;;;;;;;;;;;    
resettimer:
    ldi r18, 0
    ldi r19, 0
    ldi r20, 0
    ldi r21, 0
    ret
    
;; INTIALIZATION OF INTERRUPTS
init_int:
    push r16
    cli

    clr r16
    sts TCNT1H, r16
    sts TCNT1L, r16

    ldi r16, (1<<OCIE1A)
    sts TIMSK1, r16

    ldi r16, (1<<WGM12) | (0b101<<CS10)
    sts TCCR1B, r16

    ldi r16, 61
    sts OCR1AH, r16
    ldi r16, 8
    sts OCR1AL, r16

    clr r16
    out EIMSK, r16

    sei
    pop r16
    ret

interrupt:             
    push r16
    in r16, SREG
    push r16

    ldi r16, 1
    sts flag, r16

    pop r16
    out SREG, r16
    pop r16
    reti
;; ROTATING TEXT
; Delay
delay:
    ldi r26, 3
    wait3:
    ldi r25, 100
    wait2: 
    ldi r24, 255
    wait:  
    Dec r24
    brne wait
    Dec r25
    brne wait2
    Dec r26
    brne wait3
ret
 
reset:
    ldi r30, low(2*message)
    ldi r31, high(2*message)
ret
    
printMessage:
    mov r22, r18
    call printChar
    call reset
ret
    
printChar:
    lpm r16, Z+ ; Increment char to show
    cpi r16, 0
    breq termByte
    jmp loadChar
    
    termByte:
    ldi r16, ' ' ; print one space
    call reset
    
    loadChar: 
    mov r17, r22
    cpi r17, 16
    brge moveLine
    jmp continueMoveLine
    
    moveLine:
    ldi r23, 48
    add r17, r23
    
    continueMoveLine:
    cpi r22, 0
    brge print
    jmp contPrint
    print:
    call show_char
    contPrint:
    inc r22
    cpi r22, 32
    brlt printChar
ret
    
display:
    call printMessage
    dec r18
    cp r18, r20
    breq resetOnEnd
    jmp cont
    resetOnEnd: 
    mov r18, r21
    cont:
    call delay
    call display ; infinite loop

