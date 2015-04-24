* file flp9th.sp----9th order low-pass filter
*
*   reference: jiri vlach and kishore singhal, 'computer
*              methods for circuit analysis and design',
*              van nostrand reinhold co., 1983, pages 142
*              and 494 to 496.
*
*  pole/zero analysis and using vcvs as an ideal op-amp.
*  for just pole/zero analysis .ac statement is not required.


.SUBCKT opamp  1 2 24
R1 10 0 1e+3
R2 9 0 50e+3
R3 11 0  1e+3
R4 12 0  3e+3
R5 15 17 39e+3
R6 23 24 50
R7 24 25 25
R8 18 0  100
R9 14 0  50e+3
R10 21 20 40e+3
R11 13 0  50e+3
COMP 22 8 30e-12
*
*Q1 3 1 4 BNP1
*Rx1 1 29 6.7114e+2
Rpi1 1 4 4.6083e+5
Cpi1 1 4 1.64e-12
Cmu1 1 3 1.14e-13
Gm1 3 4 1 4 4.82e-4
*
*Q2 3 0 5 BNP1
*Rx2 0 30 6.7114e+2
Rpi2 2 5 4.5872e+5
Cpi2 2 5 1.64e-12
Cmu2 2 3 1.14e-13
Gm2 3 5 2 5 4.84e-4
*
*Q3 7 6 4 BPN1
*Rx3 6 31 5.0000e+2
Rpi3 6 4 1.8415e+5
Cpi3 6 4 1.3e-11
Cmu3 6 7 3.45e-13
Gm3 7 4 6 4 4.66e-4
*
*Q4 8 6 5 BPN1
*Rx4 6 32 5.0000e+2
Rpi4 6 5 1.8283e+5
Cpi4 6 5 1.3e-11
Cmu4 6 8 3.47e-13
Gm4 8 5 6 5 4.67e-4
*here
*Q5 7 9 10 BNP1
*Rx5 9 33 6.7114e+2
Rpi5 9 10 4.3290e+5
Cpi5 9 10 1.63e-12
Cmu5 9 7 2.73e-13
Gm5 7 10 9 10 4.77e-4
*
*Q6 8 9 11 BNP1
*Rx6 9 34 6.7114e+2
Rpi6 9 11 4.3290e+5
Cpi6 9 11 1.63e-12
Cmu6 9 8 2.59e-13
Gm6 8 11 9 11 4.77e-4
*
*Q7 0  7 9 BNP1
*Rx7 7 35 6.7114e+2
Rpi7 7 9 5.1020e+5
Cpi7 7 9 1.62e-12
Cmu7 7 0  9.08e-14
Gm7 0  9 7 9 4.7e-4
*
*Q8 3 3 0  BPNC1
*Rx8 3 36 5.0000e+2
Rpi8 3 0  7.2464e+4
Cpi8 3 0  2.41e-11
Cmu8 3 3 1.05e-12
Gm8 3 0  3 0  8.73e-4
*
*Q9 6 3 0  BPN1
*Rx9 3 37 5.0000e+2
Rpi9 3 0  7.2464e+4
Cpi9 3 0  3.09e-11
Cmu9 3 6 3.23e-13
Gm9 6 0  3 0  1.12e-3
*
*Q10 6 15 12 BNP1
*Rx10 15 38 6.7114e+2
Rpi10 15 12 1.7986e+5
Cpi10 15 12 2.49e-12
Cmu10 15 6 1.17e-13
Gm10 6 12 15 12 1.2e-3
*
*Q11 15 15 0  BNP1
*Rx11 15 39 6.7114e+2
Rpi11 15 0  5.1020e+3
Cpi11 15 0  2.59e-11
Cmu11 15 15 4.46e-13
Gm11 15 0  15 0  2.14e-2
*
*Q12 17 17 0  BPN1
*Rx12 17 40 5.0000e+2
Rpi12 17 0  1.1806e+3
Cpi12 17 0  3.46e-10
Cmu12 17 17 1.11e-12
Gm12 17 0  17 0  1.26e-2
*
*Q13A 28 17 0  BPN3
*Rx13A 17 41 1.0000e+2
Rpi13A 17 0  2.8329e+2
Cpi13A 17 0  1.23e-9
Cmu13A 17 28 7.83e-14
Gm13A 28 0  17 0  4.5e-2
*
*Q13B 22 17 0  BPN4
*Rx13B 17 42 1.6000e+2
Rpi13B 17 0  3.4130e+2
Cpi13B 17 0  3.06e-10
Cmu13B 17 22 2.28e-13
Gm13B 22 0  17 0  1.12e-2
*
*Q14 0  28 23 BNP2
*Rx14 28 43 1.8484e+2
Rpi14 28 23 3.2468e+4
Cpi14 28 23 1.48e-11
Cmu14 28 0  4.0e-13
Gm14 0  23 28 23 1.28e-2
*
*Q15 28 23 24 BNP1
*Rx15 23 44 6.7114e+2
Rpi15 23 28 1.9084e+14
Cpi15 23 28 6.56e-13
Cmu15 23 24 2.63e-13
Gm15 24 28 23 28 8.87e-14
*
*Q16 0  8 14 BNP1
*Rx16 8 45 6.7114e+2
Rpi16 8 14 3.0395e+5
Cpi16 8 14 1.99e-12
Cmu16 8 0  9.1e-14
Gm16 0  14 8 14 7.8e-4
*
*Q17 22 14 18 BNP1
*Rx17 14 46 6.7114e+2
Rpi17 14 18 5.3763e+3
Cpi17 14 18 2.51e-11
Cmu17 14 22 4.06e-13
Gm17 22 18 14 18 2.08e-2
*
*Q18 28 21 20 BNP1
*Rx18 21 47 6.7114e+2
Rpi18 21 20 2.2173e+3
Cpi18 21 20 4.21e-11
Cmu18 21 28 3.15e-13
Gm18 28 20 21 20 3.55e-2
*
*Q19 28 28 21 BNP1
*Rx19 28 48 6.7114e+2
Rpi19 28 21 1.7668e+5
Cpi19 28 21 2.41e-12
Cmu19 28 28 3.62e-13
Gm19 28 21 28 21 1.14e-3
*
*Q20 0  20 25 BPN2
*Rx20 20 49 8.0000e+1
Rpi20 20 25 5.9880e+3
Cpi20 20 25 2.69e-10
Cmu20 20 0  1.97e-12
Gm20 0  25 20 25 9.6e-3
*
*Q21 13 25 24 BPN1
*Rx21 25 50 5.0000e+2
Rpi21 25 24 6.4103e+13
Cpi21 25 24 1.01e-13
Cmu21 25 25 5.64e-13
Gm21 25 24 25 24 1.3e-13
*
*Q22 8 13 0  BNP1
*Rx22 13 51 6.7114e+2
Rpi22 13 0  1.9920e+14
Cpi22 13 0  6.5e-13
Cmu22 13 8 2.26e-13
Gm22 8 0  13 0  4.04e-14
*
*Q23A 0  22 20 BPN5
*Rx23A 22 52 1.1001e+3
Rpi23A 22 20 1.1976e+2
Cpi23A 22 20 6.11e-10
Cmu23A 22 0  1.91e-12
Gm23A 0  20 22 20 2.3e-2
*
*Q23B 0  22 8 BPN6
*Rx23B 22 53 6.4935e+2
Rpi23B 22 8 8.3333e+7
Cpi23B 22 8 2.71e-12
Cmu23B 22 0  1.92e-12
Gm23B 0  8 22 8 2.29e-7
*
*Q24 13 13 0  BNP1
*Rx24 13 54 6.7114e+2
Rpi24 13 0  1.9920e+14
Cpi24 13 0  6.50e-13
Cmu24 13 13 3.6e-13
Gm24 13 0  13 0  7.33e-19
.ends
*
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
*.print vm(20) vm(10) vp(20)
.print vdb(20)
*.graph ac vdb(20,10) par('db(vm(20)/vm(10))')
.options post=2 dcstep=1e3 x0r=-1.23456e+3 x1r=-1.23456e+2 x2r=1.23456e+3 fscal=1e-6 gscal=1e3 cscal=1e9 lscal=1e3
.end
