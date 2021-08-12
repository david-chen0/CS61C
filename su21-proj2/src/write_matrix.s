.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
# - If you receive an fopen error or eof,
#   this function terminates the program with error code 64
# - If you receive an fwrite error or eof,
#   this function terminates the program with error code 67
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 65
# ==============================================================================
write_matrix:

    # Prologue
    addi sp, sp -32
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    sw s3, 16(sp)
    sw s4, 20(sp)
    sw s5, 24(sp)
    sw s6, 28(sp)

    # save register
    mv s0, a0
    mv s1, a1
    mv s2, a2
    mv s3, a3
    
    # initialise params for fopen
    mv a1, s0
    li a2, 1 # w mode overwrite old files
    
    jal fopen
    mv s4, a0 # store file descriptor

    li t0, -1 # error code for fopen
    beq a0, t0, fopen_error
  
    mv s4, a0 # store file descriptor

    #create malloc
    li a0, 8
    jal malloc 

    beq a0, x0, malloc_error
 
    #store # of rows and cols into heap; these are needed for fwrite
    mv s6, a0
    sw s2, 0(s6)
    sw s3, 4(s6)

    # initialise params for fwrite
    mv a1, s4 # file descriptor
    mv a2, s6 # buffer
    li a3, 2
    li a4, 4 # int::4bytes

    jal fwrite
    li a3, 2
    bne a0, a3, fwrite_error

    #close 
    mv a1, s4
    jal fclose
  
    bne a0, x0, fclose_error

    #free malloc
    mv a0, s6 # initialise pointer param for free
    jal free

    #open but this time in a mode
    # initialise params for fopen
    mv a1, s0
    li a2, 2 # a mode
    
    jal fopen
    mv s4, a0 # store file descriptor

    li t0, -1 # error code for fopen
    beq a0, t0, fopen_error
  
    mv s4, a0 # store file descriptor

    # initialise params for fwrite
    mv a1, s4 # file descriptor
    mv a2, s1 # buffer
    mul s5, s2, s3 # number of items
    mv a3, s5
    li a4, 4 # int::4bytes

    jal fwrite
    bne a0, s5, fwrite_error
 
    mv a1, s4
    jal fclose
  
    bne a0, x0, fclose_error
    
    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    lw s4, 20(sp)
    lw s5, 24(sp)
    lw s6, 28(sp)
    addi sp, sp 32

    ret

fopen_error:
    li a1, 64
    jal exit2

fwrite_error:
    li a1, 67
    jal exit2

fclose_error:
    li a1, 65
    jal exit2

malloc_error:
    li a1, 48
    jal exit2 
