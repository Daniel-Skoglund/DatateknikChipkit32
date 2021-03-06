  # labwork.S
  # Written 2015 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1 2015-2016.
  # The macros PUSH and POP are in the public domain.

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm

  # You can add your own code here
  #


.text

.global delay
.global time2string  
hexasc:
	
	
	li $t0, 10 		# l�ger 10 i t0
	slt $t1, $a0, $t0 	# ger t1 1 om vi har en siffra mellan 0-9
	beq $t1,0, letter 	# g�r till letter om talet �r mellan 10-15, allts� en bokstav
	nop
	
	addi $v0, $a0, 0x30 	#l�gger till 48 f�r att f� ascii koden

	
	jr $ra
	nop
letter:

	addi $v0, $a0, 0x37	 #l�gger till 55 f�r att f� ascii koden
	jr $ra
	nop
 
 delay:
	li	$t1,1				# constant = 1
	li	$t2,20000000		# constant <-- easy to change value?
	li	$t3,0				# var in for loop
	li	$t4,0				# var1 in while loop
	
	while:
		beq	$a0,$0,zero		# (ms == 0)? zero : ->
		nop	
		sub	$a0,$a0,$t1		# ms = ms - 1
		li	$t0,0			# i = 0
	
		for:	
			sltu	$t3,$t0,$t2	# (i < 4711)? var = 1 : var = 0
			addiu	$t0,$t0,1		# i += 1
			beq	$t3,$t1,for	# (var == 1)? for : ->
			nop
	
		sltu	$t4,$0,$a0		# (0 < ms)? var1 = 1 : var1 = 0
		bne	$t4,$t1,while		# (var1 == 1)? while : ->
		nop
	
	zero:
		jr	$ra			# return to main
		nop
	
	
time2string:

	PUSH $s7		# vi pushar alla s-register vi anv�nder f�r att inte byta orginala inneh�llet	
	PUSH $s2
	PUSH $s3
	PUSH $s4
	PUSH $ra		# vi pushar $ra f�r att inte byta orginala inneh�llet
	PUSH $a0  		# vi pushar $a0 f�r att inte byta orginala inneh�llet

	andi $a0, $a1, 0x000f 	# maskar fram f�rsta siffran i klockslaget och l�gger den i a0
	jal hexasc		# k�r hexasc
	nop	
	move $s7, $v0 		# kopierar v�rdet i v0 till s7 

	andi $a0, $a1, 0x00f0	# maskar fram andra siffran i klockslaget och l�gger den i a0
	srl $a0, $a0, 4
	jal hexasc		# k�r hexasc
	nop
	move $s2, $v0		# kopierar v�rdet i v0 till s2 

	andi $a0, $a1, 0x0f00	# maskar fram tredje siffran i klockslaget och l�gger den i a0
	srl $a0, $a0, 8
	jal hexasc		# k�r hexasc
	nop
	move $s3, $v0		# kopierar v�rdet i v0 till s3 

	andi $a0, $a1, 0xf000	# maskar fram fj�rde siffran i klockslaget och l�gger den i a0
	srl $a0, $a0, 12
	jal hexasc		# k�r hexasc
	nop
	move $s4, $v0		# kopierar v�rdet i v0 till s4 

	li $t5, 0x3a 		#l�gger tecknet ; i t5
	li $t6, 0x00 		#l�gger null i t6
	
	POP $a0 		# poppar tillbaka a0
	
	sb $s4, 0($a0) 		#sparar det som finns i register s4 till a0
	sb $s3, 1($a0)		#sparar det som finns i register s3 till a0
	sb $t5, 2($a0)		#sparar det som finns i register t5 till a0
	sb $s2, 3($a0)		#sparar det som finns i register s2 till a0
	sb $s7, 4($a0)		#sparar det som finns i register s7 till a0
	sb $t6, 5($a0)		#sparar det som finns i register t6 till a0
	
back:
	POP $ra			# poppar tillbaka ra

	POP $s4			# poppar tillbaka s4
	POP $s3			# poppar tillbaka s3
	POP $s2			# poppar tillbaka s2
	POP $s7			# poppar tillbaka s7


	jr $ra
	nop
	



