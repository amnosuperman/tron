include 2_proc.inc
include bike.inc
include turn.inc

data segment

	replay_temp db 100 
	paused dw 0
	replay_view db 0; 0 for user, 1 for ai
	player_color db 0
	a dw 0
	x dw 0	
	dw_temp1 dw 0
	input db 100
	dg1 dw 0
	dg2 dw 0
	dg3 dw 0
	dg4 dw 0
	dg5 dw 0
	dg6 dw 0
	dg7 dw 0
	drp_x dw 0
	drp_y dw 0
	matrix db 40804 dup(0) 
	pointer dw 0
	to_draw_x dw 0
	to_draw_y dw 0
	ui_pointer dw 0
	ai_player_x dw 100
	ai_player_y dw 100
	ui_player_x dw 150
	ui_player_y dw 10
	game_status db 0
	ai_direction db 3
	ui_direction db 1
	prev_ui_direction db 1
	ui_dirn_x dw 0
	ui_dirn_y dw 0
	t_x dw 0
	t_y dw 0
	t_dirn_x dw 0
	t_dirn_y dw -1
	is_collision db 0
	ai_dirn_x dw 0
	ai_dirn_y dw 1
	l_x1 dw 0
	l_x2 dw 0
	l_y1 dw 0
	l_y2 dw 0
	l_x_diff dw 0
	l_y_diff dw 0
	temp1 dw 0
	temp2 dw 0
	temp3 dw 0
	temp4 dw 0
	temp_1 dw 0
	temp_2 dw 0
	temp_3 dw 0
	negative db 0  
	delay db 0
	x1 dw 0
	x2 dw 0
	x3 dw 0
	x4 dw 0
	y1 dw 0
	y2 dw 0
	y3 dw 0
	y4 dw 0
	vertical db 0
	line_vert db 0
	color db 0
	dist_x dw 0
	dist_a dw 0
	filename db "path.txt",0,'$'
	press_any_key db "Press any key to start",'$'
	instr1 db "Instructions:",'$'
	instr2 db "left-a", '$'
	instr3 db "right-d", '$'
	instr4 db "pause-<space>", '$'
	player_1_wins db "User wins",'$'
	player_2_wins db "AI wins",'$'
	replay_message db "Replay-r",'$'
	quit_message db "Quit-q",'$'
	newgame_message db "New game-n",'$'
	user_select db "View: ai-1",'$'
	user_select2 db "user-0", '$'
	handle dw ?

data ends


code segment
	assume cs:code, ds:data
; 

	start:

		mov dx, data
		mov ds, dx
		mov color, 0101b
		
	start_:
		mov ah, 3ch
		mov al, 2
		mov dx, offset filename
		mov cx, 0
		int 21h
		mov handle, ax

	mov si, offset matrix
	mov dl, 1
	mov cx, 202
	in_row1:
		mov [si], dl
		dec cx
		inc si
		cmp cx, 0
		jne in_row1
	
	mov ax, 200
	mov bx, 201
	in_cols:
		mov [si], dl
		add si, bx
		mov [si], dl
		inc si
		dec ax
		cmp ax, 0
		jne in_cols
	
	mov ax, 202
	in_row2:
		mov [si], dl
		inc si
		dec ax
		cmp ax, 0
		jne in_row2

		mov si, offset matrix
		add si, 203
		mov ui_pointer, si
		
		mov ax, 0013h
		int 10h


		mov ui_direction, 2
		mov ui_dirn_x, 0
		mov ui_dirn_y, 1
		mov ai_dirn_x, 0
		mov ai_dirn_y, 1
		mov is_collision, 0

		mov delay, 9
		mov a, 100
		mov x, 200
		mov color, 11110111b

		mov color, 0100b
		call welcome_screen
		mov_cursor 20, 10
		prompt press_any_key

		mov ah, 1
		int 21h
		call write_ins		
		running:
			cmp paused, 1
			jne not_paused
			call change_direction
			jmp running
			not_paused:
			mov ax, x
			mov bx, 120
			mul bx
			mov bx, 200
			div bx
			mov bx, 200
			sub bx, ax
			mov dist_x, bx

			mov ax, dist_x
			mov bx, 2
			mul bx
			mov bx, a
			mul bx
			mov bx, 200
			div bx
			mov dist_a, ax
					
			call set_wall_corners
			call clrscr
			mov l_x1, 200
			mov l_x2, 200
			mov l_y1, 0
			mov l_y2, 200
			mov color, 11110111b
			call draw_line
			mov color, 11110111b
			call draw_frame
			call draw_pixels
			mov tt1, 198
			mov tt2, 100
			call draw_bike
			call change_direction

			cmp game_status, 0
			jne game_ended_
			call waste_time
			jmp running
		
		game_ended_:
		call end_screen
		mov_cursor 15, 26
		prompt replay_message
		mov_cursor 17, 26
		prompt newgame_message
		mov_cursor 19, 26
		prompt quit_message
		
		mov_cursor 13, 26
		cmp game_status, 2
		je user_wins
		prompt player_2_wins
		jmp game_ended
		user_wins:
		prompt player_1_wins
		game_ended:
			mov ah, 6
			mov dl, 255
			int 21h
			jz game_ended

			cmp al, 'r'
			jne not_r
			call replay
			jmp game_ended_
			not_r:
			cmp al, 'n'
			jne not_n
			call new_game
			jmp start_
			not_n:
			cmp al, 'q'
			je ended
			jmp game_ended
	
		ended:
		mov ax, 3h
		int 10h

		mov ax, 4c00h
		int 21h

