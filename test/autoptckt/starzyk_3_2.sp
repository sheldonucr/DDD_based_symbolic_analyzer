* Testing Starzyk's example
* Testing opamps
* 
*
* part: starzyk_3_1.part


*PINO-MILLER-OPAMP out(v(6))
*
* MILLER opamp, pMOS input pair, n-well technology
* ------------------------------------------------
*
* CIRCUIT DESCRIPTION
*
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
.ends

* circuit 1
.subckt blk3 1 12 11 15
r1 1 2 1.0 
r2 12 8 1.0 
r3 2 0 1.0
r4 8 3 1.0
R5 3 4 1.0
r6 4 5 1.0
c6 4 5 1.0
r7 5 6 1.0
c8 6 7 1.0
r9 7 8 1.0
xo1 8 3 2 opamp
xo2 4 5 0 opamp
xo3 6 7 0 opamp

r10 5 9 1.0 
r12 9 0 1.0
r13 10 15 1.0
r14 10 11 1.0
xo4 15 10 9 opamp
.ends

* circuit 1
.subckt blk1 23 22 12 11 15
r11 19 15 1.0 
r15 11 12 1.0
c15 11 12 1.0
r16 12 13 1.0
c17 13 14 1.0
r18 14 15 1.0
xo5 11 12 0 opamp
xo6 13 14 0 opamp

r19 12 16 1.0
r21 16 0 1.0
r22 22 17 1.0
r23 17 18 1.0
r24 18 19 1.0
c24 18 19 1.0
r25 19 20 1.0
c26 20 21 1.0
r27 21 22 1.0
xo7 22 17 16 opamp
xo8 18 19 0 opamp
xo9 20 21 0 opamp
r28 19 23 1.0
r30 23 0 1.0
.ends

* circuit 2
.subckt blk2 30 23 22
r20 22 26 1.0
* circuit 4
r29 29 30 1.0
r31 24 29 1.0
r32 24 25 1.0
r33 25 26 1.0
c33 25 26 1.0
r34 26 27 1.0
c35 27 28 1.0
r36 28 29 1.0
xo10 29 24 23 opamp
xo11 25 26 0 opamp
xo12 27 28 0 opamp
 
r37 26 31 1.0
r38 32 0 1.0
r39 31 0 1.0
r40 32 30 1.0
xo13 32 30 31 opamp
.ends

vin 1 0 dc 0 ac 1
xo1 23 22 12 11 15 blk1
xo2 30 23 22 blk2
xo3 1 12 11 15 blk3

.ac dec 1 1 100meg
.print vdb(30)
.end
