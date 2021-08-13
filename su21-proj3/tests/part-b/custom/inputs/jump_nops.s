start1:
    jal ra start10

start2:
    jal ra start9

start3:
    jal ra start8

start4:
    jal ra start7

start5:
    jal ra start2

start6:
    jal ra done

start7:
    jal ra start6

start8:
    jal ra start4

start9:
    jal ra start3


start10:
    jal ra start2
    
done:
    addi t0, x0, 4