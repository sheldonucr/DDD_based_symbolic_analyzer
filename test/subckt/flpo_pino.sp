* file flp9th.sp----9th order low-pass filter
*
*   reference: jiri vlach and kishore singhal, 'computer
*              methods for circuit analysis and design',
*              van nostrand reinhold co., 1983, pages 142
*              and 494 to 496.
*
*  pole/zero analysis and using vcvs as an ideal op-amp.
*  for just pole/zero analysis .ac statement is not required.



*.subckt opamp in+ in- out
.subckt opamp 8 7 6
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

.subckt fdnr 1 
r1 1 2 2k
c1 2 3 12n
r2 3 4 3.3k
r3 4 5 3.3k
r4 5 6 4.5k
c2 6 0 10n
xop1 2 4 5 opamp
xop2 6 4 3 opamp
*eop1 5 0  2 4   1000 
*eop2 3 0  6 4   1000
.ends
*

rs 10 1 5.4779k
r12 1 2 4.44k
r23 2 3 3.2201k
r34 3 4 3.63678k
r45 4 20 1.2201k
c5 20 0 10n
x1 1 fdnr 
x2 2 fdnr 
x3 3 fdnr 
x4 4 fdnr 

vin 10 0 dc 0 ac 1
.ac dec 200 1 10k
*.tran 1ns 10us
*.print vm(20) vm(10) vp(20)
.print vdb(20)
*.graph ac vdb(20,10) par('db(vm(20)/vm(10))')
.options post=2 dcstep=1e3 x0r=-1.23456e+3 x1r=-1.23456e+2 x2r=1.23456e+3 fscal=1e-6 gscal=1e3 cscal=1e9 lscal=1e3
.end
