* The second order file 
* from the Symbolic Analysis Techniques
* Chapter 1, G. Gielen

R1 1 2 1k
R2 2 4 2k
R3 2 3 5k
C1 2 0 10nf
C2 3 4 20nf
EA0 4 0 0 3 1e5

vin 1 0 dc 1.0 ac 1.0
.ac dec 10 1 10000
.print ac vdb(4)
 
 
