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

x1 1  2 4 opamp 
x2 2 4 3 opamp
x3 4 3 5 opamp
x4 3 5 8 opamp
r37 7 5 1.0
r38 6 0 1.0
r39 7 0 1.0
r40 6 8 1.0
e1 8 0 7 6 1e5
*x5 6 8 7 opamp

vin 1 0 dc 0 ac 1
.ac dec 10 1 1meg
.print vdb(8)
.end
