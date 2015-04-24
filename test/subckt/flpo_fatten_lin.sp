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
rii_xop1_x1 102 104 2e+06
ri1_xop1_x1 102 0 5e+08
ri2_xop1+ri2_xop2_x1 104 0 2.5e+08
g1_xop1_x1 201 0 102 104 2
c1_xop1_x1 201 0 2.66e-05
r1_xop1_x1 201 0 1000
g2_xop1_x1 105 0 201 0 1.33333
rld_xop1_x1 105 0 75
rii_xop2_x1 106 104 2e+06
ri1_xop2_x1 106 0 5e+08
g1_xop2_x1 301 0 106 104 2
c1_xop2_x1 301 0 2.66e-05
r1_xop2_x1 301 0 1000
g2_xop2_x1 103 0 301 0 1.33333
rld_xop2_x1 103 0 75

r1_x2 2 202 2000
c1_x2 202 203 1.2e-08
r2_x2 203 204 3300
r3_x2 204 205 3300
r4_x2 205 206 4500
c2_x2 206 0 1e-08
rii_xop1_x2 202 204 2e+06
ri1_xop1_x2 202 0 5e+08
ri2_xop1+ri2_xop2_x2 204 0 2.5e+08
g1_xop1_x2 301 0 202 204 2
c1_xop1_x2 301 0 2.66e-05
r1_xop1_x2 301 0 1000
g2_xop1_x2 205 0 301 0 1.33333
rld_xop1_x2 205 0 75
rii_xop2_x2 206 204 2e+06
ri1_xop2_x2 206 0 5e+08
g1_xop2_x2 401 0 206 204 2
c1_xop2_x2 401 0 2.66e-05
r1_xop2_x2 401 0 1000
g2_xop2_x2 203 0 401 0 1.33333
rld_xop2_x2 203 0 75

r1_x3 3 302 2000
c1_x3 302 303 1.2e-08
r2_x3 303 304 3300
r3_x3 304 305 3300
r4_x3 305 306 4500
c2_x3 306 0 1e-08
rii_xop1_x3 302 304 2e+06
ri1_xop1_x3 302 0 5e+08
ri2_xop1+ri2_xop2_x3 304 0 2.5e+08
g1_xop1_x3 401 0 302 304 2
c1_xop1_x3 401 0 2.66e-05
r1_xop1_x3 401 0 1000
g2_xop1_x3 305 0 401 0 1.33333
rld_xop1_x3 305 0 75
rii_xop2_x3 306 304 2e+06
ri1_xop2_x3 306 0 5e+08
g1_xop2_x3 501 0 306 304 2
c1_xop2_x3 501 0 2.66e-05
r1_xop2_x3 501 0 1000
g2_xop2_x3 303 0 501 0 1.33333
rld_xop2_x3 303 0 75

r1_x4 4 402 2000
c1_x4 402 403 1.2e-08
r2_x4 403 404 3300
r3_x4 404 405 3300
r4_x4 405 406 4500
c2_x4 406 0 1e-08
rii_xop1_x4 402 404 2e+06
ri1_xop1_x4 402 0 5e+08
ri2_xop1+ri2_xop2_x4 404 0 2.5e+08
g1_xop1_x4 501 0 402 404 2
c1_xop1_x4 501 0 2.66e-05
r1_xop1_x4 501 0 1000
g2_xop1_x4 405 0 501 0 1.33333
rld_xop1_x4 405 0 75
rii_xop2_x4 406 404 2e+06
ri1_xop2_x4 406 0 5e+08
g1_xop2_x4 601 0 406 404 2
c1_xop2_x4 601 0 2.66e-05
r1_xop2_x4 601 0 1000
g2_xop2_x4 403 0 601 0 1.33333
rld_xop2_x4 403 0 75

vin 10 0 dc 0 ac 1
.ac dec 200 1 10k
*.print vm(20) vm(10) vp(20)
.print vdb(20)
*.graph ac vdb(20,10) par('db(vm(20)/vm(10))')
.options post=2 dcstep=1e3 x0r=-1.23456e+3 x1r=-1.23456e+2 x2r=1.23456e+3 fscal=1e-6 gscal=1e3 cscal=1e9 lscal=1e3
.end
