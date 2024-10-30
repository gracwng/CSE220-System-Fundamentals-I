.data 
space: 	.asciiz " "
myString: .space 8		# Reserve space for the string

.text

init_student:
	sll $a0, $a0, 10		# $a0: id, shift 10 bits to the left (32 - 22 = 10)
	or $a0, $a0, $a1 		# OR $a0: id and $a1: credits
							# take contents of $a0 and write into into the first 4 bytes (first word) of _record
    sw $a0, 0($a3)     		# Store the word (4 bytes) with the value of $a0 at the beginning of _record (contains ID and credits)
	sw $a2, 4($a3) 			# store name into the second half of record
	
	jr $ra
	
print_student:	
	li $t0, 0xFFFFFFFF		# t0 is helper to shift and mask 
	lw $a1, 4($a0) 			# read word from $a0: record, at memory address 4 into $a1 (pointer to name)

	lw $t1 , 0($a0)			# get first half of record and store into $t1
	li $t0, 0x000003FF		# create bit mask: $t0 = 0x000003FF.
	and $a2, $t1, $t0 		# extract credit into $a2 by anding mask ($t0) and first half of record ($t1)
	
	srl $t1, $t1, 10 		# shift first half of record ($t1) by 10 bits 
	move $a3, $t1 			# extract ID into $a3 by moving first half of record ($t1) into $a3
	
	# a0 = record, a1 = name, a2 = credit, a3 = ID
	move $a0, $a3			# print ID 
	li $v0, 1				# print integer
	syscall 
	
	la $a0, space 			# print a space, not its address
	li $v0, 4 				# print string
	syscall 

	move $a0, $a2			# print credits
	li $v0, 1				# print integer
	syscall 

	la $a0, space 			# print a space, not its address
	li $v0, 4 				# print string
	syscall 

	move $a0, $a1 			# print name, not its address. QUESTION: why doesn't la work but move does?
	li $v0, 4 				# print string
	syscall 
	
	jr $ra
	
init_student_array:
# move all arguments into local variables since 'a' registers will be used by init_student
	lw $t0, 0($sp)  # to prevent losing the records variable at the top of the stack, store it first

	# don't forget to save the existing values in the s registers! (biggest mistake)
	addi $sp, $sp, -12 					# make space on stack
	sw $s0, 0($sp) 						# save $s0 on stack
	sw $s1, 4($sp) 						# save $s1 on stack
	sw $s2, 8($sp) 						# save $s2 on stack

	move $s0, $a1 						# s0 = id list
	move $s1, $a2						# s1 = credits list
	move $s2, $a3						# s2 = all names
	move $s3, $t0						# s3 = all student records. load the records parameter stored in the stack into $s3
	move $s4, $a0 						# s4 = total num_students/upperBound
	
	li $s5, 0							# s5: i = 0

	loop:
		beq $s5, $s4, done 			# repeat until i = num_students
		sll $t1, $s5, 2				# t1 = i * 4. offset for IDs
		sll $t2, $s5, 2				# t1 = i * 4. offset for credits
		add $t1, $t1, $s0 			# t1 = address of id_list[i]
		add $t2, $t2, $s1 			# t1 = address of credits_list[i]
		lw $a0, 0($t1) 				# a0 = id_list[i]
		lw $a1, 0($t2)				# a1 = credits_list[i]
		
		move $a2, $s2				# a2 = pointer to student's name 

		sll $t3, $s5, 3				# t3 = i + 8. offset for records
		add $t3, $t3, $s3			# a3 = address of records[i], go to next student record
		move $a3, $t3 				# a3 = records[i], MOVE not load word because we want to pass in the address of records into init array not the actual content
		# QUESTION: why is the line below wrong?
		# add $t3, $t3, $s3			# t3 = address of records[i]
		# lw $a3, 0($t3)				# a3 = records[i]
		
		# before jumping to another function, save the return address of this function
		addi $sp, $sp, -4 			# make space on stack
		sw $ra, 0($sp) 				# save $ra on stack
		jal init_student			# call init_student
		lw $ra, 0($sp) 				# restore $s0 from stack
		addi $sp, $sp, 4 			# deallocate stack space
		
		
		# iterate through names to get the name of our student
		addi $t4, $s5, 1
		beq $t4, $s4, char_done	# if we are at the last student, then don't look for the next name
		char_loop:
			lb $t0, 0($s2)				# DON'T FORGET: Instead of checking if the pointer to names ($s2) is equal to zero, you should check if the byte at that address is zero
			beqz $t0, char_done			# repeat until names pointer = '\0'
			#sb $t0, 0($t4)				# store character into myString ($t4)
			addi $s2, $s2, 1 			# get address of next character in names 
			#addi $t4, $t4, 1			# move pointer to next empty byte in myString
			j char_loop
		char_done:
		#sb $t0, 0($t4) 					# add the null terminator
		addi $s2, $s2, 1 				# get address of next character in names 
		addi $s5, $s5, 1           			# i++
		j loop

	done: 
	lw $s0, 0($sp) 				# restore $s0 from stack
	lw $s1, 4($sp) 				# restore $s1 from stack
	lw $s2, 8($sp) 				# restore $s2 from stack		
	
	addi $sp, $sp, 12			# deallocate stack space
	
	jr $ra
	
