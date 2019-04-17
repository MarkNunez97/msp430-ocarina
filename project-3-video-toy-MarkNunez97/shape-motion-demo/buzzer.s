	.file	"buzzer.c"
.data
	.balign 2
	.type	rate_c, @object
	.size	rate_c, 6
rate_c:
	.short	1
	.short	1
	.short	3
	.balign 2
	.type	complete, @object
	.size	complete, 8
complete:
	.short	3000
	.short	2000
	.short	1000
	.short	0
	.local	duration_c
	.comm	duration_c,2,2
	.local	current_note_c
	.comm	current_note_c,2,2
	.balign 2
	.type	d, @object
	.size	d, 2
d:
	.short	10
	.balign 2
	.type	note_length, @object
	.size	note_length, 2
note_length:
	.short	6
	.local	counter
	.comm	counter,2,2
.text
	.balign 2
	.global	buzzer_init
	.type	buzzer_init, @function
buzzer_init:
; start of function
; framesize_regs:     0
; framesize_locals:   0
; framesize_outgoing: 0
; framesize:          0
; elim ap -> fp       2
; elim fp -> sp       0
; saved regs:(none)
	; start of prologue
	; end of prologue
	CALL	#timerAUpmode
	MOV.B	&P2SEL2, R12
	AND.B	#63, R12
	AND	#0xff, R12
	MOV.B	R12, &P2SEL2
	MOV.B	&P2SEL, R12
	AND.B	#127, R12
	AND	#0xff, R12
	MOV.B	R12, &P2SEL
	MOV.B	&P2SEL, R12
	BIS.B	#64, R12
	AND	#0xff, R12
	MOV.B	R12, &P2SEL
	MOV.B	#64, &P2DIR
	MOV.W	&rate_c, R12
	MOV.W	R12, &duration_c
	NOP
	; start of epilogue
	RET
	.size	buzzer_init, .-buzzer_init
	.balign 2
	.global	play_complete
	.type	play_complete, @function
play_complete:
; start of function
; framesize_regs:     0
; framesize_locals:   4
; framesize_outgoing: 0
; framesize:          4
; elim ap -> fp       2
; elim fp -> sp       4
; saved regs:(none)
	; start of prologue
	SUB.W	#4, R1
	; end of prologue
	MOV.W	&current_note_c, R12
	ADD.W	R12, R12
	ADD.W	#complete, R12
	MOV.W	@R12, 2(R1)
	MOV.W	#4, @R1
	MOV.W	&current_note_c, R12
	CMP.W	@R1, R12 { JGE	.L3
	MOV.W	2(R1), R12
	CALL	#buzzer_set_period
.L3:
	MOV.W	&duration_c, R12
	ADD.W	#-1, R12
	MOV.W	R12, &duration_c
	MOV.W	&duration_c, R12
	CMP.W	#0, R12 { JNE	.L5
	MOV.W	&current_note_c, R12
	ADD.W	#1, R12
	MOV.W	R12, &current_note_c
	MOV.W	&current_note_c, R12
	ADD.W	R12, R12
	ADD.W	#rate_c, R12
	MOV.W	@R12, R12
	MOV.W	R12, &duration_c
.L5:
	NOP
	; start of epilogue
	ADD.W	#4, R1
	RET
	.size	play_complete, .-play_complete
	.balign 2
	.global	buzzer_set_period
	.type	buzzer_set_period, @function
buzzer_set_period:
; start of function
; framesize_regs:     0
; framesize_locals:   2
; framesize_outgoing: 0
; framesize:          2
; elim ap -> fp       2
; elim fp -> sp       2
; saved regs:(none)
	; start of prologue
	SUB.W	#2, R1
	; end of prologue
	MOV.W	R12, @R1
	MOV.W	@R1, R12
	MOV.W	R12, &TA0CCR0
	MOV.W	@R1, R12
	RRA.W	R12
	MOV.W	R12, &TA0CCR1
	NOP
	; start of epilogue
	ADD.W	#2, R1
	RET
	.size	buzzer_set_period, .-buzzer_set_period
	.ident	"GCC: (SOMNIUM Technologies Limited - msp430-gcc 6.2.1.16) 6.2.1 20161212"
