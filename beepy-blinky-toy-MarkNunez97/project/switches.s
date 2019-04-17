	.file	"switches.c"
	.comm	switch_state_down,1,1
	.comm	switch_state_changed,1,1
	.comm	sw1_state_down,1,1
	.comm	sw2_state_down,1,1
	.comm	sw3_state_down,1,1
	.comm	sw4_state_down,1,1
	.comm	sw1_state_changed,1,1
	.comm	sw2_state_changed,1,1
	.comm	sw3_state_changed,1,1
	.comm	sw4_state_changed,1,1
.data
	.balign 2
	.type	state, @object
	.size	state, 2
state:
	.short	5
	.comm	note,2,2
.text
	.balign 2
	.type	switch_update_interrupt_sense, @function
switch_update_interrupt_sense:
	; start of prologue
	SUB.W	#2, R1
	; end of prologue
	MOV.B	&P1IN, 1(R1)
	MOV.B	1(R1), R12
	SXT	R12
	AND.B	#8, R12
	MOV.B	R12, R13
	SXT	R13
	MOV.B	&P1IES, R12
	SXT	R12
	BIS.B	R13, R12
	SXT	R12
	AND	#0xff, R12
	MOV.B	R12, &P1IES
	MOV.B	1(R1), R12
	SXT	R12
	BIS.B	#-9, R12
	MOV.B	R12, R13
	SXT	R13
	MOV.B	&P1IES, R12
	SXT	R12
	AND.B	R13, R12
	SXT	R12
	AND	#0xff, R12
	MOV.B	R12, &P1IES
	MOV.B	1(R1), R12
	; start of epilogue
	ADD.W	#2, R1
	RET
	.size	switch_update_interrupt_sense, .-switch_update_interrupt_sense
	.balign 2
	.type	switch_update_interrupt_sense2, @function
switch_update_interrupt_sense2:
	; start of prologue
	SUB.W	#2, R1
	; end of prologue
	MOV.B	&P2IN, 1(R1)
	MOV.B	1(R1), R12
	SXT	R12
	AND.B	#15, R12
	MOV.B	R12, R13
	SXT	R13
	MOV.B	&P2IES, R12
	SXT	R12
	BIS.B	R13, R12
	SXT	R12
	AND	#0xff, R12
	MOV.B	R12, &P2IES
	MOV.B	1(R1), R12
	SXT	R12
	BIS.B	#-16, R12
	MOV.B	R12, R13
	SXT	R13
	MOV.B	&P2IES, R12
	SXT	R12
	AND.B	R13, R12
	SXT	R12
	AND	#0xff, R12
	MOV.B	R12, &P2IES
	MOV.B	1(R1), R12
	; start of epilogue
	ADD.W	#2, R1
	RET
	.size	switch_update_interrupt_sense2, .-switch_update_interrupt_sense2
	.balign 2
	.global	switch_init
	.type	switch_init, @function
switch_init:
	MOV.B	&P1REN, R12
	BIS.B	#8, R12
	AND	#0xff, R12
	MOV.B	R12, &P1REN
	MOV.B	#8, &P1IE
	MOV.B	&P1OUT, R12
	BIS.B	#8, R12
	AND	#0xff, R12
	MOV.B	R12, &P1OUT
	MOV.B	&P1DIR, R12
	BIC.B	#8, R12
	AND	#0xff, R12
	MOV.B	R12, &P1DIR
	CALL	#switch_update_interrupt_sense
	MOV.B	&P2REN, R12
	BIS.B	#15, R12
	AND	#0xff, R12
	MOV.B	R12, &P2REN
	MOV.B	#15, &P2IE
	MOV.B	&P2OUT, R12
	BIS.B	#15, R12
	AND	#0xff, R12
	MOV.B	R12, &P2OUT
	MOV.B	&P2DIR, R12
	AND.B	#-16, R12
	AND	#0xff, R12
	MOV.B	R12, &P2DIR
	CALL	#switch_update_interrupt_sense2
	CALL	#led_update
	NOP
	; start of epilogue
	RET
	.size	switch_init, .-switch_init
	.balign 2
	.global	switch_interrupt_handler
	.type	switch_interrupt_handler, @function
