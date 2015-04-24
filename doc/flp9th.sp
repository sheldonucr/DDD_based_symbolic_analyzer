* file flp9th.sp----9th order low-pass filter
*
*   reference: jiri vlach and kishore singhal, 'computer
*              methods for circuit analysis and design',
*              van nostrand reinhold co., 1983, pages 142
*              and 494 to 496.
*
*  pole/zero analysis and using vcvs as an ideal op-amp.
*  for just pole/zero analysis .ac statement is not required.

.subckt fdnr 1 r1=2k c1=12n r4=4.5k
r1 1 2 r1
c1 2 3 c1
r2 3 4 3.3k
r3 4 5 3.3k
r4 5 6 r4
c2 6 0 10n
eop1 5 0  2 4   1000 
eop2 3 0  6 4   1000
.ends
*
rs 10 1 5.4779k
r12 1 2 4.44k
r23 2 3 3.2201k
r34 3 4 3.63678k
r45 4 20 1.2201k
c5 20 0 10n
x1 1 fdnr r1=2.0076k  c1=12n    r4=4.5898k
x2 2 fdnr r1=5.9999k  c1=6.8n   r4=4.25725k
x3 3 fdnr r1=5.88327k c1=4.7n   r4=5.62599k
x4 4 fdnr r1=1.0301k  c1=6.8n   r4=5.808498k

vin 10 0 ac 1
.ac dec 100 1 100k
*.print vm(20) vm(10) vp(20)
.print vm(20)
*.graph ac vdb(20,10) par('db(vm(20)/vm(10))')
.options post=2 dcstep=1e3 x0r=-1.23456e+3 x1r=-1.23456e+2 x2r=1.23456e+3 fscal=1e-6 gscal=1e3 cscal=1e9 lscal=1e3
.end
