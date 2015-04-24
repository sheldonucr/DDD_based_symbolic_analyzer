*This is a sample from Carl Sechen paper

r0 1 0 0.1
c1 1 2 1u
r2 2 0 1.5
r3 2 3 1.8
r4 3 0 1.9 
*c5 3 4 10pf 
*c6 4 5 12pf 
*c7 5 0 1.3pf 
*c8 5 6 10pf 
*rr8 6 0 14 
*c9 6 7 10pf 
*c10 7 0 15pf 

v1 1 0 dc 1.0 ac 1.0
.ac dec 10 100 1000
.print ac vdb(3)
