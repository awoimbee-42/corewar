.name "nul a chier"
.comment "non"

start:
	sti		r1, %:defense, %1
	ld		%2147483647, r16
	ld		%6, r2
	ld		%0, r3
start2:
	fork	%:preattack
	sti		r1, r2, r3
	live	%155134
	fork	%:defense
	st		r1, 6
	live	%155134
	fork	%:attack2
	st		r1, 6
	live	%155134
	ld		%0, r3
	zjmp	%:start2

defense:
	live	%155134
	st		r16, -220
	st		r16, -220
	st		r16, -220
	st		r16, -220
	st		r16, -220
	st		r16, -220
	st		r16, -220
	st		r16, -220
	st		r16, -220
	st		r16, -220
	st		r16, -220
	st		r16, -220
	st		r16, -220
	st		r16, -220
	st		r16, -220
	st		r16, -220
	ld		%0, r2
	zjmp	%:defense

prepreattack:
	ld		%35, r6
preattack:
	ld		%245, r5

attack:
	sti		r16, %200, r5
	sti		r15, %203, r5
	sti		r14, %206, r5
	sti		r13, %209, r5
	sti		r12, %212, r5
	sti		r11, %215, r5
	sti		r10, %218, r5
	sti		r9, %221, r5
	sub		r5, r6, r5
	zjmp	%:preattack
	ld		%0, r2
	zjmp	%:attack

attack2:
	ld		%6, r2
	ld		%123, r5
	ld		%1234421, r6
	ld		%1234421, r7
	ld		%1234421, r8
	st		r5, r15
	st		r6, r14
	st		r6, r13
	st		r6, r12
	live	%155134
