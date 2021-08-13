addi t0, t0, 2000
addi t1, t1, 0x605
sb t1, 0(t0)
sb t1, 1(t0)
sb t1, 2(t0)
sb t1, 3(t0)
sb t1, -1(t0)
sb t1, -4(t0)
sb t1, -2(t0)
sb t1, 10(t0)
sb t1, -20(t0)
lb t1, 0(t0)
lb t1, 1(t0)
lb t1, 2(t0)
lb t1, 3(t0)
lb s0, 5(t0)
lb s3, -3(t0)
lb s2, 8(t0)
lb s2, -7(t0)

addi t2, t2, 1200
addi t1, t0, 20
sw t1, 0(t2)
lw t1, 0(t2)

addi t1, t1, 30
sw t1, 4(t2)
lw t1, 4(t2)

addi t1, t1, 40
sb t1, 2(t2)
lb t1, 3(t2)

addi t1, t1, 31
sh t1, 2(t2)
lh t1, 2(t2)

addi t1, t1, 304
sh t1, 0(t2)
lh t1, 0(t2)

addi t1, t1, 41
sh t1, 1(t2)
lh t1, 1(t2)

addi t1, t1, 93
sb t1, 1(t2)
lb t1, 2(t2)

lw t1, 0(t2)

sw t1, 0(t2)
sb t1, 3(t2)
lw t1, 4(t2)
sh t1, 2(t2)
lh t1, 0(t2)
lb t1, 3(t2)
