* file flp9th.sp----9th order low-pass filter
*
*   reference: jiri vlach and kishore singhal, 'computer
*              methods for circuit analysis and design',
*              van nostrand reinhold co., 1983, pages 142
*              and 494 to 496.
*
*  pole/zero analysis and using vcvs as an ideal op-amp.
*  for just pole/zero analysis .ac statement is not required.


rs 10 1 5.4779k
r12 1 2 4.44k
r23 2 3 3.2201k
r34 3 4 3.63678k
r45 4 20 1.2201k
c5 20 0 10n

r1_x1 1 102 2000
c1_x1 102 103 1.2e-08
r2_x1 103 104 3300
r3_x1 104 105 3300
r4_x1 105 106 4500
c2_x1 106 0 1e-08
gm1a_xop1_x1 204 203 104 203 5.89e-06
rds1a_xop1_x1 204 203 9.26e+07
cbd1a_xop1_x1 203 204 2.04e-14
cgs1a_xop1_x1 104 203 4.78e-14
gm1b_xop1_x1 205 203 102 203 5.89e-06
rds1b_xop1_x1 205 203 9.26e+07
cbd1b_xop1_x1 203 205 2.04e-14
cgs1b_xop1_x1 102 203 4.78e-14
gm2a_xop1_x1 204 0 204 0 3e-05
rds2a_xop1_x1 204 0 2.46e+07
cbd2a+cgs2a+cgs2b_xop1_x1 0 204 1.1164e-12
gm2b_xop1_x1 205 0 204 0 3e-05
rds2b_xop1_x1 205 0 2.46e+07
cbd2b+cgs3_xop1_x1 0 205 2.174e-13
gm3_xop1_x1 105 0 205 0 0.000406
rds3+rds4+rl_xop1_x1 105 0 3469.05
cbd3+cbd4+cgd4+cl_xop1_x1 0 105 1.04677e-11
rds5_xop1_x1 203 0 4.695e+07
cbd5_xop1_x1 0 203 2.45e-14
cc_xop1_x1 205 105 1e-12
gm1a_xop2_x1 304 303 104 303 5.89e-06
rds1a_xop2_x1 304 303 9.26e+07
cbd1a_xop2_x1 303 304 2.04e-14
cgs1a_xop2_x1 104 303 4.78e-14
gm1b_xop2_x1 305 303 106 303 5.89e-06
rds1b_xop2_x1 305 303 9.26e+07
cbd1b_xop2_x1 303 305 2.04e-14
cgs1b_xop2_x1 106 303 4.78e-14
gm2a_xop2_x1 304 0 304 0 3e-05
rds2a_xop2_x1 304 0 2.46e+07
cbd2a+cgs2a+cgs2b_xop2_x1 0 304 1.1164e-12
gm2b_xop2_x1 305 0 304 0 3e-05
rds2b_xop2_x1 305 0 2.46e+07
cbd2b+cgs3_xop2_x1 0 305 2.174e-13
gm3_xop2_x1 103 0 305 0 0.000406
rds3+rds4+rl_xop2_x1 103 0 3469.05
cbd3+cbd4+cgd4+cl_xop2_x1 0 103 1.04677e-11
rds5_xop2_x1 303 0 4.695e+07
cbd5_xop2_x1 0 303 2.45e-14
cc_xop2_x1 305 103 1e-12
r1_x2 2 202 2000
c1_x2 202 203 1.2e-08
r2_x2 203 204 3300
r3_x2 204 205 3300
r4_x2 205 206 4500
c2_x2 206 0 1e-08
gm1a_xop1_x2 304 303 204 303 5.89e-06
rds1a_xop1_x2 304 303 9.26e+07
cbd1a_xop1_x2 303 304 2.04e-14
cgs1a_xop1_x2 204 303 4.78e-14
gm1b_xop1_x2 305 303 202 303 5.89e-06
rds1b_xop1_x2 305 303 9.26e+07
cbd1b_xop1_x2 303 305 2.04e-14
cgs1b_xop1_x2 202 303 4.78e-14
gm2a_xop1_x2 304 0 304 0 3e-05
rds2a_xop1_x2 304 0 2.46e+07
cbd2a+cgs2a+cgs2b_xop1_x2 0 304 1.1164e-12
gm2b_xop1_x2 305 0 304 0 3e-05
rds2b_xop1_x2 305 0 2.46e+07
cbd2b+cgs3_xop1_x2 0 305 2.174e-13
gm3_xop1_x2 205 0 305 0 0.000406
rds3+rds4+rl_xop1_x2 205 0 3469.05
cbd3+cbd4+cgd4+cl_xop1_x2 0 205 1.04677e-11
rds5_xop1_x2 303 0 4.695e+07
cbd5_xop1_x2 0 303 2.45e-14
cc_xop1_x2 305 205 1e-12
gm1a_xop2_x2 404 403 204 403 5.89e-06
rds1a_xop2_x2 404 403 9.26e+07
cbd1a_xop2_x2 403 404 2.04e-14
cgs1a_xop2_x2 204 403 4.78e-14
gm1b_xop2_x2 405 403 206 403 5.89e-06
rds1b_xop2_x2 405 403 9.26e+07
cbd1b_xop2_x2 403 405 2.04e-14
cgs1b_xop2_x2 206 403 4.78e-14
gm2a_xop2_x2 404 0 404 0 3e-05
rds2a_xop2_x2 404 0 2.46e+07
cbd2a+cgs2a+cgs2b_xop2_x2 0 404 1.1164e-12
gm2b_xop2_x2 405 0 404 0 3e-05
rds2b_xop2_x2 405 0 2.46e+07
cbd2b+cgs3_xop2_x2 0 405 2.174e-13
gm3_xop2_x2 203 0 405 0 0.000406
rds3+rds4+rl_xop2_x2 203 0 3469.05
cbd3+cbd4+cgd4+cl_xop2_x2 0 203 1.04677e-11
rds5_xop2_x2 403 0 4.695e+07
cbd5_xop2_x2 0 403 2.45e-14
cc_xop2_x2 405 203 1e-12
r1_x3 3 302 2000
c1_x3 302 303 1.2e-08
r2_x3 303 304 3300
r3_x3 304 305 3300
r4_x3 305 306 4500
c2_x3 306 0 1e-08
gm1a_xop1_x3 404 403 304 403 5.89e-06
rds1a_xop1_x3 404 403 9.26e+07
cbd1a_xop1_x3 403 404 2.04e-14
cgs1a_xop1_x3 304 403 4.78e-14
gm1b_xop1_x3 405 403 302 403 5.89e-06
rds1b_xop1_x3 405 403 9.26e+07
cbd1b_xop1_x3 403 405 2.04e-14
cgs1b_xop1_x3 302 403 4.78e-14
gm2a_xop1_x3 404 0 404 0 3e-05
rds2a_xop1_x3 404 0 2.46e+07
cbd2a+cgs2a+cgs2b_xop1_x3 0 404 1.1164e-12
gm2b_xop1_x3 405 0 404 0 3e-05
rds2b_xop1_x3 405 0 2.46e+07
cbd2b+cgs3_xop1_x3 0 405 2.174e-13
gm3_xop1_x3 305 0 405 0 0.000406
rds3+rds4+rl_xop1_x3 305 0 3469.05
cbd3+cbd4+cgd4+cl_xop1_x3 0 305 1.04677e-11
rds5_xop1_x3 403 0 4.695e+07
cbd5_xop1_x3 0 403 2.45e-14
cc_xop1_x3 405 305 1e-12
gm1a_xop2_x3 504 503 304 503 5.89e-06
rds1a_xop2_x3 504 503 9.26e+07
cbd1a_xop2_x3 503 504 2.04e-14
cgs1a_xop2_x3 304 503 4.78e-14
gm1b_xop2_x3 505 503 306 503 5.89e-06
rds1b_xop2_x3 505 503 9.26e+07
cbd1b_xop2_x3 503 505 2.04e-14
cgs1b_xop2_x3 306 503 4.78e-14
gm2a_xop2_x3 504 0 504 0 3e-05
rds2a_xop2_x3 504 0 2.46e+07
cbd2a+cgs2a+cgs2b_xop2_x3 0 504 1.1164e-12
gm2b_xop2_x3 505 0 504 0 3e-05
rds2b_xop2_x3 505 0 2.46e+07
cbd2b+cgs3_xop2_x3 0 505 2.174e-13
gm3_xop2_x3 303 0 505 0 0.000406
rds3+rds4+rl_xop2_x3 303 0 3469.05
cbd3+cbd4+cgd4+cl_xop2_x3 0 303 1.04677e-11
rds5_xop2_x3 503 0 4.695e+07
cbd5_xop2_x3 0 503 2.45e-14
cc_xop2_x3 505 303 1e-12
r1_x4 4 402 2000
c1_x4 402 403 1.2e-08
r2_x4 403 404 3300
r3_x4 404 405 3300
r4_x4 405 406 4500
c2_x4 406 0 1e-08
gm1a_xop1_x4 504 503 404 503 5.89e-06
rds1a_xop1_x4 504 503 9.26e+07
cbd1a_xop1_x4 503 504 2.04e-14
cgs1a_xop1_x4 404 503 4.78e-14
gm1b_xop1_x4 505 503 402 503 5.89e-06
rds1b_xop1_x4 505 503 9.26e+07
cbd1b_xop1_x4 503 505 2.04e-14
cgs1b_xop1_x4 402 503 4.78e-14
gm2a_xop1_x4 504 0 504 0 3e-05
rds2a_xop1_x4 504 0 2.46e+07
cbd2a+cgs2a+cgs2b_xop1_x4 0 504 1.1164e-12
gm2b_xop1_x4 505 0 504 0 3e-05
rds2b_xop1_x4 505 0 2.46e+07
cbd2b+cgs3_xop1_x4 0 505 2.174e-13
gm3_xop1_x4 405 0 505 0 0.000406
rds3+rds4+rl_xop1_x4 405 0 3469.05
cbd3+cbd4+cgd4+cl_xop1_x4 0 405 1.04677e-11
rds5_xop1_x4 503 0 4.695e+07
cbd5_xop1_x4 0 503 2.45e-14
cc_xop1_x4 505 405 1e-12
gm1a_xop2_x4 604 603 404 603 5.89e-06
rds1a_xop2_x4 604 603 9.26e+07
cbd1a_xop2_x4 603 604 2.04e-14
cgs1a_xop2_x4 404 603 4.78e-14
gm1b_xop2_x4 605 603 406 603 5.89e-06
rds1b_xop2_x4 605 603 9.26e+07
cbd1b_xop2_x4 603 605 2.04e-14
cgs1b_xop2_x4 406 603 4.78e-14
gm2a_xop2_x4 604 0 604 0 3e-05
rds2a_xop2_x4 604 0 2.46e+07
cbd2a+cgs2a+cgs2b_xop2_x4 0 604 1.1164e-12
gm2b_xop2_x4 605 0 604 0 3e-05
rds2b_xop2_x4 605 0 2.46e+07
cbd2b+cgs3_xop2_x4 0 605 2.174e-13
gm3_xop2_x4 403 0 605 0 0.000406
rds3+rds4+rl_xop2_x4 403 0 3469.05
cbd3+cbd4+cgd4+cl_xop2_x4 0 403 1.04677e-11
rds5_xop2_x4 603 0 4.695e+07
cbd5_xop2_x4 0 603 2.45e-14
cc_xop2_x4 605 403 1e-12

vin 10 0 dc 0 ac 1
.ac dec 200 1 10k
*.tran 1ns 10us
*.print vm(20) vm(10) vp(20)
.print vdb(20)
*.graph ac vdb(20,10) par('db(vm(20)/vm(10))')
.options post=2 dcstep=1e3 x0r=-1.23456e+3 x1r=-1.23456e+2 x2r=1.23456e+3 fscal=1e-6 gscal=1e3 cscal=1e9 lscal=1e3
.end
