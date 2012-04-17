data segment
	b_x dw 0
	b_y dw 0
	tt1 dw 0
	tt2 dw 0

	line macro x1, y1, x2, y2
	mov l_x1, x1
	mov l_x2, x2
	mov l_y1, y1
	mov l_y2, y2
	call draw_line_
	endm

push1_reg macro
push ax
push bx
push cx
push dx
endm

pop1_reg macro
pop dx
pop cx
pop bx
pop ax
endm 


	draw_pixel macro row, col, color
	push1_reg
	mov al, color
	mov cx, col
	mov dx, row
	mov ah, 0ch
	int 10h
	pop1_reg
	endm

data ends

.stack 200h
.model small

code segment
	assume cs:code, ds:data
	mov ax, data
	mov ds, ax

	; set video mode
	mov al,13h
	mov ah,0
	int 10h

	;call clrscr
	mov color, 4fh
	call welcome_screen

	; Set bike's x and y coordinates
	;mov b_x, 260
	;mov b_y, 100

	; mov color, 
	;call draw_bike

    mov ah, 1h
    int 21h
    
	mov color, 4fh
	call end_screen

	mov ah,01h
	int 21h

	mov ax, 4c00h
	int 21h

end_screen proc
push1_reg
call clrscr
	;G
	line 0, 70, 30, 70
	line 0, 71, 30, 71
	line 0, 72, 30, 72
	line 0, 73, 30, 73
	line 0, 70, 0, 110
	line 1, 70, 1, 110
	line 2, 70, 2, 110
	line 3, 70, 3, 110
	line 0, 110, 31, 110
	line 0, 109, 30, 109
	line 0, 108, 30, 108
	line 0, 107, 30, 107
	line 30, 110, 30, 90
	line 29, 110, 29, 90
	line 28, 110, 28, 90
	line 27, 110, 27, 90
	line 30, 90, 15, 90
	line 30, 89, 15, 89
	line 31, 88, 15, 88
	line 30, 91, 15, 91

	;A
	line 32,110,47,70 
	line 33,110,47,71
	line 34,110,47,72
	line 35,110,47,73
	line 62,110,47,70 
	line 61,110,47,71
	line 60,110,47,72
	line 59,110,47,73
	line 40, 90, 55, 90

	;M
	line 64, 70, 64, 110
	line 65, 70, 65, 110
	line 66, 70, 66, 110
	line 67, 70, 67, 110
	line 67, 70, 77, 90
	line 67, 69, 78, 90
	line 67, 71, 76, 90
	line 77, 90, 89, 70
	line 76, 90, 89, 69
	line 78, 90, 89, 71
	line 89, 70, 89, 110
	line 90, 70, 90, 110
	line 91, 70, 91, 110
	line 92, 70, 92, 110

	;E
	line 94, 70, 94, 110
	line 95, 70, 95, 110
	line 96, 70, 96, 110
	line 94, 70, 115, 70
	line 94, 71, 115, 71
	line 94, 72, 115, 72
	line 94, 89, 110, 89
	line 94, 90, 110, 90
	line 94, 91, 110, 91
	line 94, 108, 115, 108
	line 94, 109, 115, 109
	line 94, 110, 115, 110

	;O
	line 90, 120, 90, 160
	line 88, 120, 88, 160
	line 89, 120, 89, 160
	line 88, 120, 109, 120
	line 88, 121, 109, 121
	line 88, 122, 109, 122
	line 109, 120, 109, 161
	line 108, 120, 108, 160
	line 107, 120, 107, 160
	line 88, 160, 109, 160
	line 88, 159, 109, 159
	line 88, 158, 109, 158

	;V
	line 111, 120, 125, 160
	line 112, 120, 126, 160
	line 111, 121, 125, 160
	line 125, 160, 139, 120
	line 126, 160, 139, 121
	line 124, 160, 138, 120

	;E
	line 142, 120, 142, 160
	line 143, 120, 143, 160
	line 144, 120, 144, 160
	line 142, 120, 163, 120
	line 142, 121, 163, 121
	line 142, 122, 163, 122
	line 142, 139, 158, 139
	line 142, 140, 158, 140
	line 142, 141, 158, 141
	line 142, 158, 163, 158
	line 142, 159, 163, 159
	line 142, 160, 163, 160

	;R
	line 165, 120, 165, 160
	line 166, 120, 166, 160
	line 167, 120, 167, 160
	line 165, 120, 189, 120
	line 165, 121, 189, 121
	line 165, 122, 189, 122
	line 165, 139, 189, 139
	line 165, 140, 190, 140
	line 165, 141, 191, 141
	line 190, 120, 190, 141
	line 189, 120, 189, 141
	line 188, 120, 188, 141
	line 167, 141, 190, 160
	line 166, 141, 189, 160
	line 165, 141, 188, 160
	
pop1_reg
ret
end_screen endp

