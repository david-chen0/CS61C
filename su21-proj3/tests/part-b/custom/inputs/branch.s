beq s0, s0, Start

Skipped: 
    addi t0, t0, 4
    beq t0, t0, Skipped

Start:
    addi t0, t0, 4
    addi t1, t0, 1
    beq t0, t1, Skipped
    bne t0, t1, Next
    bne x0, t0, Skipped

Middle1:
    addi t0, x0, -1
    bltu x0, t0, Final

Middle2:
    addi t0, x0, 57
    addi t1, t0, 83
    bgeu t1, t0, End

Next:
    add t1, t0, t0
    blt t1, t0, Skipped
    blt t1, t1, Skipped
    bge t0, t1, Skipped
    bge t0, t0, Middle1

Final:
    addi t0, x0, 2047
    addi t2, x0, 2046
    blt t2, t0, Middle2

End:
    addi t0, x0, -5
    addi t1, x0, 3
    bgeu t1, t0, Skipped