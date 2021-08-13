addi t0, x0, 10

Start: 
    addi t0, t0, -1
    jal t1, Loop # Loops ten times
    addi t4, x0, 50 # Shouldn't execute
    mul t0, t0, t4

Loop:
    beq t0, x0, Next # Jumps to next
    jal t1, Start

jalr t0, t3, 0 # Jumps to line 20
jalr t2, t1, 0 # Jumps to line 27

Next:
    addi t1, x0, 20
    jalr t3, t1, 8 # Jumps to line 13

addi t2, t2, 1
jal t1, Final
addi t1, t1, 4 # Shouldn't execute

Final: 
    jalr t1, t1, -20 # Jumps to line 14

add t2, t0, t1
jalr x0, x0, 2047
jalr x0, x0, 293
