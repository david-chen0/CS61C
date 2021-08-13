.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
# 	d = matmul(m0, m1)
# Arguments:
# 	a0 (int*)  is the pointer to the start of m0 
#	a1 (int)   is the # of rows (height) of m0
#	a2 (int)   is the # of columns (width) of m0
#	a3 (int*)  is the pointer to the start of m1
# 	a4 (int)   is the # of rows (height) of m1
#	a5 (int)   is the # of columns (width) of m1
#	a6 (int*)  is the pointer to the the start of d
# Returns:
#	None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 34
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 34
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 34
# =======================================================
matmul:
    # Error checks
	addi t0, x0, 1
    blt a1, t0, error
    blt a2, t0, error
    blt a4, t0, error
    blt a5, t0, error
    bne a2, a4, error

    # Prologue
    addi sp, sp, -36
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    sw s3, 16(sp)
    sw s4, 20(sp)
    sw s5, 24(sp)
    sw s6, 28(sp)
    sw s7, 32(sp)
    
    mv s0, a0 # Pointer to m0
    mv s1, a1 # Number of rows of m0 
    mv s2, a2 # Length of vectors for dot / the # of columns (width) of m0
    mv s3, a3 # Pointer to m1
    mv s4, a5 # Number of cols of m1  
    
    mv s5, x0 # Counter i
    mv s6, x0 # Counter j
    
    mv s7, a6 # Pointer to the result array / Assume that the length is already well-formed(i.e n*k)

outer_loop_start:
	beq s5, s1, outer_loop_end

inner_loop_start:
	#initialising the args for dot function a0, a1, a2, a3, a4
    addi t5, x0, 4 # constant 4 byte to use later for stride
	mul t0, s5, s2 # i * m
    mul t0, t0, t5 # i * m * 4 byte
    add a0, s0, t0 # m0 + (i * m) to get to the right starting address of m0
    #calculate the new v1 pointer addr
    #formula is given by 4 * j + v0 pointer addr
    mul t0, t5, s6 # 4 * j
    add t0, t0, s3 # 4 * j + v0 pointer addr
    mv a1, t0 # v1 new pointer addr
    mv a2, s2 # length of the vectors
    li a3, 1
    add a4, s4, x0
    jal ra dot
    sw a0, 0(s7)
    addi s7, s7, 4 # move to the next index in the result array
    addi s6, s6, 1 # counter j++
	blt s6, s4, inner_loop_start
    
inner_loop_end:
    mv s6, x0 # reset Counter j to 0
    addi s5, s5, 1 # counter i ++
	jal x0 outer_loop_start

outer_loop_end:
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    lw s4, 20(sp)
    lw s5, 24(sp)
    lw s6, 28(sp)
    lw s7, 32(sp)
    addi sp, sp, 36
    # Epilogue
    ret
    
error:
	li a1, 34
	jal ra exit2
    