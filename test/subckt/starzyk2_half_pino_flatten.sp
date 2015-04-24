* Testing Starzyk's example
* Testing opamps
* 
*



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

* circuit 5
r37 12 31 1.0
r38 32 0 1.0
r39 31 0 1.0
r40 32 19 1.0

*Expand xo1(opamp) ...
gm1a_xo1 104 103 8 103 5.89e-06
rds1a_xo1 104 103 9.26e+07
cbd1a_xo1 103 104 2.04e-14
cgs1a_xo1 8 103 4.78e-14
gm1b_xo1 105 103 2 103 5.89e-06
rds1b_xo1 105 103 9.26e+07
cbd1b_xo1 103 105 2.04e-14
cgs1b_xo1 2 103 4.78e-14
gm2a_xo1 104 0 104 0 3e-05
rds2a_xo1 104 0 2.46e+07
cbd2a+cgs2a+cgs2b_xo1 0 104 1.1164e-12
gm2b_xo1 105 0 104 0 3e-05
rds2b_xo1 105 0 2.46e+07
cbd2b+cgs3_xo1 0 105 2.174e-13
gm3_xo1 3 0 105 0 0.000406
rds3+rds4+rl_xo1 3 0 3469.05
cbd3+cbd4+cgd4+cl_xo1 0 3 1.04677e-11
rds5_xo1 103 0 4.695e+07
cbd5_xo1 0 103 2.45e-14
cc_xo1 105 3 1e-12
*Expand xo2(opamp) ...
gm1a_xo2 204 203 4 203 5.89e-06
rds1a_xo2 204 203 9.26e+07
cbd1a_xo2 203 204 2.04e-14
cgs1a_xo2 4 203 4.78e-14
gm1b_xo2 205 203 0 203 5.89e-06
rds1b_xo2 205 203 9.26e+07
cbd1b_xo2 203 205 2.04e-14
cgs1b_xo2 0 203 4.78e-14
gm2a_xo2 204 0 204 0 3e-05
rds2a_xo2 204 0 2.46e+07
cbd2a+cgs2a+cgs2b_xo2 0 204 1.1164e-12
gm2b_xo2 205 0 204 0 3e-05
rds2b_xo2 205 0 2.46e+07
cbd2b+cgs3_xo2 0 205 2.174e-13
gm3_xo2 5 0 205 0 0.000406
rds3+rds4+rl_xo2 5 0 3469.05
cbd3+cbd4+cgd4+cl_xo2 0 5 1.04677e-11
rds5_xo2 203 0 4.695e+07
cbd5_xo2 0 203 2.45e-14
cc_xo2 205 5 1e-12
*Expand xo3(opamp) ...
gm1a_xo3 304 303 6 303 5.89e-06
rds1a_xo3 304 303 9.26e+07
cbd1a_xo3 303 304 2.04e-14
cgs1a_xo3 6 303 4.78e-14
gm1b_xo3 305 303 0 303 5.89e-06
rds1b_xo3 305 303 9.26e+07
cbd1b_xo3 303 305 2.04e-14
cgs1b_xo3 0 303 4.78e-14
gm2a_xo3 304 0 304 0 3e-05
rds2a_xo3 304 0 2.46e+07
cbd2a+cgs2a+cgs2b_xo3 0 304 1.1164e-12
gm2b_xo3 305 0 304 0 3e-05
rds2b_xo3 305 0 2.46e+07
cbd2b+cgs3_xo3 0 305 2.174e-13
gm3_xo3 7 0 305 0 0.000406
rds3+rds4+rl_xo3 7 0 3469.05
cbd3+cbd4+cgd4+cl_xo3 0 7 1.04677e-11
rds5_xo3 303 0 4.695e+07
cbd5_xo3 0 303 2.45e-14
cc_xo3 305 7 1e-12
*Expand xo4(opamp) ...
gm1a_xo4 404 403 15 403 5.89e-06
rds1a_xo4 404 403 9.26e+07
cbd1a_xo4 403 404 2.04e-14
cgs1a_xo4 15 403 4.78e-14
gm1b_xo4 405 403 9 403 5.89e-06
rds1b_xo4 405 403 9.26e+07
cbd1b_xo4 403 405 2.04e-14
cgs1b_xo4 9 403 4.78e-14
gm2a_xo4 404 0 404 0 3e-05
rds2a_xo4 404 0 2.46e+07
cbd2a+cgs2a+cgs2b_xo4 0 404 1.1164e-12
gm2b_xo4 405 0 404 0 3e-05
rds2b_xo4 405 0 2.46e+07
cbd2b+cgs3_xo4 0 405 2.174e-13
gm3_xo4 10 0 405 0 0.000406
rds3+rds4+rl_xo4 10 0 3469.05
cbd3+cbd4+cgd4+cl_xo4 0 10 1.04677e-11
rds5_xo4 403 0 4.695e+07
cbd5_xo4 0 403 2.45e-14
cc_xo4 405 10 1e-12
*Expand xo5(opamp) ...
gm1a_xo5 504 503 11 503 5.89e-06
rds1a_xo5 504 503 9.26e+07
cbd1a_xo5 503 504 2.04e-14
cgs1a_xo5 11 503 4.78e-14
gm1b_xo5 505 503 0 503 5.89e-06
rds1b_xo5 505 503 9.26e+07
cbd1b_xo5 503 505 2.04e-14
cgs1b_xo5 0 503 4.78e-14
gm2a_xo5 504 0 504 0 3e-05
rds2a_xo5 504 0 2.46e+07
cbd2a+cgs2a+cgs2b_xo5 0 504 1.1164e-12
gm2b_xo5 505 0 504 0 3e-05
rds2b_xo5 505 0 2.46e+07
cbd2b+cgs3_xo5 0 505 2.174e-13
gm3_xo5 12 0 505 0 0.000406
rds3+rds4+rl_xo5 12 0 3469.05
cbd3+cbd4+cgd4+cl_xo5 0 12 1.04677e-11
rds5_xo5 503 0 4.695e+07
cbd5_xo5 0 503 2.45e-14
cc_xo5 505 12 1e-12
*Expand xo6(opamp) ...
gm1a_xo6 604 603 13 603 5.89e-06
rds1a_xo6 604 603 9.26e+07
cbd1a_xo6 603 604 2.04e-14
cgs1a_xo6 13 603 4.78e-14
gm1b_xo6 605 603 0 603 5.89e-06
rds1b_xo6 605 603 9.26e+07
cbd1b_xo6 603 605 2.04e-14
cgs1b_xo6 0 603 4.78e-14
gm2a_xo6 604 0 604 0 3e-05
rds2a_xo6 604 0 2.46e+07
cbd2a+cgs2a+cgs2b_xo6 0 604 1.1164e-12
gm2b_xo6 605 0 604 0 3e-05
rds2b_xo6 605 0 2.46e+07
cbd2b+cgs3_xo6 0 605 2.174e-13
gm3_xo6 14 0 605 0 0.000406
rds3+rds4+rl_xo6 14 0 3469.05
cbd3+cbd4+cgd4+cl_xo6 0 14 1.04677e-11
rds5_xo6 603 0 4.695e+07
cbd5_xo6 0 603 2.45e-14
cc_xo6 605 14 1e-12
*Expand xo13(opamp) ...
gm1a_xo13 704 703 32 703 5.89e-06
rds1a_xo13 704 703 9.26e+07
cbd1a_xo13 703 704 2.04e-14
cgs1a_xo13 32 703 4.78e-14
gm1b_xo13 705 703 31 703 5.89e-06
rds1b_xo13 705 703 9.26e+07
cbd1b_xo13 703 705 2.04e-14
cgs1b_xo13 31 703 4.78e-14
gm2a_xo13 704 0 704 0 3e-05
rds2a_xo13 704 0 2.46e+07
cbd2a+cgs2a+cgs2b_xo13 0 704 1.1164e-12
gm2b_xo13 705 0 704 0 3e-05
rds2b_xo13 705 0 2.46e+07
cbd2b+cgs3_xo13 0 705 2.174e-13
gm3_xo13 19 0 705 0 0.000406
rds3+rds4+rl_xo13 19 0 3469.05
cbd3+cbd4+cgd4+cl_xo13 0 19 1.04677e-11
rds5_xo13 703 0 4.695e+07
cbd5_xo13 0 703 2.45e-14
cc_xo13 705 19 1e-12

vin 1 0 dc 0 ac 1
.ac dec 100 1 10000meg
.print vdb(19)
.end


