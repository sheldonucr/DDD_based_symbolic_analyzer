* Testing Starzyk's example
* Testing opamps
* 
*


* format for opamp 
* opamp -node out +node

.subckt opamp 3 4 2 
rii 2 3 2meg
ri1 2 0 500meg
ri2 3 0 500meg
g1 1 0 2 3 2
c1 1 0 26.6u
r1 1 0 1k
g2 4 0 1 0 1.33333
rld 4 0 75
.ends

*subckt in+ in- out
.subckt subft 1 12 5
r1 1 2 1.0 
r2 12 8 1.0 
r3 2 0 1.0
r4 8 3 1.0
r5 3 4 1.0
r6 4 5 1.0
c6 4 5 1.0
r7 5 6 1.0
c8 6 7 1.0
r9 7 8 1.0
xo1 8 3 2 opamp
*e11 3 0 2 8 1e5 
xo2 4 5 0 opamp
*e12 5 0 0 4 1e5
xo3 6 7 0 opamp
*e13 7 0 0 6 1e5
.ends

x1 1 4 2 subft 
x2 2 3 4 subft
x3 4 5 3 subft
x4 3 8 5 subft
r37 7 5 1.0
r38 6 0 1.0
r39 7 0 1.0
r40 6 8 1.0
*e1 8 0 7 6 1e5
x5 6 8 7 opamp

vin 1 0 dc 0 ac 1
.ac dec 10 1 100meg
.print vdb(8)
.end