draw_pixels proc
push_reg

	mov cx, ui_player_y
	mov dx, ui_player_x
	
	mov si, offset input
	mov [si], dh
	inc si
	mov [si], dl
	inc si
	mov [si], ch
	inc si
	mov [si], cl
	inc si
	mov ah, ui_direction
	mov [si], ah
	
	mov ah, 40h
	mov dx, offset input
	mov cx, 5
	mov bx, handle
	int 21h

	mov dx, ui_player_y
	mov cx, ui_player_x
	mov ah, 0ch
	mov al, 1111b
;	int 10h
	
	mov t_x, cx
	mov t_y, dx
	call get_pointer
	mov si, pointer
	mov bh, 0
	
	cmp [si], bh
	je cool
	mov game_status, 1
	cool:
	mov bh, 1
	mov [si], bh

	mov dx, ai_player_x
	mov cx, ai_player_y
	mov si, offset input
	mov [si], dh
	inc si
	mov [si], dl
	inc si
	mov [si], ch
	inc si
	mov [si], cl
	inc si

	cmp ai_dirn_x, -1
	je dp_negative
	mov ax, 1
	add ax, ai_dirn_y
	mov [si], al
	jmp dp_dirn_set
	dp_negative:
	mov ch, 3
	mov [si], ch
	dp_dirn_set:
	mov ah, 40h
	mov dx, offset input
	mov cx, 5
	mov bx, handle
	int 21h
	
	mov ah, 0ch
	mov al, 1011b
	mov dx, ai_player_y
	mov cx, ai_player_x
;	int 10h

	mov t_x, cx
	mov t_y, dx
	call get_pointer
	mov si, pointer
	mov bh, 2
	mov [si], bh
	
pop_reg
ret
draw_pixels endp

update_ui_position proc
push_reg
	
	cmp ui_direction, 0
	jne up_ui_not_zero
	mov ui_dirn_x, 0
	mov ui_dirn_y, -1
	dec ui_player_y
	sub ui_pointer, 202
	jmp up_ui_ended
	up_ui_not_zero:
	cmp ui_direction, 1
	jne up_ui_not_one
	mov ui_dirn_x, 1
	mov ui_dirn_y, 0
	inc ui_player_x
	inc ui_pointer
	jmp up_ui_ended
	up_ui_not_one:
	cmp ui_direction, 2
	jne up_ui_not_two
	mov ui_dirn_x, 0
	mov ui_dirn_y, 1
	inc ui_player_y
	add ui_pointer, 202
	jmp up_ui_ended
	up_ui_not_two:
	mov ui_dirn_x, -1
	mov ui_dirn_y, 0
	dec ui_player_x
	dec ui_pointer
	up_ui_ended:

pop_reg 
ret
update_ui_position endp

change_direction proc 
push_reg
	mov direction, 0
	mov ah, 6
	mov dl, 255
	int 21h
	jz chd_ended
	cmp paused, 1
	je chd_ended
	cmp al, 'd'
	jne chd_not_right
	mov direction, 2
	add ui_direction, 1
	cmp ui_direction, 4
	jne chd_ended
	mov ui_direction, 0
	jmp chd_ended
	chd_not_right:
	cmp al, 'a'
	jne chd_ended
	mov direction, 1
	sub ui_direction, 1
	cmp ui_direction, -1
	jne chd_ended
	mov ui_direction, 3
	chd_ended:
	cmp al, ' '
	jne chd_final
	cmp paused, 1
	jne paused_not_one
	mov paused, 0
	jmp chd_final
	paused_not_one:
	mov paused, 1
	chd_final:
	cmp paused, 1
	je chd_exit
		call update_ui_position
		call update_ai_position
		call set_a_x
		call turn