insert:
	# first extract student ID from student record (located in a0)
	lw $a3, 0($a0)		# extract first 4 bytes of records (ID and credts) into a3
	srl $a3, $a3, 10	# shift a3 10 bits to the right. now a3 contains the student ID

	move $t0, $a3 
	div $t0, $a2		# find the initial index to start in the hashtable (studentID % tableSize = a3 / a2). store into t0 
	mfhi $t0			# t0 = initial index (where the record is supposed to be in the table)

	sll $t1, $t0, 2  	# Shift the value in $t0 left by 2 positions (equivalent to multiplying by 4)
	add $a3, $t1, $a1 	# move moving table pointer to initialIndex by adding to the memory address of initial position of tablePointer
	
	li $t1, 0			# countSize = 0 (inside t1)
	# $a2 = tableSize
	# $a1 = table of pointers to student records

	li $v0, -1						# set return register to be -1 initially

	while: 							# (while countSize < tableSize)
		bge $t1,$a2 , done_while		#  branch to target if  $t0 >= $t1 meaning countSize >= tableSize
		lw $t2, 0($a3) 				# get value in index in the table. DOUBLE CHECK THIS

		# bne	$t2, $zero, else		#  FORGOT TO HANDLE TOMBSTONES: branch to else, if value in index != NULL meaning theres already a value:
		
		beq	$t2, $zero, do 		# branch to do if the first condition is true. if it's false, go to next line
		bne $t2, $v0, else		# if the memory address contains -1, then continue, else go to else
		do:
		# (if table[initialIndex] == NULL) meaning there's an open space
		#sw $a0, 0($a3)				# put student record ($a0) at that index in the table 
		sw $a0, 0($a3)				# CAUTION: NOT SURE IF THIS IS RIGHT 
		#move $a3, $a0
		
		move $v0, $t0				# return the index. jump to done
		j done_while

		else: 						
		addi $t1, $t1, 1			# increment countSize
		# move to the nextIndex
		addi $t0, $t0, 1			# initialIndex + 1 (t0)
		div $t0, $a2 				# initialIndex = initialIndex (t0) % tableSize (a2)
		mfhi $t0					# move remainder to initial index register
		# move pointer of a3 again to its new position in the table
		sll $t3, $t0, 2  			# Shift the value in $t0 left by 2 positions (equivalent to multiplying by 4)
		add $a3, $t3, $a1 			# move moving table pointer to new index/initialIndex by adding to the memory address of initial position of tablePointer
		
		j while

	done_while:
	jr $ra


# id = a0, table = a1, tablesize = a2, currentTableIndexPosition = a3	
search:
	div $a0, $a2 						# initial index = id % table_size ($t0)
	mfhi $t0 
	li $t1, 0 							# countsize = 0, in $t1 
	move $v0, $zero 					# initialize $v0 with $zero. this means a matching record was not found
	li $v1, -1							# initialize $v1 with -1. this means a matching record was not found
	sll $t2, $t0, 2  					# Shift the value in $t0 left by 2 positions (equivalent to multiplying by 4)
	add $a3, $t2, $a1 					# move moving table pointer to initialIndex by adding to the memory address of initial position of tablePointer
	
	search_while:
	bge $t1, $a2, search_done			# if countsize >= tableSize go to search done
	lw $t3, 0($a3)	# get value in table[initialIndex] (will be stored in $t3)
	
	beq $t3, $zero, search_done			# if value in table[initialIndex] == zero, jump to search done
	li $t5, -1
	beq $t3, $t5, move_to_next_index	# if value in table[initialIndex] == -1, jump to move_to_next_index

	# check if our current student's id matches our given id:
	# extract curr student's id 

	lw $t4, 0($t3)						# extract first 4 bytes of records (ID and credts) into t4
	srl $t4, $t4, 10					# shift t4 10 bits to the right. now t4 contains the student ID

	bne $t4, $a0, move_to_next_index	# if curr studentID != input ID, jump to move_to_next_index
	move $v0, $t3						# otherwise, set $v0 with the pointer to the student record and $v1 with initialIndex in ($t0)
	move $v1, $t0
	j search_done


	move_to_next_index:
	# increment count size
	# increment to next index
	addi $t1, $t1, 1					# increment countSize
	
	# move to the nextIndex
	addi $t0, $t0, 1					# initialIndex + 1 (t0)
	div $t0, $a2 						# initialIndex = initialIndex (t0) % tableSize (a2)
	mfhi $t0							# move remainder to initial index register
	# move pointer of a3 again to its new position in the table
	sll $t2, $t0, 2  					# Shift the value in $t0 left by 2 positions (equivalent to multiplying by 4)
	add $a3, $t2, $a1 					# move moving table pointer to new index/initialIndex by adding to the memory address of initial position of tablePointer
	
	j search_while

	search_done:
	jr $ra

delete:
	addi $sp, $sp, -4 						# save return address of this function before calling search
	sw $ra, 0($sp)
	jal search 								# call search function $v0 = pointer to student record, $v1 = index of student record
	lw $ra, 0($sp)
	addi $sp, $sp, 4

	li $t0, -1								# v0 now contains the pointer to the student record 
	beq $t0, $v1, none_found 				# if (index == -1) return -1 // (in v0)
	j found 								# otherwise, go to the position of where the student record lives in the table and replace it with -1
											# return the pointer to the student record

	none_found:
	li $v0, -1								# if index = -1, return -1 into v0
	jr $ra 

	found:									# go to the position of where the student record lives in the table and replace it with -1
	sll $t4, $v1, 2							# index * 4
	add $v0, $a1, $t4						# made error here. add to the original table starting pointer, not the pointer to the student record
	li $t0, -1
	sw $t0, 0($v0)							# return the pointer to the student record
	# move $v0, $t0

	move $v0, $v1 							# $v0 should contain the array index of where the student was found
	delete_done: 
	jr $ra