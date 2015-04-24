*This is 4 sections ladder circuit
C1 1 0 4pf
R2 1 2 0.1k
C3 2 0 1pf
R4 2 3 3k
C5 3 0 10pf
R6 3 4 100k
C7 4 0 92pf
R8 4 5 34k
R9 5 0 29k

vin 1 0 dc 0 ac 1
.ac dec 1 1 10
.plot v(5)
.end