chd_exit:

pop_reg
ret
change_direction endp

update_ai_position proc
push_reg

	call detect_collison

pop_reg
ret
update_ai_position endp

detect_collison proc
push_reg
	
	mov ax, ai_player_x
	mov bx, ai_player_y
	mov t_x, ax
	mov t_y, bx
	mov ax, ai_dirn_x
	mov cx, ai_dirn_y
	add t_x, ax
	add t_y, cx

	call get_pointer
	mov si, pointer
	mov dh, 0
	cmp [si], dh
	jne collison_infront

	add t_x, ax
	add t_y, cx

	call get_pointer
	mov si, pointer
	mov dh, 0
	cmp [si], dh
	je no_collison_

	collison_infront:
	mov ax, ai_player_x
	mov t_x, ax
	mov ax, ai_player_y
	mov t_y, ax
	mov ax, ai_dirn_x
	mov bx, ai_dirn_y

	add t_x, bx
	add t_y, ax

	call get_pointer
	mov si, pointer
	mov dh, 0
	cmp [si], dh
	jne collison_positive

	add t_x, bx
	add t_y, ax

	call get_pointer
	mov si, pointer
	mov bh, 0
	cmp [si], bh
	jne collison_positive

	mov ax, ai_dirn_x
	mov bx, ai_dirn_y
	mov ai_dirn_x, bx
	mov ai_dirn_y, ax
	no_collison_:
	jmp no_collison
	
	collison_positive:
	mov ax, ai_dirn_x
	mov bx, ai_dirn_y
	mov cx, 0
	sub cx, ax
	mov ax, cx
	mov cx, 0
	sub cx, bx
	mov bx, cx

	mov cx, ai_player_x
	mov t_x, cx
	mov cx, ai_player_y
	mov t_y, cx

	add t_x, bx
	add t_y, ax

	call get_pointer
	mov si, pointer
	mov dh, 0
	cmp [si], dh
	jne ai_dead

	add t_x, bx
	add t_y, ax

	call get_pointer
	mov si, pointer
	mov dh, 0
	cmp [si], dh
	jne ai_dead

	mov ai_dirn_x, bx
	mov ai_dirn_y, ax
	jmp no_collison

	ai_dead:
	mov game_status, 2
	jmp collison_ended

no_collison:
mov ax, ai_player_x
mov bx, ai_player_y
mov cx, ai_dirn_x
add ax, cx
mov cx, ai_dirn_y
add bx, cx
mov ai_player_x, ax
mov ai_player_y, bx

collison_ended:
pop_reg
ret
detect_collison endp

get_pointer proc
push_reg
	
	mov si, offset matrix
	mov bx, t_y
	add bx, 1
	mov ax, 202
	mul bx

	add si, ax
	mov bx, t_x
	add si, bx
	inc si

	mov pointer, si

pop_reg
ret
get_pointer endp

draw_line proc
push_reg

	call check_same
	mov cl, is_same
	cmp cl, 1
	je dl_ended
	mov ax, l_x1
	cmp l_x2, ax
	jg dl_x2_greater
	mov ax, l_x1
	sub ax, l_x2
	jmp dl_x_diff_done
	dl_x2_greater:
	mov ax, l_x2
	sub ax, l_x1
	dl_x_diff_done:

	mov bx, l_y1
	cmp l_y2, bx
	jg dl_y2_greater
	mov bx, l_y1
	sub bx, l_y2
	jmp dl_y_diff_done
	dl_y2_greater:
	mov bx, l_y2
	sub bx, l_y1
	dl_y_diff_done:

	cmp ax, bx
	jge dl_horizontal
	call draw_vertical
	
	jmp dl_ended
	dl_horizontal:
	call draw_horizontal

dl_ended:
pop_reg
ret
draw_line endp


draw_vertical proc
push_reg
	mov negative, 0
	mov ax, l_y1
	cmp l_y2, ax
	jl dv_y2_lesser
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
	jg dv_temps_set
	mov ax, 0
	sub ax, l_x_diff
	mov l_x_diff, ax
	mov negative, 1
	jmp dv_temps_set
