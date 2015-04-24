Low-pass filter with one Opamp circuit
*output: 4
*input: 1

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
*vi 2 3 dc 0 ac 1
.ends

R1	0 2 1000k
RF	2 4 10k
CF	2 4 160pf
R2	3 0 9.1k

** four opmap circuits
xe1 2 4 100 opamp
*E1 4 0 3 2 1.0E5

*VIN 1 0 DC 0 
Vs 100 3 DC 0 AC 1
.ac dec 100 2k 2000k
.print vdb(4)
*.noise lowpass.niz 
.end
