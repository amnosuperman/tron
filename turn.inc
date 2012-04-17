data segment

	c_pt1_x dw 0
	c_pt2_x dw 0
	c_pt3_x dw 0
	c_pt4_x dw 0
	c_pt1_y dw 0
	c_pt2_y dw 0
	c_pt3_y dw 0
	c_pt4_y dw 0
	
	n_pt1_x dw 0
	n_pt2_x dw 0
	n_pt3_x dw 0
	n_pt4_x dw 0
	n_pt1_y dw 0
	n_pt2_y dw 0
	n_pt3_y dw 0
	n_pt4_y dw 0
	
	inc_pt1_x dw 0
	inc_pt2_x dw 0
	inc_pt3_x dw 0
	inc_pt4_x dw 0
	inc_pt1_y dw 0
	inc_pt2_y dw 0
	inc_pt3_y dw 0
	inc_pt4_y dw 0
	
	el_x1 dw 0
	el_x2 dw 0
	el_y1 dw 0
	el_y2 dw 0
	
	varx dw 0
	vary dw 0
	varx1 dw 0
	vary1 dw 0
	
	direction db 0
	dist_b dw 0
	t_dist_a dw 0
	t_dist_b dw 0
	t_dist_x dw 0
	
	neg_a dw 0
	neg_b dw 0
	should_draw db 0
	
	is_same db 0
	
data ends

