State variable filter circuit from Bozena's paper
*output: port 1, 3

R1 2 5 1.0E4
R2 5 8 2.0E4
R3 4 1 1.0E4
R4 6 11 1.0E4
R5 11 1 1.0E5
R6 10 3 1.0E4
R7 8 10 3.0E4
R8 6 7 4.55E4
R9 8 9 2.2E3
R10 4 6 1.0E+4
C1 7 8 1.6E-9
C2 9 1 1.6E-9
*xe1 5 6 4 opamp
E1 6 0 4 5 -1.0E5

*xe2 0 8 7 opamp
E2 8 0 7 0 -1.0E5

*xe3 0 1 9 opamp
E3 1 0 9 0 -1.0E5

*xe4 11 3 10 opamp
E4 3 0 10 11 -1.0E5
VIN 2 0 DC 0 AC 1
*.ac lin 100 2k 20k
.ac lin 100 2k 20k
.print vdb(1)
.end
