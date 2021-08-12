.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 (int*) is the pointer to the start of v0
#   a1 (int*) is the pointer to the start of v1
#   a2 (int)  is the length of the vectors
#   a3 (int)  is the stride of v0
#   a4 (int)  is the stride of v1
# Returns:
#   a0 (int)  is the dot product of v0 and v1
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 32
# - If the stride of either vector is less than 1,
#   this function terminates the program with error code 33
# =======================================================
dot:
    # Prologue
    addi sp, sp, -16
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    addi t0, x0, 1
    
    # Exits with error code 32 if vector length is less than 1
    blt a2, t0, error32
    
    # Exits with error code 33 if stride is less than 1
    blt a3, t0, error33
    blt a4, t0, error33
    
    addi t0, x0, 4
    mul a3, a3, t0 # Since ints are 4 bytes
    mul a4, a4, t0 # Since ints are 4 bytes
    
    add s0, a0, x0 # Pointer to v0(since a0 might change)
    add s1, a1, x0 # Pointer to v1(since a1 might change)
    add s2, a2, x0 # Length of vectors(since a2 might change)
    mv t0, x0 # Current dot product
    mv t1, x0 # Counter i
    

loop_start:

	lw t2, 0(s0) # v0[i]
    lw t3, 0(s1) # v1[i]

    
	mul t2, t2, t3
	add t0, t0, t2
    
    addi t1, t1, 1
    add s0, s0, a3
    add s1, s1, a4
    blt t1, s2, loop_start


loop_end:
    # Epilogue
	mv a0, t0
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    addi sp, sp, 16
    
    ret  
    
error32: 
	li a1 32
	jal ra exit2
    
error33: 
	li a1 33
	jal ra exit2
    