code segment
assume cs:code, ds:data

	check_same proc
		push_reg
			mov is_same, 0
			mov ax, l_x1
			cmp ax, l_x2
			jne end_cs
			mov ax, l_y1
			cmp ax, l_y2
			jne end_cs
			mov is_same, 1
			end_cs:			
		pop_reg
	ret
	check_same endp

	set_wall_corners proc
		push_reg
		
		mov ax, dist_x
		mov c_pt2_y, ax
		mov c_pt3_y, ax
		add ax, 120
		mov c_pt1_y, ax
		mov c_pt4_y, ax
		
		mov ax, dist_a
		mov bx, 100
		sub bx, ax
		mov c_pt2_x, bx
		mov ax, dist_x
		mov bx, 2
		mul bx
		sub ax, dist_a
		mov dist_b, ax
		add ax, 100
		mov c_pt3_x, ax
		
		mov ax, -120
		mov dx, 0
		imul dist_a
		neg ax
		mov dx, 0
		idiv dist_x
		neg ax
		add ax, c_pt2_x
		mov c_pt1_x, ax
				
		mov ax, 120
		mov dx, 0
		imul dist_b
		mov dx, 0
		idiv dist_x
		add ax, c_pt3_x
		mov c_pt4_x, ax
		pop_reg			
	ret
	set_wall_corners endp
	
	set_new_wall_corners proc
		push_reg
		
			mov ax, x
			mov bx, 120
			imul bx
			mov bx, 200
			mov dx, 0
			idiv bx
			mov bx, 200
			sub bx, ax
			mov n_pt2_y, bx
			mov n_pt3_y, bx
			mov t_dist_x, bx
			add bx, 120
			mov n_pt1_y, bx
			mov n_pt4_y, bx
			
			mov ax, t_dist_x
			mov bx, 2
			imul bx
			mov bx, a
			imul bx
			mov bx, 200
			mov dx, 0
			idiv bx
			mov t_dist_a, ax
			
			mov bx, 100
			sub bx, ax
			mov n_pt2_x, bx
			mov ax, t_dist_x
			mov bx, 2
			mul bx
			sub ax, t_dist_a
			mov t_dist_b, ax
			add ax, 100
			mov n_pt3_x, ax
			
			mov ax, -120
			mov dx, 0
			imul t_dist_a
			neg ax
			mov dx, 0
			idiv t_dist_x
			neg ax
			add ax, n_pt2_x
			mov n_pt1_x, ax
					
			mov ax, 120
			mov dx, 0
			imul t_dist_b
			mov dx, 0
			idiv t_dist_x
			add ax, n_pt3_x
			mov n_pt4_x, ax
		
		pop_reg	
	ret
	set_new_wall_corners endp
	
	check_neg_a proc
		mov neg_a, 1
		cmp ax, 0
		jge not_neg_a
		mov neg_a, -1
		neg ax
		not_neg_a:
		ret
	check_neg_a endp
	
	recheck_neg_a proc
		cmp neg_a, -1
		jne not_neg_a1
		neg ax
		not_neg_a1:
		ret
	recheck_neg_a endp
	
	check_neg_b proc
		mov neg_b, 1
		cmp bx, 0
		jge not_neg_b
		mov neg_b, -1
		neg bx
		not_neg_b:
		ret
	check_neg_b endp
	
	recheck_neg_b proc
		cmp neg_b, -1
		jne not_neg_b1
		neg ax
		neg bx
		not_neg_b1:
		ret
	recheck_neg_b endp
	
	turn proc
		push_reg
		
		cmp direction, 0
		je turn_d1
		
		call set_new_wall_corners
		
		cmp direction, 1
		je turn_d1
		
		right:
		;c2->n1 c3->n2 c4->n3 c1->n4	
			mov ax, n_pt1_x
			sub ax, c_pt2_x
			mov bx, 20
			mov dx, 0
			call check_neg_a
			idiv bx
			call recheck_neg_a
			mov inc_pt2_x, ax
			
			mov ax, n_pt1_y
			sub ax, c_pt2_y
			mov bx, 20
			mov dx, 0
			call check_neg_a
			idiv bx
			call recheck_neg_a
			mov inc_pt2_y, ax
			
			mov ax, n_pt2_x
			sub ax, c_pt3_x
			mov bx, 20
			mov dx, 0
			call check_neg_a
			idiv bx
			call recheck_neg_a
			mov inc_pt3_x, ax
			
			mov ax, n_pt2_y
			sub ax, c_pt3_y
			call check_neg_a
			mov bx, 20
			mov dx, 0
			idiv bx
			call recheck_neg_a
			mov inc_pt3_y, ax
			
			jmp e_turn_d1
			turn_d1:
			jmp left
			e_turn_d1:
			
			mov ax, n_pt3_x
			sub ax, c_pt4_x
			mov bx, 20
			mov dx, 0
			call check_neg_a
			idiv bx
			call recheck_neg_a
			mov inc_pt4_x, ax
			
			mov ax, n_pt3_y
			sub ax, c_pt4_y
			mov bx, 20
			mov dx, 0
			call check_neg_a
			idiv bx
			call recheck_neg_a
			mov inc_pt4_y, ax
			
			mov ax, n_pt4_x
			sub ax, c_pt1_x
			mov bx, 20
			mov dx, 0
			call check_neg_a
			idiv bx
			call recheck_neg_a
			mov inc_pt1_x, ax
			
			mov ax, n_pt4_y
			sub ax, c_pt1_y
			call check_neg_a
			mov bx, 20
			mov dx, 0
			idiv bx
			call recheck_neg_a
			mov inc_pt1_y, ax
			
			call rotate		
			jmp endturn
		
		left:
		;c2->n3 c3->n4 c4->n1 c1->n2	
			
			cmp direction, 1
			jne turn_d2
			
			mov ax, n_pt3_x
			sub ax, c_pt2_x
			mov bx, 20
			mov dx, 0
			call check_neg_a
			idiv bx
			call recheck_neg_a
			mov inc_pt2_x, ax
			
			mov ax, n_pt3_y
			sub ax, c_pt2_y
			call check_neg_a
			mov bx, 20
			mov dx, 0
			idiv bx
			call recheck_neg_a
			mov inc_pt2_y, ax
			
			mov ax, n_pt4_x
			sub ax, c_pt3_x
			mov bx, 20
			mov dx, 0
			call check_neg_a
			idiv bx
			call recheck_neg_a
			mov inc_pt3_x, ax
			
			mov ax, n_pt4_y
			sub ax, c_pt3_y
			mov bx, 20
			mov dx, 0
			call check_neg_a
			idiv bx
			call recheck_neg_a
			mov inc_pt3_y, ax
			
			jmp e_turn_d2
			turn_d2:
			jmp endturn
			e_turn_d2:
			
			mov ax, n_pt1_x
			sub ax, c_pt4_x
			mov bx, 20
			mov dx, 0
			call check_neg_a
			idiv bx
			call recheck_neg_a
			mov inc_pt4_x, ax
			
			mov ax, n_pt1_y
			sub ax, c_pt4_y
			call check_neg_a
			mov bx, 20
			mov dx, 0
			idiv bx
			call recheck_neg_a
			mov inc_pt4_y, ax
			
			mov ax, n_pt2_x
			sub ax, c_pt1_x
			mov bx, 20
			mov dx, 0
			call check_neg_a
			idiv bx
			call recheck_neg_a
			mov inc_pt1_x, ax
			
			mov ax, n_pt2_y
			sub ax, c_pt1_y
			mov bx, 20
			mov dx, 0
			call check_neg_a
			idiv bx
			call recheck_neg_a
			mov inc_pt1_y, ax
			
			call rotate		
		
		endturn:
		
	pop_reg
	ret
	turn endp

	rotate proc
		push_reg
		
			mov si, 20
			
			rotate_loop:

				call clrscr

				mov tt1, 198
				mov tt2, 100
				cmp direction, 1
				je this_moving_right
				call draw_bike1
				jmp this_moving_done
				this_moving_right:
				call draw_bike2
				this_moving_done:
				mov l_x1, 200
				mov l_x2, 200
				mov l_y1, 0
				mov l_y2, 200
				mov color, 11110111b
				call draw_line_
				mov color, 11110111b

				mov ax, c_pt1_x
				add ax, inc_pt1_x
				mov el_x1, ax
				mov c_pt1_x, ax									

				mov bx, c_pt1_y
				add bx, inc_pt1_y
				mov el_y1, bx									
				mov c_pt1_y, bx									

				mov ax, c_pt2_x
				add ax, inc_pt2_x
				mov el_x2, ax									
				mov c_pt2_x, ax									

				mov bx, c_pt2_y
				add bx, inc_pt2_y
				mov el_y2, bx									
				mov c_pt2_y, bx									

				call extended_line
				
				mov ax, c_pt3_x
				add ax, inc_pt3_x
				mov el_x1, ax									
				mov c_pt3_x, ax									

				mov bx, c_pt3_y
				add bx, inc_pt3_y
				mov el_y1, bx									
				mov c_pt3_y, bx									

				call extended_line
				
				mov ax, c_pt4_x
				add ax, inc_pt4_x
				mov el_x2, ax									
				mov c_pt4_x, ax									

				jmp skipd
				rotate_dummy:
				jmp rotate_loop
				skipd:
				
				mov bx, c_pt4_y
				add bx, inc_pt4_y
				mov el_y2, bx									
				mov c_pt4_y, bx									

				call extended_line
				
				mov ax, c_pt1_x
				mov el_x1, ax									

				mov bx, c_pt1_y
				mov el_y1, bx									

				call extended_line

				call waste_less_time
				dec si
				cmp si, 0
				jne rotate_dummy	

		pop_reg
	ret
	rotate endp

	extended_line proc
		push_reg
		
			mov ax, el_x1
			cmp ax, 0
			jl pt1_out
			cmp ax, 200
			jge pt1_out
			mov bx, el_y1
			cmp bx, 0
			jl pt1_out
			cmp bx, 200
			jge pt1_out
			
			mov l_x1, ax
			mov l_y1, bx
			
			mov ax, el_x2
			cmp ax, 0
			jl pt2_out1
			cmp ax, 200
			jge pt2_out1
			mov bx, el_y2
			cmp bx, 0
			jl pt2_out1
			cmp bx, 200
			jge pt2_out1
			
			mov l_x2, ax
			mov l_y2, bx
			call draw_line_
			jmp end_ex_line
			
			jmp pt1_out
			pt2_out1:
			jmp pt2_out
			
			pt1_out:		
				mov ax, el_x2
				cmp ax, 0
				jl both_out1
				cmp ax, 200
				jge both_out1
				mov bx, el_y2
				cmp bx, 0
				jl both_out1
				cmp bx, 200
				jge both_out1
			
				mov l_x2, ax
				mov l_y2, bx
				
				cmp el_x1, 200
				jge fc1
				mov ax, el_x1
				mov l_x1, ax
				mov ax, el_y1
				mov l_y1, ax
				call draw_line_
				jmp end_ex_line
				
				jmp fc1
				both_out1:
				jmp both_out
				
				fc1:
				call find_coord
				mov ax, varx
				mov l_x1, ax
				mov ax, vary
				mov l_y1, ax
		
				call draw_line_
				jmp end_ex_line
				
				
			pt2_out:
				cmp el_x2, 200
				jge fc2
				mov ax, el_x2
				mov l_x2, ax
				mov ax, el_y2
				mov l_y2, ax
				call draw_line_
				jmp end_ex_line
				
				fc2:
				call find_coord
				mov ax, varx
				mov l_x2, ax
				mov ax, vary
				mov l_y2, ax
		
				call draw_line_
				jmp end_ex_line
				
			both_out:
				
				mov should_draw, 0
				call find_coord
				cmp should_draw, 0
				je end_ex_line
				
				mov ax, varx
				mov l_x1, ax
				mov ax, vary
				mov l_y1, ax
				
				mov ax, varx1
				mov l_x2, ax
				mov ax, vary1
				mov l_y2, ax
	
				call draw_line_
			end_ex_line:
		pop_reg
	ret
	extended_line endp
	
	find_coord proc
		push_reg
		
				cmp el_x1, 0
				jg x1_is_greater_1
				cmp el_x2, 0
				jl nl
				jmp cont_x_1
				x1_is_greater_1:
				cmp el_x2, 0
				jg nl
				cont_x_1:
				mov dx, 0
				mov ax, el_y2
				sub ax, el_y1
				mov bx, el_x1
				mov dx, 0
				imul bx
				call check_neg_a
				mov bx, el_x2
				sub bx, el_x1
				mov dx, 0
				cmp bx, 0
				je nl
				call check_neg_b
				idiv bx
				call recheck_neg_a
				call recheck_neg_b
				mov bx, el_y1
				sub bx, ax
				cmp bx, 0
				jl nl
				cmp bx, 200
				jg nl
				mov vary, bx
				mov varx, 0
				jmp l1
				
				nl:
				cmp el_y1, 200
				jg y1_is_greater_1
				cmp el_y2, 200
				jl nb
				jmp cont_y_1
				y1_is_greater_1:
				cmp el_y2, 200
				jg nb
				cont_y_1:
				mov ax, el_x2
				sub ax, el_x1
				mov bx, 200
				sub bx, el_y1
				mov dx, 0
				imul bx
				call check_neg_a
				mov bx, el_y2
				sub bx, el_y1
				cmp bx, 0
				je nb
				mov dx, 0
				call check_neg_b
				idiv bx
				call recheck_neg_a
				call recheck_neg_b
				mov bx, ax
				add bx, el_x1
				cmp bx, 0
				jl nb
				cmp bx, 200
				jg nb
				mov varx, bx
				mov vary, 200
				jmp b1
				
				nb:
				cmp el_x1, 200
				jg x1_is_greater_2
				cmp el_x2, 200
				jl nr
				jmp cont_x_2
				x1_is_greater_2:
				cmp el_x2, 200
				jg nr
				cont_x_2:
				mov ax, el_y2
				sub ax, el_y1
				mov bx, 200
				sub bx, el_x1
				mov dx, 0
				imul bx
				call check_neg_a
				mov bx, el_x2
				sub bx, el_x1
				cmp bx, 0
				je nr
				mov dx, 0
				call check_neg_b
				idiv bx
				call recheck_neg_a
				call recheck_neg_b
				mov bx, ax
				add bx, el_y1
				cmp bx, 0
				jl nr
				cmp bx, 200
				jg nr
				mov vary, bx
				mov varx, 200
				t1:
				jmp r1
				
				nr:
				cmp el_y1, 0
				jg y1_is_greater_2
				cmp el_y2, 0
				jl end_dum1
				jmp cont_y_2
				y1_is_greater_2:
				cmp el_y2, 0
				jg end_dum1
				cont_y_2:
				mov ax, el_x2
				sub ax, el_x1
				mov bx, el_y1
				mov dx, 0
				imul bx
				call check_neg_a
				mov bx, el_y2
				sub bx, el_y1
				cmp bx, 0
				je end_dum1
				mov dx, 0
				call check_neg_b
				idiv bx
				call recheck_neg_a
				call recheck_neg_b
				mov bx, el_x1
				sub bx, ax
				cmp bx, 0
				jl end_dum1
				cmp bx, 200
				jg end_dum1
				mov varx, bx
				mov vary, 0
				jmp end_dum1
						
				jmp c_flow1
				end_dum1:
				jmp end_dum
				c_flow1:
				

				l1:
				cmp el_y1, 200
				jg y1_is_greater_3
				cmp el_y2, 200
				jl b1
				jmp cont_y_3
				y1_is_greater_3:
				cmp el_y2, 200
				jg b1
				cont_y_3:
				mov ax, el_x2
				sub ax, el_x1
				mov bx, 200
				sub bx, el_y1
				mov dx, 0
				imul bx
				call check_neg_a
				mov bx, el_y2
				sub bx, el_y1
				cmp bx, 0
				je b1
				mov dx, 0
				call check_neg_b
				idiv bx
				call recheck_neg_a
				call recheck_neg_b
				mov bx, ax
				add bx, el_x1
				cmp bx, 0
				jl b1
				cmp bx, 200
				jg b1
				mov varx1, bx
				mov vary1, 200
				mov should_draw, 1
				jmp end_findc
				
				jmp c_flow
				end_dum:
				jmp end_findc
				c_flow:
				
				b1:
				cmp el_x1, 200
				jg x1_is_greater_3
				cmp el_x2, 200
				jl r1
				jmp cont_x_3
				x1_is_greater_3:
				cmp el_x2, 200
				jg r1
				cont_x_3:
				mov ax, el_y2
				sub ax, el_y1
				mov bx, 200
				sub bx, el_x1
				mov dx, 0
				imul bx
				call check_neg_a
				mov bx, el_x2
				sub bx, el_x1
				cmp bx, 0
				je r1
				mov dx, 0
				call check_neg_b
				idiv bx
				call recheck_neg_a
				call recheck_neg_b
				mov bx, ax
				add bx, el_y1
				cmp bx, 0
				jl r1
				cmp bx, 200
				jg r1
				mov vary1, bx
				mov varx1, 200
				mov should_draw, 1
				jmp end_findc
				
				r1:
				cmp el_y1, 0
				jg y1_is_greater_4
				cmp el_y2, 0
				jl end_findc
				jmp cont_y_4
				y1_is_greater_4:
				cmp el_y2, 0
				jg end_findc
				cont_y_4:
				mov ax, el_x2
				sub ax, el_x1
				mov bx, el_y1
				mov dx, 0
				imul bx
				call check_neg_a
				mov bx, el_y2
				sub bx, el_y1
				cmp bx, 0
				je end_findc
				mov dx, 0
				call check_neg_b
				idiv bx
				call recheck_neg_a
				call recheck_neg_b
				mov bx, el_x1
				sub bx, ax
				cmp bx, 0
				jl end_findc
				cmp bx, 200
				jg end_findc
				mov varx1, bx
				mov vary1, 0
				mov should_draw, 1
		
		end_findc:
		pop_reg
	ret
	find_coord endp
	
