.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 (int*) is the pointer to the start of the vector
#	a1 (int)  is the # of elements in the vector
# Returns:
#	a0 (int)  is the first index of the largest element
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 32
# =================================================================
argmax:
    # Prologue where do i put? can I also put at loop_start? loop_continue? what is the best practice?
    addi sp, sp, -4
    sw s0, 0(sp)
    
    # Prologue
    li t0, 1 # conditional check length of vector is less than 1
    blt a1 t0 error #check length of vector is less than 1, if yes return error code 32

loop_start:
    li t0, 1 # reuse t0 as i counter for the loop, set to 0 to implement do-while loop
    li s0, 0 # holder to load integer
    lw t2, 0(a0) # current highest value
    li t3, 0 # current index with the highest value
    addi a0, a0, 4 # go to the next loop

breakpoint:
    li t5, 1
	bne t0, t5, loop_continue
    ebreak
    
loop_continue:
	#load the word
    beq t0, a1, loop_end 
    lw s0, 0(a0)
    addi a0, a0, 4 # go to the next loop
    addi t0, t0, 1 # i++
    bge t2, s0, breakpoint
	addi t3, t0, -1
    add t2, x0, s0
	jal x0 breakpoint

loop_end:
	add a0, x0, t3
    # Epilogue
	lw s0, 0(sp)
    addi sp, sp, 4
    
	ret

error:
	li a1 32
    jal ra exit2
   