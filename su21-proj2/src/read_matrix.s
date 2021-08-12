.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
# - If malloc returns an error,
#   this function terminates the program with error code 48
# - If you receive an fopen error or eof, 
#   this function terminates the program with error code 64
# - If you receive an fread error or eof,
#   this function terminates the program with error code 66
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 65
# ==============================================================================
read_matrix:
    # Prologue
	addi sp, sp -36
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw s6, 24(sp)
    sw s7, 28(sp)
    sw ra, 32(sp)

    # Saved all the args 
    mv s0, a0
    mv s1, a1
    mv s2, a2

    #load args for fopen
    mv a1 s0
    li a2 0
    jal fopen 
 
    li t0, -1 # error code for fopen
    beq a0, t0, fopen_error

    mv s3, a0 # Saved file descriptor

    mv a1 s3 # pass the file descriptor
    mv a2 s1 # pass the address / pointer to int::number of rows
    li a3 4  # pass int::4 bytes argument to be read
    jal fread

    li a3 4  # pass int::4 bytes argument to be read   
    bne a0, a3, fread_error
    
    mv a1 s3 # pass the file descriptor
    mv a2 s2 # pass the address / pointer to int::number of cols
    li a3 4  # pass int::4 bytes argument to be read
    jal fread

    li a3 4  # pass int::4 bytes argument to be read   
    bne a0, a3, fread_error

    # calculate # of elements in the matrix
    lw t0, 0(s1) # row
    lw t1, 0(s2) # col
    mul s4, t0, t1 # total elements
	slli s4, s4, 2 # total number of bytes
    
    #create malloc
    mv a0, s4
    jal malloc 

    beq a0, x0, malloc_error
    mv s5, a0  # Save the pointer to the matrix in memory
    
    mv a1, s3  # pass the file descriptor
    mv a2, s5 # pass the address / pointer to int::number of rows
    mv a3, s4 # total number of bytes

    jal fread

    mv a3, s4 
    bne a0, a3, fread_error

done:
    mv a1, s3 # close file
    jal fclose
  
    bne a0, x0, fclose_error

    mv a0, s5 # return the pointer to the matrix

    # Epilogue test
    lw ra, 32(sp)
    lw s7, 28(sp)
    lw s6, 24(sp)
    lw s5, 20(sp)
    lw s4, 16(sp)
    lw s3, 12(sp)
    lw s2, 8(sp)
    lw s1, 4(sp)
    lw s0, 0(sp)
	addi sp, sp 36

    ret


fopen_error:
    li a1, 64
    jal exit2

fread_error:
	li a1, 66
	jal exit2

fclose_error:
    li a1, 65
    jal exit2
    
malloc_error:
    li a1, 48
    jal exit2 