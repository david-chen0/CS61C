.import lotsofaccumulators.s

.data
inputarray: .word 1,2,3,4,5,6,7,0
breakfive: .word 0,3,0

TestPassed: .asciiz "Test Passed!"
TestFailed: .asciiz "Test Failed!"

.text
# Tests if the given implementation of accumulate is correct.
#Input: a0 contains a pointer to the version of accumulate in question. See lotsofaccumulators.s for more details
#
#
#
#The main function currently runs a simple test that checks if accumulator works on the given input array. All versions of accumulate should pass this.
#Modify the test so that you can catch the bugs in four of the five solutions!
main:

    
    
	la a0 inputarray
    addi s0, x0, 10 # Break accumulator 2
    jal accumulatortwo
    li t0 28
    bne a0 t0 Fail
    addi t0 x0 10
    bne s0 t0 Fail   

	la a0 inputarray
    jal accumulatorthree # Correct accumulator
    li t0 28
    bne a0 t0 Fail

    la a0 inputarray
    addi t2 t2 10 # Break accumulator 4
    jal accumulatorfour
    li t0 28
    bne a0 t0 Fail
    
    la a0 breakfive
    jal accumulatorfive
    li t0 0
    bne a0 t0 Fail
    
    j Pass
Fail:
    la a0 TestFailed
    jal print_string
    j End
Pass:
    la a0 TestPassed
    jal print_string
End:
    jal exit

print_int:
	mv a1 a0
    li a0 1
    ecall
    jr ra
    
print_string:
	mv a1 a0
    li a0 4
    ecall
    jr ra
    
exit:
    li a0 10
    ecall