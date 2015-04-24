*This is 3 sections ladder circuit
C1 1 0 20pf
R2 1 2 3k
C3 2 0 4pf
R4 2 3 10k
C5 3 0 30pf
R6 3 4 15k
R7 4 0 45k

vin 1 0 dc 0 ac 1
.ac dec 1 1 10
.plot v(4)
.end