draw_line_ proc
push_reg

	call check_same
	mov cl, is_same
	cmp cl, 1
	je dl_ended_
	mov ax, l_x1
	cmp l_x2, ax
	jg dl_x2_greater_
	mov ax, l_x1
	sub ax, l_x2
	jmp dl_x_diff_done_
	dl_x2_greater_:
	mov ax, l_x2
	sub ax, l_x1
	dl_x_diff_done_:

	mov bx, l_y1
	cmp l_y2, bx
	jg dl_y2_greater_
	mov bx, l_y1
	sub bx, l_y2
	jmp dl_y_diff_done_
	dl_y2_greater_:
	mov bx, l_y2
	sub bx, l_y1
	dl_y_diff_done_:

	cmp ax, bx
	jge dl_horizontal_
	call draw_vertical_
	
	jmp dl_ended_
	dl_horizontal_:
	call draw_horizontal_

dl_ended_:
pop_reg
ret
draw_line_ endp


draw_vertical_ proc
push_reg
	mov negative, 0
	mov ax, l_y1
	cmp l_y2, ax
	jl dv_y2_lesser_
	mov ax, l_x1
	mov temp1, ax
	mov bx, l_y1
	mov temp2, bx
	mov ax, l_y2
	mov temp4, ax
	sub ax, l_y1
	mov l_y_diff, ax
	mov ax, l_x2
	mov temp3, ax
	sub ax, l_x1
	mov l_x_diff, ax
	cmp ax, 0
	jg dv_temps_set_
	mov ax, 0
	sub ax, l_x_diff
	mov l_x_diff, ax
	mov negative, 1
	jmp dv_temps_set_
