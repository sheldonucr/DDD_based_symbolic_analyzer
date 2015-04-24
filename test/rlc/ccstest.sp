* Example 3 from Rutkowski's paper 6/88
* Testing controlled voltage sources 
* 
*

c1 0 8 .0000001 
c2 2 8 .0000001 
c3 7 8 .0000001 
c4 1 2 .0000001

* add for spice3 simulation 
r4 8 3 1000 

r5 0 3 1000
r6 2 9 1000
g7 7 10 2 9   2
r8 0 5 1000
r9 5 6 1000
r17 2 3 1000
*f10 0 1 r17  2 
v17 2 3 dc 0
f10 0 1 v17  2
r11 1 3 1000 
r12 0 2 1000
r13 0 6 1000
r14 0 7 1000
r15 5 7 1000 
r16 6 7 1000
r18 2 7 1000
r19 7 9 1000
r20 2 10 1000 
r21 9 10 1000
vin 10 0 dc 0 ac 1.0
*.ac dec 100 1 1000000
.ac dec 1000 1 10
.print ac vdb(8)
.end
