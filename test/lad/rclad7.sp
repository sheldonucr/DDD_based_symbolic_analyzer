*resistant Ladder filter (7 section)
R1 1 2 1k
R2 2 0 1k
c3 2 3 0.01 
R4 3 0 1k
R5 3 4 1k
c6 4 0 0.005 
R7 4 5 1k
R8 5 0 1k
R9 5 6 1k
c0 6 0 1k
R11 6 7 1k
R12 7 0 1k
R13 7 8 1k
c14 8 0 1k

vin 1 0 dc 0 ac 1
.ac dec 1000 1 10
.plot v(8)
.end
