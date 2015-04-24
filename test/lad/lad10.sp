*This is 10 sections ladder circuit

R1 1 2 1k
R2 2 0 1k
R3 2 3 1k
R4 3 0 1k

R5 3 4 1k
R6 4 0 1k
R7 4 5 1k
R8 5 0 1k
R9 5 6 1k
R10 6 0 1k
R11 6 7 1k
R12 7 0 1k
R13 7 8 1k
R14 8 0 1k
R15 8 9 1k
R16 9 0 1k
R17 9 10 1k
R18 10 0 1k
R19 10 11 1k
R20 11 0 1k

vin 1 0 dc 0 ac 1
.ac dec 1000 1 10
.plot v(11)
.end
