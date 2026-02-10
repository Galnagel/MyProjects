IDEAL
MODEL small
STACK 100h

DATASEG   
GameOver db 'Game Over','$'
printCount db 'count = 00'   
count dw 0                   
Apple_placement dw 400
Snake_Size dw 0
lastMov dw 0
body dw 100 dup (0), 'p'


CODESEG

Start:
    mov ax, @data
    mov ds, ax  
	
	mov ax,0B800h ;מיקום ES
	mov es,ax
	
	mov di, 1990d; מיקום במסך
		
	
	
main_loop:

; print count    
	call print_Count
; print apple 
	call PrintApple
; mov snake 
	call move
; check Apple 
	call checkApple

; go to main loop
	jmp main_loop
;-----------------------------------------------------	
exit:
    mov ax, 4C00h
    int 21h










; print apple	
proc PrintApple
    ; save di 
	push di

	mov di,[ds : offset Apple_placement]  
	
	mov bl , "o" ; 
	mov bh, 00000100b ; צבע 
	mov [es: di ], bx ; 
	
@@end: 	
	pop di
	ret 
endp PrintApple

proc move
;------------------------------------
; start location
	
	mov bl , 2 ; 
	mov bh, 00000010b ; צבע 
	mov [es: di ], bx ; 
	
	call Wait_for_char
	
	mov ah, 01h
    int 16h
    jz  NoKey

    mov ah, 00h
    int 16h
    or al, 20h
	
	; out if enter p 
    cmp al, 'p'
    jne @@Up
    jmp @@p_enter
	

NoKey:
    call movBody
	
	cmp [lastMov], -160
	je if_e
	
	cmp [lastMov], 160
	jne else_ne
if_e:
     call Wait_U_D
else_ne:
	
    add di, [offset lastMov]
	jmp @@end_Input




;------------------------------------
@@Up:
	cmp al, 'w'
	jne @@Down
	call movBody
	sub di, 160
	mov [lastMov], -160
	jmp @@end_Input
	
@@Down:
	cmp al, 's'
	jne @@Left
	call movBody
	add di, 160
	mov [lastMov], 160
	jmp @@end_Input
	
@@Left:
	cmp al, 'a'
	jne @@Right
	call movBody
	sub di, 2
	mov [lastMov], -2
	jmp @@end_Input
	
@@Right:
	cmp al, 'd'
	jne @@end_Input
	call movBody
	add di, 2
	mov [lastMov], 2
	jmp @@end_Input

	
@@end_Input:
	call checkOut1
	call checkOut2
	mov cx, di
	xor di, di

call PrintBoard
	
@@print:
    ;call printBody
	mov di, cx
	mov bl , 2 ; 
	mov bh, 00000010b ; צבע 
	mov [es: di ], bx ;
	
	call printBody

	
	
@@exit:
	ret
	
@@p_enter:
    mov ax, 4C00h
    int 21h
    
endp move

proc print_Count

    push bx
    push cx
    push si
    push di
    
    mov cx, 10
    mov si, 0   
	
	mov di , 170
@@loop:
    mov al, [ds:printCount + si]  ; טען תו
    mov ah, 00000010b             ; צבע
    mov [es:di], ax               ; כתוב למסך
    add di, 2                     ; התקדם
    inc si                        ; הגדל אינדקס
    loop @@loop
    
    pop di
    pop si
    pop cx
    pop bx
    ret 
endp print_Count

proc RANDGEN
    push ax
    push cx
    push dx

    mov ah, 0
    int 1Ah
    mov ax, dx
    
    mov cx, 3800
    xor dx, dx
    div cx
    and dx, 0FFFEh
	
    mov [Apple_placement], dx
    
    pop dx
    pop cx
    pop ax
    ret
endp RANDGEN

proc inc_count
    push ax
    push dx
    push cx
    
    inc [word ptr count]
    mov ax, [count]
    
    ; בדוק אם עבר 99
    cmp ax, 100
    jb @@ok
	jmp @@exit
	
@@ok:
    ; המר לשתי ספרות
    xor dx, dx
    mov cx, 10
    div cx              ; ax = עשרות, dx = אחדות
    
    add al, '0'
    add dl, '0'
    mov [printCount + 8], al    ; ספרת עשרות (מיקום 8)
    mov [printCount + 9], dl    ; ספרת אחדות (מיקום 9)
    
    pop cx
    pop dx
    pop ax
    ret
	
