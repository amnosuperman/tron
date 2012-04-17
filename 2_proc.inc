;415

code segment
assume cs:code, ds:data

push_reg macro
push ax
push bx
push cx
push dx
endm

pop_reg macro
pop dx
pop cx
pop bx
pop ax
endm 

	line macro x1, y1, x2, y2
	mov l_x1, x1
	mov l_x2, x2
	mov l_y1, y1
	mov l_y2, y2
	call draw_line
	endm

mov_cursor macro row,col                                       ;macro to move cursor 
 
    mov dh, row                                                ;row number
    mov dl, col                                                ;column numnber
    mov bh, 0                                                  ;page number
    mov ah, 2
    int 10h
    
endm                                                           ;end of macro

prompt macro message                                           ;macro to display a message

    push ax
    mov ah , 09h
    lea dx , message                                           ;get effective adress of a variable
    int 21h
    
    pop ax                                                     ;pop the value that was pushed
    
endm                                                           ;end of macro

waste_time proc
push_reg
mov ax, 8000
waste1:
		mov bx, 4000
		waste2:
			dec bx
			cmp bx, 0
			jg waste2
		dec ax
		jg waste1

pop_reg
ret
waste_time endp

waste_less_time proc
push_reg
mov ax, 9000
waste_l1:
		mov bx, 3000
		waste_l2:
			dec bx
			cmp bx, 0
			jg waste_l2
		dec ax
		jg waste_l1

pop_reg
ret
waste_less_time endp

set_wall_corners2 proc
push_reg

	mov ax, dist_x
	mov bx, 2
	mul bx
	mov cx, dist_a
	sub ax, cx

	mov temp_2, ax
	mov cx, dist_a
	mov temp_1, cx
	mov bx, dist_x
	mov temp_3, bx

	mov ax, temp_3
	mov c_pt2_y, ax
	mov c_pt3_y, ax
	add ax, 120
	mov c_pt1_y, ax
	mov c_pt4_y, ax

	; temp3-x, temp1-a, temp2-b
	mov ax, 100
	cmp temp_1, ax
	jge df_a_exceeds1
	mov ax, 100
	sub ax, temp_1
	mov c_pt2_x, ax
	mov dx, 0
	mov bx, ax
	mov ax, -1
	imul temp_1
	idiv temp_3
	mov cx, 120
	imul cx
	add ax, bx 
	mov c_pt1_x, ax
	jmp df_a_done1

	df_a_exceeds1:
	mov temp_1, 100
	mov c_pt1_x, 100
	mov c_pt2_x, 100
	df_a_done1:
	mov ax, 100
	cmp temp_2, ax
	jge df_b_exceeds1
	
	mov ax, 100
	add ax, temp_2
	mov c_pt3_x, ax
	mov bx, ax
	
	mov ax, 120
	mul temp_2
	div temp_3
	add ax, bx
	mov c_pt4_x, ax

	jmp df_b_done1

	df_b_exceeds1:
	mov temp_2, 100
	mov c_pt3_x, 100
	mov c_pt4_x, 100

	df_b_done1:

pop_reg
ret


set_wall_corners2 endp
code ends

