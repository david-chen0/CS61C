addi sp, sp, -4
addi t0, x0, 3 # add 3 to t0
sh t0, 0(sp)
sh t0, 2(sp)
lw t1, 0(sp)