@@exit:
    mov ax, 4C00h
    int 21h
    
endp inc_count

proc Wait_for_char

@@delay:
    push ax
    push dx

    mov ah, 00h
    int 1Ah        ; 

    add dx, 2   ; זמן לחכות 
    mov bx, dx

@@wait:
    mov ah, 00h
    int 1Ah
    cmp dx, bx
    jb @@wait

    pop dx
    pop ax
    ret
endp Wait_for_char

proc inc_body

push bx
mov bx, offset body

@@loop:

    cmp [word bx], 0

    je @@inc
	
	cmp [word bx], 'p'
	
	je @@exit
	
	add bx , 2

jmp @@loop


@@inc:
  mov [word bx], di

@@end:
	pop bx 
    ret
	
@@exit:
    mov ax, 4C00h
    int 21h

 
endp inc_body

proc checkApple

	mov ax, [Apple_placement]  
	cmp di, ax     
; apple not found 	
	jne @@exit
; found
@@appleLocation:
    push bx 
	push dx 
	xor dx, dx 
	
	mov bx, 180
	call RANDGEN   ; change apple location  
	div bx  
    cmp dx, 0	
	
	jne @@else
	
jmp @@appleLocation

@@else:
    pop dx
	pop bx
	call PrintApple   ; show apple 
@@body:
	call inc_count ; change count 
	call inc_body
	call printBody
@@exit:	
	ret
	
endp checkApple

proc PrintBoard

@@clean:
	cmp di, 4000
	jae @@top	
	mov [byte es: di ],''
	add di, 2
	jmp @@clean

@@top:
    xor di,di
@@print:	
	cmp di, 160
	jae @@left	
	mov [byte es: di ],'_'
	mov [byte es: di + 3840 ],'_'
	add di, 2
	jmp @@print
	
@@left:
    xor di,di
	add di, 160
@@printL:	
	cmp di, 4000
	jae @@exit	
	mov [byte es: di ],'|'
	mov [byte es: di + 158],'|'
	add di, 160
	jmp @@printL
 
@@exit:
    ret

endp PrintBoard

proc checkOut1
push bx

mov bx ,  di

cmp bx, 3958
ja @@exit 

pop bx
ret 

@@exit:
    call clean
	
    pop bx
    mov dx, offset GameOver
    mov ah, 9
    int 21h
    
	mov ax, 4C00h
    int 21h
endp checkOut1

proc checkOut2
push bx
push ax 
push dx

xor dx, dx

mov ax ,  di
mov bx, 160
div bx

cmp dx, 0
je @@exit     

cmp dx, 158
je @@exit     

pop dx
pop ax
pop bx
ret 

@@exit:
    call clean
	
    pop dx
    pop ax
    pop bx
    mov dx, offset GameOver
    mov ah, 9
    int 21h
    
	mov ax, 4C00h
    int 21h
endp checkOut2

proc clean
xor di, di

@@clean:
cmp di, 4000
jae @@exit	
mov [byte es: di ],''
add di, 2
jmp @@clean

@@exit:
   ret

endp clean

proc printBody
push di    
push si

mov si, offset body

@@BodyLoop:
    
	cmp [word si], 0
	
	je @@exit 
	
	cmp [word si], 'p'
	
	je @@exit 
	
    mov di, [word si]
	mov bl , 2 ; 
	mov bh, 00000010b ; צבע 
	mov [es: di ], bx ; 
	
	add si, 2
	
jmp @@BodyLoop

@@exit:
    pop si
	pop di
    ret
endp printBody

proc movBody
push si
push bx 
push dx 

mov si, offset body


@@Loop:
    
	cmp [word si], 0
	
	je @@BodyLoop
	
	add si, 2
	
jmp @@Loop


@@BodyLoop:

    sub si, 2
	mov bx, di
	
	
@@BodyLoopStart:
    
	cmp si, offset body
	
	jl @@exit

	mov dx, [word si]
	
	mov [word si], bx
		
	mov bx, dx
	sub si, 2

jmp @@BodyLoopStart



@@exit:

    pop dx
    pop bx
    pop si
    ret



endp movBody

proc Wait_U_D

@@delay:
    push ax
    push dx

    mov ah, 00h
    int 1Ah        ; 

    add dx, 2; זמן לחכות 
    mov bx, dx

@@wait:
    mov ah, 00h
    int 1Ah
    cmp dx, bx
    jb @@wait

    pop dx
    pop ax
    ret
endp Wait_U_D
END start