dv_y2_lesser:
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
	jg dv_temps_set
	mov ax, 0
	sub ax, l_x_diff
	mov l_x_diff, ax
	mov negative, 1
dv_temps_set:
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
	je dv_negative
	add bx, ax
	jmp donewith_negative
	dv_negative:
	sub bx, ax
	donewith_negative:
	mov temp1, bx
	inc temp2
	cmp temp2, 200
	jg dv_ended
	cmp temp1, 200
	jg dv_ended
	mov ax, temp2
	cmp ax, temp4
	je dv_ended
	jmp dv_temps_set

dv_ended:


pop_reg
ret
draw_vertical endp


draw_horizontal proc
push_reg
	mov negative, 0
	mov ax, l_x1
	cmp l_x2, ax
	jl dl_x2_lesser
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
	jg dl_temps_set
	mov ax, 0
	sub ax, l_y_diff
	mov l_y_diff, ax
	mov negative, 1
	jmp dl_temps_set
dl_x2_lesser:
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
	jg dl_temps_set
	mov ax, 0
	sub ax, l_y_diff
	mov l_y_diff, ax
	mov negative, 1
dl_temps_set:
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
	je dl_negative
	add bx, ax
	jmp done_with_negative
	dl_negative:
	sub bx, ax
	done_with_negative:
	mov temp2, bx
	inc temp1
	cmp temp1, 200
	jg dh_ended
	cmp temp2, 200
	jg dh_ended
	mov ax, temp1
	cmp ax, temp3
	je dh_ended
	jmp dl_temps_set

dh_ended:

pop_reg
ret
draw_horizontal endp

draw_frame proc
push_reg

	mov dx, 200
	mov cx, 100
	mov al, 0100b
	mov ah, 0ch
	int 10h
	mov dx, 199
	int 10h
	mov dx, 198
	int 10h

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

	;temp3-x, temp1-a, temp2-b
	mov ax, 100
	cmp temp_1, ax
	jge df_a_exceeds
	mov ax, 100
	sub ax, temp_1
	mov l_x2, ax
	mov dx, 0
	mul temp_3
	div temp_1
	add ax, temp_3
	mov dg7, ax
	mov l_y1, ax
	mov l_x1, 0
	mov ax, temp_3
	mov l_y2, ax
	call draw_line
	jmp df_a_done

	df_a_exceeds:
	mov temp_1, 100
	df_a_done:
	mov ax, 100
	cmp temp_2, ax
	jge df_b_exceeds

	mov ax, temp_3
	mov l_y1, ax
	mov ax, 100
	add ax, temp_2
	mov l_x1, ax

	mov ax, 100
	sub ax, temp_2
	mul temp_3
	div temp_2
	add ax, temp_3
	mov l_y2, ax
	mov dg6, ax
	mov l_x2, 200

	call draw_line

	jmp df_b_done

	df_b_exceeds:
	mov temp_2, 100
	df_b_done:

	call draw_grid
	mov ax, temp_3
	mov l_y1, ax
	mov l_y2, ax
	mov ax, 100
	sub ax, temp_1
	mov l_x1, ax
	mov ax, 100
	add ax, temp_2
	mov l_x2, ax
	call draw_line
	mov ax, l_x1
	mov l_x2, ax
	mov l_y2, 0
;	call draw_line
	mov ax, temp_3
	mov l_y1, ax
	mov ax, temp_2
	add ax, 100
	mov l_x1, ax
	mov l_x2, ax
	mov l_y2, 0
;	call draw_line

	call draw_walls

pop_reg
ret
draw_frame endp

clrscr proc
push_reg

        mov ax,0600h            ;; 06 is for clearscreen and 00 is full screen
        mov bh,00               ;; 3 is background colour and 1 is foreground colour
        mov cx, 0000           ;;top left corner(ch is row)
        mov dx, 1818h           ;; bottom right
        int 10h
 
pop_reg
ret
clrscr endp

clrscr_rite proc
push_reg

        mov ax,0600h            ;; 06 is for clearscreen and 00 is full screen
        mov bh,00               ;; 3 is background colour and 1 is foreground colour
        mov cx, 0019h           ;;top left corner(ch is row)
        mov dx, 1831h           ;; bottom right
        int 10h
 
pop_reg
ret
clrscr_rite endp

