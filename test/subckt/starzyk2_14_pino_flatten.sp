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
*Expand xo13(opamp) ...
gm1a_xo13 404 403 32 403 5.89e-06
rds1a_xo13 404 403 9.26e+07
cbd1a_xo13 403 404 2.04e-14
cgs1a_xo13 32 403 4.78e-14
gm1b_xo13 405 403 31 403 5.89e-06
rds1b_xo13 405 403 9.26e+07
cbd1b_xo13 403 405 2.04e-14
cgs1b_xo13 31 403 4.78e-14
gm2a_xo13 404 0 404 0 3e-05
rds2a_xo13 404 0 2.46e+07
cbd2a+cgs2a+cgs2b_xo13 0 404 1.1164e-12
gm2b_xo13 405 0 404 0 3e-05
rds2b_xo13 405 0 2.46e+07
cbd2b+cgs3_xo13 0 405 2.174e-13
gm3_xo13 12 0 405 0 0.000406
rds3+rds4+rl_xo13 12 0 3469.05
cbd3+cbd4+cgd4+cl_xo13 0 12 1.04677e-11
rds5_xo13 403 0 4.695e+07
cbd5_xo13 0 403 2.45e-14
cc_xo13 405 12 1e-12

* circuit 5
r37 5 31 1.0
r38 32 0 1.0
r39 31 0 1.0
r40 32 12 1.0

vin 1 0 dc 0 ac 1
.ac dec 100 1 10000meg
.print vdb(12)
.end


