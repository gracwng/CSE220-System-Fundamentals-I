loop:
		beq $s5, $s4, done 			
		sll $t1, $s5, 2				
		sll $t2, $s5, 2				
		add $t1, $t1, $s0 			
		lw $a0, 0($t1) 				
		lw $a1, 0($t2)				
done:
    jr $ra