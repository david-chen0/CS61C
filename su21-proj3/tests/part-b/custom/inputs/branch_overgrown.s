addi t0, x0, 0
beq x0, x0, first

first:
    beq x0, t0, second
    beq x0, x0, done

second:
    addi t0, x0, 1
    beq x0, x0, first

done:
    addi, t0, x0, 1