set_a_x proc
push_reg
	
	cmp ui_direction, 0
	jne set_not_zero
	mov ax, ui_player_x	
	mov a, ax
	mov ax, ui_player_y
	mov x, ax
	jmp set_ended

	set_not_zero:
	cmp ui_direction, 1
	jne set_not_one
	mov ax, ui_player_x
	mov bx, 200
	sub bx, ax
	mov x, bx
	mov ax, ui_player_y
	mov a, ax
	jmp set_ended

	set_not_one:
	cmp ui_direction, 2
	jne set_not_two
	mov ax, ui_player_x
	mov bx, 200
	sub bx, ax
	mov a, bx
	mov ax, ui_player_y
	mov bx, 200
	sub bx, ax
	mov x, bx
	jmp set_ended

	set_not_two:
	mov ax, ui_player_x
	mov x, ax
	mov ax, ui_player_y
	mov bx, 200
	sub bx, ax
	mov a, bx

set_ended:
pop_reg
ret
set_a_x endp

draw_walls proc
push_reg

	cmp ui_dirn_x, 0
	jne drw_intermediate
	mov si, offset matrix
	add si, 202
	mov ax, ui_player_y
	mov dx, 0
	add ax, ui_dirn_y
	mov to_draw_y, ax
	mov bx, 202
	mul bx
	add si, ax
	cmp ui_dirn_y, -1
	je drw_vrt_mom_up
	add si, 200
	jmp drw_vrt_drawing
	drw_vrt_mom_up:
	inc si
	drw_vrt_drawing:
		mov to_draw_x, 200
		drw_vrt_row:
			mov ch, 0
			cmp [si], ch
			je drw_vrt_row_nz
			mov cl, [si]
			cmp cl, 1
			je drw_vrt_user
			mov player_color, 1001b
			jmp drw_vrt_color_set
			drw_vrt_user:
			mov player_color, 0100b
			jmp drw_vrt_color_set
			drw_intermediate:
			jmp drw_horizontal
			drw_vrt_color_set:
			call draw_3d_pixel
			drw_vrt_row_nz:
			dec to_draw_x
			sub si, ui_dirn_y
			cmp to_draw_x, 0
			jg drw_vrt_row
		add si, ui_dirn_y
		cmp ui_dirn_y, 1
		je drw_vrt_not1
		mov dx, 401
		sub si, dx
		jmp drw_vrt_si_set
		drw_vrt_not1:
		add si, 401
		drw_vrt_si_set:
		mov ax, to_draw_y
		add ax, ui_dirn_y
		mov to_draw_y, ax
		cmp to_draw_y, 0
		jle drw_ended_
		cmp to_draw_y, 200
		jge drw_ended_
		jmp drw_vrt_drawing
	drw_ended_:
	jmp drw_ended

	drw_horizontal:
	
	mov si, offset matrix
	cmp ui_dirn_x, 1
	je ui_moving_right
	add si, 40601
	jmp ui_si_set
	ui_moving_right:
	add si, 203

	ui_si_set:
	
	mov to_draw_x, 1
	cmp ui_dirn_x, -1
	je ui_hr_not_1
	mov ax, 200
	sub ax, ui_player_x
	mov dw_temp1, ax
	jmp ui_hr_set
	ui_hr_not_1:
	mov ax, ui_player_x
	mov dw_temp1, ax
	ui_hr_set:
		mov ax, dw_temp1
		mov to_draw_y, ax
		mov ax, 200
		sub ax, dw_temp1
		cmp ui_dirn_x, 1
		je ui_hr_dr_right
		sub si, ax
		jmp ui_hr_drawing
		ui_hr_dr_right:
		add si, ax
		ui_hr_drawing:
			mov bl, 0
			cmp [si], bl
			je ui_hr_not_one
			mov cl, [si]
			cmp cl, 1
			je ui_hr_user
			mov player_color, 1001b
			jmp ui_hr_color_set
			ui_hr_user:
			mov player_color, 0100b
			ui_hr_color_set:
			call draw_3d_pixel
			ui_hr_not_one:
			add si, ui_dirn_x
			dec to_draw_y
			cmp to_draw_y, 0
			jg ui_hr_drawing
		add si, ui_dirn_x
		add si, ui_dirn_x
		inc to_draw_x
		cmp to_draw_x, 200
		jl ui_hr_set

drw_ended:
pop_reg
ret
draw_walls endp

