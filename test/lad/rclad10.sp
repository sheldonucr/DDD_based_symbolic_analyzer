*This is 10 sections ladder circuit
C1 1 0 1pf
R2 1 2 1k
C3 2 0 1pf
R4 2 3 1k
C5 3 0 1pf
R6 3 4 1k
C7 4 0 1pf
R8 4 5 1k
C9 5 0 1pf
R10 5 6 1k
C11 6 0 1pf
R12 6 7 1k
C13 7 0 1pf
R14 7 8 1k
C15 8 0 1pf
R16 8 9 1k
C17 9 0 1pf
R18 9 10 1k
C19 10 0 1pf
R20 10 11 1k
R21 11 0 1k

vin 1 0 dc 0 ac 1
.ac dec 1000 1 10
.plot v(11)
.end