welcome_screen proc
push1_reg
	; T
	line 80, 49, 120, 49
	line 80, 50, 120, 50	
	line 80, 51, 120, 51
	line 80, 52, 120, 52
	line 80, 53, 120, 53
	line 98, 54, 98, 90
	line 99, 54, 99, 90
	line 100, 54, 100, 90
	line 101, 54, 101, 90
	line 102, 54, 102, 90

	;R
	line 130, 49, 130, 90
	line 131, 49, 131, 90
	line 132, 49, 132, 90
	line 133, 49, 133, 90
	line 134, 49, 134, 90
	line 135, 49, 155, 49
	line 135, 50, 155, 50
	line 135, 51, 155, 51
	line 135, 52, 155, 52
	line 135, 53, 155, 53
	line 135, 69, 155, 69
	line 135, 70, 155, 70
	line 135, 71, 155, 71
	line 135, 72, 155, 72
	line 135, 73, 155, 73
	line 151, 49, 151, 73
	line 152, 49, 152, 73
	line 153, 49, 153, 73
	line 154, 49, 154, 73
	line 155, 49, 155, 74
	line 136, 73, 155, 90
	line 135, 73, 154, 90
	line 134, 73, 153, 90
	line 134, 74, 152, 90
	line 134, 75, 151, 90

	;O
	line 165, 49, 195, 49
	line 165, 50, 195, 50
	line 165, 51, 195, 51
	line 165, 52, 195, 52
	line 165, 53, 195, 53
	line 165, 90, 195, 90
	line 165, 89, 195, 89
	line 165, 88, 195, 88
	line 165, 87, 195, 87
	line 165, 86, 195, 86
	line 165, 49, 165, 90
	line 166, 49, 166, 90
	line 167, 49, 167, 90
	line 168, 49, 168, 90
	line 169, 49, 169, 90
	line 195, 49, 195, 91
	line 194, 49, 194, 90
	line 193, 49, 193, 90
	line 192, 49, 192, 90
	line 191, 49, 191, 90

	;N
	line 200, 49, 200, 90
	line 201, 49, 201, 90
	line 202, 49, 202, 90
	line 203, 49, 203, 90
	line 204, 49, 204, 90
	line 204, 49, 235, 90
	line 204, 48, 236, 90
	line 204, 50, 234, 90
	line 204, 51, 233, 90
	line 204, 52, 232, 90
	line 232, 49, 232, 90
	line 233, 49, 233, 90
	line 234, 49, 234, 90
	line 235, 49, 235, 90
	line 236, 49, 236, 90

; ; mov_cursor 20, 10
; prompt press_any_key
pop1_reg
ret
welcome_screen endp

draw_bike proc 
push1_reg
	draw_pixel tt1, tt2, 32h
	inc tt1
	dec tt2
	draw_pixel tt1, tt2, 32h
	add tt2, 2
	draw_pixel tt1,tt2,32h
	inc tt1
	inc tt2
	draw_pixel tt1,tt2,32h
	dec tt2
	draw_pixel tt1,tt2,32h
	dec tt2
	draw_pixel tt1,tt2,32h
	dec tt2
	draw_pixel tt1,tt2,32h
	dec tt2
	draw_pixel tt1,tt2,32h
	; handle
	sub tt1, 3
	draw_pixel tt1, tt2, 4fh
	inc tt2
	draw_pixel tt1, tt2, 4fh
	inc tt2
	draw_pixel tt1, tt2, 4fh
	inc tt2
	draw_pixel tt1, tt2, 4fh
	inc tt2
	draw_pixel tt1, tt2, 4fh
	dec tt1
	sub tt2, 2
	; 32h is color of tyre
	draw_pixel tt1,tt2, 32h
	
pop1_reg
ret
draw_bike endp

draw_bike1 proc near
push1_reg
	draw_pixel tt1, tt2, 4fh
	inc tt1
	dec tt2
	draw_pixel tt1, tt2, 4fh
	add tt2, 2
	draw_pixel tt1,tt2,32h
	inc tt1
	inc tt2
	draw_pixel tt1,tt2,32h
	dec tt2
	draw_pixel tt1,tt2,32h
	dec tt2
	draw_pixel tt1,tt2,32h
	dec tt2
	draw_pixel tt1,tt2,32h
	dec tt2
	draw_pixel tt1,tt2,4fh
	; handle
	sub tt1, 3
	add tt2, 3
	draw_pixel tt1, tt2, 4fh
	dec tt1
	inc tt2
	draw_pixel tt1,tt2, 4fh
	inc tt1
	sub tt2, 3
	; 32h is color of tyre
	draw_pixel tt1,tt2, 32h
	
pop1_reg
ret
draw_bike1 endp

draw_bike2 proc near
push1_reg
	draw_pixel tt1, tt2, 4fh
	inc tt1
	dec tt2
	draw_pixel tt1, tt2, 32h
	add tt2, 2
	draw_pixel tt1,tt2,4fh
	inc tt1
	inc tt2
	draw_pixel tt1,tt2,4fh
	dec tt2
	draw_pixel tt1,tt2,32h
	dec tt2
	draw_pixel tt1,tt2,32h
	dec tt2
	draw_pixel tt1,tt2,32h
	dec tt2
	draw_pixel tt1,tt2,32h
	; handle
	sub tt1, 3
	inc tt2
	draw_pixel tt1,tt2, 4fh
	dec tt1
	dec tt2
	draw_pixel tt1,tt2, 4fh
	inc tt1
	add tt2, 3	
	; 32h is color of tyre
	draw_pixel tt1,tt2, 32h

pop1_reg
ret
draw_bike2 endp

code ends