draw_3d_pixel proc
push si
push_reg
	
	cmp ui_dirn_x, 0
	jne drp_mov_horiz

	mov ax, 201
	sub ax, to_draw_x
	mov drp_x, ax

	cmp ui_dirn_y, 1
	je drp_mov_y_1
	mov ax, to_draw_y
	mov drp_y, ax
	jmp drp_mov_y_set
	drp_mov_y_1:
	mov ax, 200
	sub ax, to_draw_y
	mov drp_y, ax
	drp_mov_y_set:
	jmp drp_x_y_set

	drp_mov_horiz:
	mov ax, to_draw_x
	mov drp_x, ax
	mov ax, to_draw_y
	mov drp_y, ax
	drp_x_y_set:

	mov ax, drp_y
	mov bx, 120
	mov dx, 0
	mul bx
	mov cx, 200
	div cx
	add ax, dist_x
	mov drp_y, ax

	mov ax, drp_x
	mov temp1, ax
	mov ax, a
	cmp drp_x, ax
	jg drp_x_gr_100
	mov ax, a
	sub ax, drp_x
	mov drp_x, ax
	jmp drp_x_gr_set
	drp_x_gr_100:
	mov ax, drp_x
	sub ax, a
	mov drp_x, ax
	drp_x_gr_set:

	mov ax, drp_x
	mov dx, 0
	mov bx, dist_a
	mul bx
	mov bx, a
	mov dx, 0
	div bx
	mov drp_x, ax

	mov ax, drp_x
	mov bx, drp_y
	mul bx
	mov bx, dist_x
	mov dx, 0
	div bx
	mov drp_x, ax

	mov ax, temp1
	cmp a, ax
	jl drp_temp1_greater
	mov ax, 100
	sub ax, drp_x
	mov drp_x, ax
	jmp drp_drawing
	drp_temp1_greater:
	mov ax, 100
	add ax, drp_x
	mov drp_x, ax

	drp_drawing:
	mov dx, drp_y
	mov cx, drp_x
	cmp cx, 200
	jg drp_ended
	mov al, player_color
	mov ah, 0ch
	int 10h
	jmp drp_ended

drp_ended:
pop_reg
pop si
ret
draw_3d_pixel endp

draw_grid proc
push_reg

	;temp3-x, temp1-a, temp2-b 
	mov ax, dist_a
	mov dg1, ax
	mov ax, dist_x
	mov dg3, ax
	mov cx, 2
	mul cx
	sub ax, dg1
	mov dg2, ax
	mov ax, dg1
	mov dg4, ax
	mov dg1, 0

	cmp dg3, 200
	jl dg_continue
	mov dg3, 198
	dg_continue:
	mov color, 11110111b
	mov l_x1, 100
	mov l_x2, 100
	mov ax, dg3
	mov l_y1, ax
	mov l_y2, 200
	call draw_line
	cmp dist_x, 198
	jge dg_vert_ended
	dg_vert:
		add dg1, 10
		cmp dg1, 0
		jle dg_vert_ended
		cmp dg1, 100
		jge dg_vert_negative
		mov ax, dg1
		cmp dg4, ax
		jl dg_vert_ended
		mov ax, 100
		sub ax, dg1
		mov l_x2, ax
		mov dx, 0
		mul dg3
		mov dx, 0
		div dg1
		add ax, dg3
		mov l_y1, ax
		mov l_x1, 0
		mov ax, dg3
		mov l_y2, ax
		call draw_line
		dg_vert_negative:
		jmp dg_vert
	dg_vert_ended:

	mov ax, dist_x
	mov dg3, ax

	mov ax, dg7
	sub ax, dg3
	mov dg7, ax

	mov ax, dg6
	sub ax, dg3
	mov dg6, ax

	mov ax, 200
	sub ax, dist_x
	mov dg1, ax

	mov dg2, 0
	cmp dg7, 0
	je dg_hor_ended_
	dg_horiz:
		add dg2, 10
		mov ax, dg1
		cmp dg2, ax
		jg dg_hor_ended_
		mov ax, dg2
		cmp dg7, ax
		jg dg_hor_run
		mov l_x1, 0
		jmp dg_hor_left_set
		dg_hor_run:
		
		cmp dist_x, 198
		jg dg_hor_ended_
		cmp dist_a, 100
		jl dg_hor_a_less
		mov l_x1, 0
		jmp dg_hor_left_set
		dg_hor_a_less:
		mov ax, 100
		sub ax, dist_a
		mov bx, ax
		mov dx, 0
		mul dg2
		mov dx, 0
		div dg7
		add ax, dist_a
		mov bx, ax
		mov ax, 100
		sub ax, bx
		mov l_x1, ax
		dg_hor_left_set:
		mov ax, dist_x
		add ax, dg2
		mov l_y1, ax
		mov l_y2, ax

		mov ax, dg2
		cmp dg6, ax
		jg dg_hor_run1
		mov l_x2, 200
		jmp dg_hor_right_set
		dg_hor_ended_:
		jmp dg_horiz_ended
		dg_hor_run1:
		
		mov ax, dist_x
		mov bx, 2
		mul bx
		sub ax, dist_a
		mov dg4, ax
		cmp ax, 100
		jl dg_hor_b_less
		mov l_x2, 200
		dg_hor_b_less:
		mov ax, 100
		sub ax, dg4
		mul dg2
		mov dx, 0
		div dg6
		add ax, dg4
		add ax, 100
		mov l_x2, ax

		dg_hor_right_set:
		call draw_line
		jmp dg_horiz

	dg_horiz_ended:
	cmp dist_x, 198
	jge dg_ended
		mov dg1, 0
		dg_right_draw:
			add dg1, 10
			mov ax, temp_2
			cmp dg1, ax
			jge dg_ended
			mov ax, dg1
			mov bx, 200
			mul bx
			mov dx, 0
			div dist_x
			add ax, 100
			mov l_x2, ax
			mov l_y2, 200
			mov ax, dist_x
			mov l_y1, ax
			mov ax, 100
			add ax, dg1
			mov l_x1, ax
			call draw_line
			jmp dg_right_draw

	
