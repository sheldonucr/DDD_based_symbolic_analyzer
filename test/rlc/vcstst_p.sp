* Example 3 from Rutkowski's paper 6/88
* Testing controlled voltage sources 
* 
*

*r0 8 0 100k
c1 0 8 1
c2 2 8 1
c3 70 8 1
c4 1 2 1
*r2 2 0 100k

r5 0 3 1
r6 20 9 1
e7 70 10 20 9  2
r8 0 5 1
r9 5 6 1
r17 20 3 1
h10 0 1 r17    2
*e10 0 1 20 3 10
*g10 0 1 20 3 2 
r11 1 3 1
r12 0 20 1
r13 0 6 1
r14 0 70 1
r15 5 70 1
r16 6 70 1
r18 20 70 1
r19 70 9 1
r20 20 10 1
r21 9 10 1
vin 10 0 dc 0 ac 1
.ac dec 1000 1 10
*.ac dec 2 1 100meg 
.print vdb(8)
.end
