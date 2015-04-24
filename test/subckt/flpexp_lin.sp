* file flp9th.sp----9th order low-pass filter
*
*   reference: jiri vlach and kishore singhal, 'computer
*              methods for circuit analysis and design',
*              van nostrand reinhold co., 1983, pages 142
*              and 494 to 496.
*
*  pole/zero analysis and using vcvs as an ideal op-amp.
*  for just pole/zero analysis .ac statement is not required.

*interface in+ in- out
.subckt opamp 2 3 4
rii 2 3 2meg
ri1 2 0 500meg
ri2 3 0 500meg
g1 1 0 2 3 2
c1 1 0 26.6u
r1 1 0 1k
g2 4 0 1 0 1.33333
rld 4 0 75
.ends

.subckt fdnr1 1 
r1 1 2 2.0076k
c1 2 3 12n
r2 3 4 3.3k
r3 4 5 3.3k
r4 5 6 4.5898k
c2 6 0 10n
xop1 2 4 5 opamp
xop2 6 4 3 opamp
.ends
.subckt fdnr2 1 
r1 1 2 5.9999k
c1 2 3 6.8n
r2 3 4 3.3k
r3 4 5 3.3k
r4 5 6 4.25725k
c2 6 0 10n
xop1 2 4 5 opamp
xop2 6 4 3 opamp
.ends
.subckt fdnr3 1 
r1 1 2 5.88327k
c1 2 3 4.7n 
r2 3 4 3.3k
r3 4 5 3.3k
r4 5 6 5.62599k
c2 6 0 10n
xop1 2 4 5 opamp
xop2 6 4 3 opamp
.ends
.subckt fdnr4 1 
r1 1 2 1.0301k
c1 2 3 6.8n
r2 3 4 3.3k
r3 4 5 3.3k
r4 5 6 5.808498k
c2 6 0 10n
xop1 2 4 5 opamp
xop2 6 4 3 opamp
.ends
*
rs 10 1 5.4779k
r12 1 2 4.44k
r23 2 3 3.2201k
r34 3 4 3.63678k
r45 4 20 1.2201k
c5 20 0 10n
x1 1 fdnr1 
x2 2 fdnr2
x3 3 fdnr3 
x4 4 fdnr4 

vin 10 0 ac 1
.ac dec 100 1 100k
*.print vm(20) vm(10) vp(20)
.print vm(20)
*.graph ac vdb(20,10) par('db(vm(20)/vm(10))')
.options post=2 dcstep=1e3 x0r=-1.23456e+3 x1r=-1.23456e+2 x2r=1.23456e+3 fscal=1e-6 gscal=1e3 cscal=1e9 lscal=1e3
.end
