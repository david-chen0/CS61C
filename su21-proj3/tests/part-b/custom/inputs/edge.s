
# Extra Strength C0FFEE
lui t0, 0xC0FFE
addi t0, t0, -4

# Siiiiiiiiiiign Extension
addi t0, x0, 0b11111111
addi t1, x0, 1500
sw t0, 0(t1)
lb t1, 0(t1)
lh t1, 0(t1)
addi t0, x0, 0b00001111
sw t0, 0(t1)
lb t0, 3(t1)
lb t0, 1(t1)

# Lots of LUIve
lui t0, 32
lui t0, 293
lui t0, 0x3928A
addi t0, t0, 403
