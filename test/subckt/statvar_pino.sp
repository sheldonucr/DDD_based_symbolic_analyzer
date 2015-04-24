State variable filter circuit from Bozena's paper
*output: port 1, 3
*.subckt opamp -in out +in

.subckt opamp 7 6 8 
gm1a  4 3 7 3 5.89e-06
rds1a 4 3 9.26e+7
cbd1a 3 4 2.04e-14 
cgs1a 7 3 4.78e-14
gm1b  5 3 8 3 5.89e-06
rds1b 5 3 9.26e+7
cbd1b 3 5 2.04e-14 
cgs1b 8 3 4.78e-14
gm2a  4 0 4 0 3e-05
rds2a 4 0 2.46e+7
cbd2a 0 4 1.64e-14 
cgs2a 4 0 5.5e-13
gm2b  5 0 4 0 3e-05
rds2b 5 0 2.46e+7
cbd2b 0 5 1.64e-14 
cgs2b 4 0 5.5e-13
gm3	 6 0 5 0 0.000406
rds3 6 0 4.95e+5 
cbd3 0 6 9.56e-14 
cgs3 5 0 2.01e-13 
rds4 6 0 3.62e+3
cbd4 0 6  3.05e-13  
cgd4 0 6 6.71e-14 
rds5 3 0 4.695e+7
cbd5 0 3 2.45e-14 

CC 5 6 1P
CL 6 0 10P
RL 6 0 100K
.ENDS

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
xe1 5 6 4 opamp
*E1 6 0 4 5 -1.0E5

xe2 0 8 7 opamp
*E2 8 0 7 0 -1.0E5

xe3 0 1 9 opamp
*E3 1 0 9 0 -1.0E5

xe4 11 3 10 opamp
*E4 3 0 10 11 -1.0E5
VIN 2 0 DC 0 AC 1
*.ac lin 100 2k 20k
.ac lin 100 2k 20k
.print vdb(1)
.end
