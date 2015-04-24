* Example 3 from Rutkowski's paper 6/88
* Testing controlled voltage sources 
* 
*

c1 0 8 1
c2 2 8 1
c3 70 8 1
c4 1 2 1

* for spice simulation
r4 8 3 1
r3 2 3 1


r5 0 3 1
r6 20 9 1
e7 70 10 20 9  2
r8 0 5 1
r9 5 6 1
r17 20 3 1
v17 20 3 dc 0
*h10 0 1 r17    2
h10 0 1 v17    2
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
*.ac dec 100 1 1000000 
.ac dec 1000 1 10
.print vdb(8)
.end