dv_y2_lesser_:
	mov ax, l_x2
	mov temp1, ax
	mov bx, l_y2
	mov temp2, bx
	mov ax, l_y1
	mov temp4, ax
	sub ax, l_y2
	mov l_y_diff, ax
	mov ax, l_x1
	mov temp3, ax
	sub ax, l_x2
	mov l_x_diff, ax
	cmp ax, 0
	jg dv_temps_set_
	mov ax, 0
	sub ax, l_x_diff
	mov l_x_diff, ax
	mov negative, 1
dv_temps_set_:
	mov cx, temp1
	mov dx, temp2
	mov al, color
	mov ah, 0ch
	int 10h
	mov ax, temp4
	sub ax, temp2
	mov dx, 0
	mul l_x_diff
	div l_y_diff
	mov bx, temp3
	cmp negative, 0
	je dv_negative_
	add bx, ax
	jmp donewith_negative_
	dv_negative_:
	sub bx, ax
	donewith_negative_:
	mov temp1, bx
	inc temp2
	mov ax, temp2
	cmp ax, temp4
	je dv_ended_
	jmp dv_temps_set_

dv_ended_:


pop_reg
ret
draw_vertical_ endp


draw_horizontal_ proc
push_reg
	mov negative, 0
	mov ax, l_x1
	cmp l_x2, ax
	jl dl_x2_lesser_
	mov ax, l_x1
	mov temp1, ax
	mov bx, l_y1
	mov temp2, bx
	mov ax, l_x2
	mov temp3, ax
	sub ax, l_x1
	mov l_x_diff, ax
	mov ax, l_y2
	mov temp4, ax
	sub ax, l_y1
	mov l_y_diff, ax
	cmp ax, 0
	jg dl_temps_set_
	mov ax, 0
	sub ax, l_y_diff
	mov l_y_diff, ax
	mov negative, 1
	jmp dl_temps_set_
dl_x2_lesser_:
	mov ax, l_x2
	mov temp1, ax
	mov bx, l_y2
	mov temp2, bx
	mov ax, l_x1
	mov temp3, ax
	sub ax, l_x2
	mov l_x_diff, ax
	mov ax, l_y1
	mov temp4, ax
	sub ax, l_y2
	mov l_y_diff, ax
	cmp ax, 0
	jg dl_temps_set_
	mov ax, 0
	sub ax, l_y_diff
	mov l_y_diff, ax
	mov negative, 1
dl_temps_set_:
	mov cx, temp1
	mov dx, temp2
	mov al, color
	mov ah, 0ch
	int 10h
	mov ax, temp3
	sub ax, temp1
	mov dx, 0
	mul l_y_diff
	div l_x_diff
	mov bx, temp4
	cmp negative, 0
	je dl_negative_
	add bx, ax
	jmp done_with_negative_
	dl_negative_:
	sub bx, ax
	done_with_negative_:
	mov temp2, bx
	inc temp1
	mov ax, temp1
	cmp ax, temp3
	je dh_ended_
	jmp dl_temps_set_

dh_ended_:

pop_reg
ret
draw_horizontal_ endp


code ends