switch_interrupt_handler:
	; start of prologue
	SUB.W	#2, R1
	; end of prologue
	CALL	#switch_update_interrupt_sense
	MOV.B	R12, 1(R1)
	MOV.B	1(R1), R12
	MOV.W	R12, R13
	AND.B	#8, R13
	MOV.B	#1, R12
	CMP.W	#0, R13 { JEQ	.L7
	MOV.B	#0, R12
.L7:
	AND	#0xff, R12
	MOV.B	R12, &switch_state_down
	MOV.B	&switch_state_down, R12
	CMP.W	#0, R12 { JEQ	.L8
	MOV.W	#0, &state
	MOV.W	#3370, &note
	MOV.B	#1, &switch_state_changed
.L8:
	CALL	#switch_update_interrupt_sense2
	MOV.B	R12, @R1
	MOV.B	@R1, R12
	MOV.W	R12, R13
	AND.B	#1, R13
	MOV.B	#1, R12
	CMP.W	#0, R13 { JEQ	.L9
	MOV.B	#0, R12
.L9:
	AND	#0xff, R12
	MOV.B	R12, &sw1_state_down
	MOV.B	&sw1_state_down, R12
	CMP.W	#0, R12 { JEQ	.L10
	MOV.W	#1, &state
	MOV.W	#2005, &note
	MOV.B	#1, &sw1_state_changed
.L10:
	MOV.B	@R1, R12
	MOV.W	R12, R13
	AND.B	#2, R13
	MOV.B	#1, R12
	CMP.W	#0, R13 { JEQ	.L11
	MOV.B	#0, R12
.L11:
	AND	#0xff, R12
	MOV.B	R12, &sw2_state_down
	MOV.B	&sw2_state_down, R12
	CMP.W	#0, R12 { JEQ	.L12
	MOV.W	#4, &state
	MOV.W	#2825, &note
	MOV.B	#1, &sw2_state_changed
.L12:
	MOV.B	@R1, R12
	MOV.W	R12, R13
	AND.B	#4, R13
	MOV.B	#1, R12
	CMP.W	#0, R13 { JEQ	.L13
	MOV.B	#0, R12
.L13:
	AND	#0xff, R12
	MOV.B	R12, &sw3_state_down
	MOV.B	&sw3_state_down, R12
	CMP.W	#0, R12 { JEQ	.L14
	MOV.W	#3, &state
	MOV.W	#1685, &note
	MOV.B	#1, &sw3_state_changed
.L14:
	MOV.B	@R1, R12
	MOV.W	R12, R13
	AND.B	#8, R13
	MOV.B	#1, R12
	CMP.W	#0, R13 { JEQ	.L15
	MOV.B	#0, R12
.L15:
	AND	#0xff, R12
	MOV.B	R12, &sw4_state_down
	MOV.B	&sw4_state_down, R12
	CMP.W	#0, R12 { JEQ	.L16
	MOV.W	#2, &state
	MOV.W	#2250, &note
	MOV.B	#1, &sw4_state_changed
.L16:
	MOV.W	&state, R12
	CMP.W	#2, R12 { JEQ	.L18
	MOV.B	#2, R13
	CMP.W	R12, R13 { JLO	.L19
	CMP.W	#0, R12 { JEQ	.L20
	CMP.W	#1, R12 { JEQ	.L21
	BR	#.L17
.L19:
	CMP.W	#3, R12 { JEQ	.L22
	CMP.W	#4, R12 { JEQ	.L23
	BR	#.L17
.L20:
	MOV.B	&switch_state_down, R12
	MOV.W	&note, R13
	CALL	#play_buzzer
	BR	#.L17
.L21:
	MOV.B	&sw1_state_down, R12
	MOV.W	&note, R13
	CALL	#play_buzzer
	BR	#.L17
.L23:
	MOV.B	&sw2_state_down, R12
	MOV.W	&note, R13
	CALL	#play_buzzer
	BR	#.L17
.L22:
	MOV.B	&sw3_state_down, R12
	MOV.W	&note, R13
	CALL	#play_buzzer
	BR	#.L17
.L18:
	MOV.B	&sw4_state_down, R12
	MOV.W	&note, R13
	CALL	#play_buzzer
	NOP
.L17:
	MOV.B	#1, &switch_state_changed
	CALL	#led_update
	NOP
	; start of epilogue
	ADD.W	#2, R1
	RET
	.size	switch_interrupt_handler, .-switch_interrupt_handler
