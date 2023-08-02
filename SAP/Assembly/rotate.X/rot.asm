.cseg
.org 0x1000
.include "printlib.inc"

.org 0
    jmp start
 
; Set length of message at the start label when changing the text
message: .db "LONDON BRIDGE IS FALLING DOWN",0

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
    
start:
    ; Important constants initialization
    ldi r18, 31 ; starting index
    ldi r23, 48 ; forward for line 2
    
    call init_disp
    call reset

    ldi r19, 29 ; Length of the message! Update each time the message is changed
    
    ldi r20, 1
    sub r20, r19
    sub r20, r19
    ldi r21, 1 
    sub r21, r19
    
    call display
    
end: jmp end