dg_ended:
pop_reg
ret
draw_grid endp

replay proc
push_reg

	 mov_cursor 21, 26
	 prompt user_select
	 mov_cursor 23, 26
	 prompt user_select2
	
	
	 mov ah, 1
	 int 21h
	 
	 sub al, 30h
	  mov replay_view, al
	
        mov ax,0600h            ;; 06 is for clearscreen and 00 is full screen
        mov bh,00               ;; 3 is background colour and 1 is foreground colour
        mov cx, 0e1ah           ;;top left corner(ch is row)
        mov dx, 1828h           ;; bottom right
        int 10h
        
	
	mov si, offset matrix
	add si, 203
	mov cl, 0
	mov ax, 200
	replay_row_clear:
	  mov bx, 200
	  replay_col_clear:
		mov [si], cl
		dec bx
		inc si
		cmp bx, 0
		jg replay_col_clear
	inc si
	inc si
	dec ax
	cmp ax, 0
	jg replay_row_clear
		
	mov bx, handle
	mov ah, 3eh
	int 21h
	
	mov ah, 3dh
	mov al, 0
	mov dx, offset filename
	mov cx, 0
	int 21h
	mov handle, ax
	
	mov ah, 3fh
	mov dx, offset input
	mov cx, 10
	mov bx, handle
	int 21h

	cmp replay_view, 0
	je replay_user
	call swap_replay
	replay_user:
	cmp ax, 0
	je replay_dummy1
	mov si, offset input
	mov dh, [si]
	inc si
	mov dl, [si]
	inc si
	mov ch, [si]
	inc si
	mov cl, [si]
	mov ui_player_y, cx
	mov t_y, cx
	mov ui_player_x, dx
	mov t_x, dx
	inc si
	mov dl, [si]
	mov ui_direction, dl
	call get_pointer
	mov si, pointer
	mov dl, 1
	mov [si], dl
	
	mov al, ui_direction
	mov prev_ui_direction, al
	
	jmp replay_dummy_end1
	replay_dummy1:
	jmp replay_dummy
	replay_dummy_end1:
	
	mov si, offset input
	add si, 5
	mov dh, [si]
	inc si
	mov dl, [si]
	inc si
	mov t_x, dx
	mov ch, [si]
	inc si
	mov cl, [si]
	mov t_y, cx
	call get_pointer
	mov si, pointer
	mov bl, 2
	mov [si], bl
	
	call set_a_x
	
	replay_looping:
			mov ax, x
			mov bx, 120
			mul bx
			mov bx, 200
			div bx
			mov bx, 200
			sub bx, ax
			mov dist_x, bx

			mov ax, dist_x
			mov bx, 2
			mul bx
			mov bx, a
			mul bx
			mov bx, 200
			div bx
			mov dist_a, ax

			call set_wall_corners
			call clrscr
			mov tt1, 198
			mov tt2, 100
			call draw_bike
			mov l_x1, 200
			mov l_x2, 200
			mov l_y1, 0
			mov l_y2, 200
			mov color, 11110111b
			call draw_line
			mov color, 11110111b
			call draw_frame
			call waste_time


	mov ah, 3fh
	mov dx, offset input
	mov cx, 10
	mov bx, handle
	int 21h

	cmp replay_view, 0
	je replayuser
	call swap_replay
	replayuser:
	cmp ax, 0
	je replay_dummy
	mov si, offset input
	mov dh, [si]
	inc si
	mov dl, [si]
	inc si
	mov ch, [si]
	inc si
	mov cl, [si]
	mov ui_player_x, dx
	mov t_x, dx
	mov ui_player_y, cx
	mov t_y, cx
	inc si
	mov dl, [si]
	mov ui_direction, dl
	call get_pointer
	mov si, pointer
	mov dl, 1
	mov [si], dl
	
	jmp replay_dummy_end
	replay_dummy:
	jmp replay_ended
	replay_dummy_end:
	mov si, offset input
	add si, 5
	mov dh, [si]
	inc si
	mov dl, [si]
	inc si
	mov t_x, dx
	mov ch, [si]
	inc si
	mov cl, [si]
	mov t_y, cx
	call get_pointer
	mov si, pointer
	mov bl, 2
	mov [si], bl
	
	mov direction, 0
	mov al, ui_direction
	sub al, prev_ui_direction
	cmp al, 0
	je change_dirn_done
	cmp al, -1
	je left_dirn_change
	cmp al, 3
	je left_dirn_change
	mov direction, 2
	jmp change_dirn_done
	left_dirn_change:
	mov direction, 1
	
		change_dirn_done:
			call update_ui_position
			call set_a_x
			call turn
			mov al, ui_direction
			mov prev_ui_direction, al
			jmp replay_looping
			
