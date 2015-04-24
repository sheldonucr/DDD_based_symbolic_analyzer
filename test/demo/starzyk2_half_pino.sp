* Testing Starzyk's example
* Testing opamps
* 
*

* format for opamp
* opamp -node out +node

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
gm3  6 0 5 0 0.000406
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

* circuit 1
r1 1 2 1.0 
r2 12 8 1.0 
r3 2 0 1.0
r4 8 3 1.0
r5 3 4 1.0
r6 4 5 1.0
c6 4 5 1.0
R7 5 6 1.0
c8 6 7 1.0
r9 7 8 1.0
xo1 8 3 2 opamp
xo2 4 5 0 opamp
xo3 6 7 0 opamp

* circuit 2
r10 5 9 1.0 
r11 19 15 1.0 
r12 9 0 1.0
r13 10 15 1.0
r14 10 11 1.0
r15 11 12 1.0
c15 11 12 1.0
r16 12 13 1.0
c17 13 14 1.0
r18 14 15 1.0
xo4 15 10 9 opamp
xo5 11 12 0 opamp
xo6 13 14 0 opamp

* circuit 5
r37 12 31 1.0
r38 32 0 1.0
r39 31 0 1.0
r40 32 19 1.0
xo13 32 19 31 opamp

vin 1 0 dc 0 ac 1
.ac dec 100 1 10000meg
.print vdb(19)
.end


