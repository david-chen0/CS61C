.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 (int*) is the pointer to the array
#	a1 (int)  is the # of elements in the array
# Returns:
#	None
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 32
# ==============================================================================
relu:
    # Prologue where do i put? can I also put at loop_start? loop_continue? what is the best practice?
    addi sp, sp, -4
    sw s0, 0(sp)
    
    li t0, 1 
    blt a1 t0 error #check length of vector is less than 1, if yes return error code 32


loop_start:
    li t0, 0 # reuse t0 as i counter for the loop
    li s0, 0 # holder to load integer
    li t1, 0 # temp placeholder for current address
    
loop_continue:
	beq t0, a1, loop_end
	#load the word
    lw s0, 0(a0)
    add t1, x0, a0 # temp to hold the current address that will be needed later to access and mod the value
    addi a0, a0, 4 # go to the next loop
    addi t0, t0, 1 # i++
    bge s0, x0, loop_continue
    sw x0, 0(t1) # return back to the memory
	jal x0 loop_continue

loop_end:
    # Epilogue
	lw s0, 0(sp)
    addi sp, sp, 4
    
	ret
    
error:
	li a1 32
    jal ra exit2