replay_ended:
	
pop_reg
ret
replay endp

new_game proc
push_reg
mov replay_view, 0
mov si, offset matrix
mov ax, 0
mov bl, 0
mov ax, 202
new_rows:
	mov bx, 202
	mov ch, 0
	new_cols:
		mov [si], ch
		inc si
		dec bx
		cmp bx, 0
		jg new_cols
	dec ax
	cmp ax, 0
	jg new_rows

	mov game_status, 0
	mov ui_player_x, 150
	mov ui_player_y, 100
	mov ai_player_x, 100
	mov ai_player_y, 100

	mov cx, 0
	mov dx, offset filename
	mov ah, 3ch
	int 21h
pop_reg
ret
new_game endp


swap_replay proc
push_reg
push si
push di

	mov si, offset input
	mov di, offset replay_temp
	
	mov ax, 10
	sw_copying:
		mov bl, [si]
		mov [di], bl
		inc si
		inc di
		dec ax
		cmp ax, 0
		jg sw_copying

	mov si, offset replay_temp
	mov ah, [si]
	inc si
	mov al, [si]
	inc si
	mov bh, [si]
	inc si
	mov bl, [si]
	inc si
	mov cl, [si]
	inc si
;	int 3
	mov ah, [si]
	inc si
	mov al, [si]
	inc si
	mov bh, [si]
	inc si
	mov bl, [si]
	inc si
	mov cl, [si]
;	int 3
	mov si, offset input
	add si, 5
	mov di, offset replay_temp

	mov ax, 5
	sw_swap1:
		mov bl, [di]
		mov [si], bl
		inc si
		inc di
		dec ax
		cmp ax, 0
		jg sw_swap1

	mov si, offset input
	mov di, offset replay_temp
	add di, 5
	mov ah, [di]
	inc di
	mov al, [di]
	inc di
	mov bh, [di]
	inc di
	mov bl, [di]
	inc di
	mov cl, [di]
;	int 3

	mov di, offset replay_temp
	add di, 5
	mov bx, di
	mov ax, 5
	sw_swap2:
		mov cl, [bx]
		mov [si], cl
		mov cl, [si]
		mov ch, [bx]
		;int 3
		inc si
		inc bx
		dec ax
		cmp ax, 0
		jg sw_swap2
pop di
pop si
pop_reg
ret
swap_replay endp

write_ins proc
push_reg


        mov ax,0600h            ;; 06 is for clearscreen and 00 is full screen
        mov bh,00               ;; 3 is background colour and 1 is foreground colour
        mov cx, 0000           ;;top left corner(ch is row)
        mov dx, 1828h           ;; bottom right
        int 10h
        
		mov_cursor 3, 26
		prompt instr1
		mov_cursor 5, 26
		prompt instr2
		mov_cursor 7, 26
		prompt instr3
		mov_cursor 9, 26
		prompt instr4

pop_reg
ret
write_ins endp

code ends
end start

