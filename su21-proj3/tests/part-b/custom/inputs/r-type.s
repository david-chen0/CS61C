Start:
    jal a0, Next

After:
    addi t0, x0, 20
    addi t1, t0, 25
    add t2, t0, t1
    add x0, t0, t1

addi s0, x0, 5
addi s1, x0, 3
sub s2, s0, s1
sub s2, s2, s0
sub t0, t0, s2

addi t3, x0, 13
addi t4, t3, -16
mul t5, t3, t4
mul t5, t5, t4

addi t0, x0, 31
addi t1, x0, 32
addi t2, x0, 2
sll t0, t0, t2
sll t2, t2, t2
sll t1, t1, t2
mulh t2, t0, t1
mulhu t0, t1, t2
addi t0, x0, -5
addi t1, x0, 6
mulhu t0, t0, t1

addi t0, x0, -5
addi t1, x0, 7
sll t2, t1, t0

addi t0, x0, 5
addi t1, x0, 6
slt t2, t0, t1
slt t2, t0, t0
slt t2, t1, t0
xor t0, t0, t2
xor t2, t0, t0

addi t0, x0, 20
add t1, t0, x0
addi t2, t0, -8
srl t0, t1, t2
srl t0, t2, t1
sra t1, t1, t2
sra t1, t2, t0
sra t1, t1, t1
and s0, t0, t1
or s1, t0, t1

addi t3, x0, -2
addi t0, x0, 20
sra t1, t0, t3

jal a2, End

Next:
    jal a1, After

End:
    srl t2, t0, t3
