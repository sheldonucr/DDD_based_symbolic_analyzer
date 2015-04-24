*The sample of bandpass
v1 1 0 DC 0 AC 1
*r0 1 0 100k
r1 1 2 1k 
c1 2 0 1u 
c2 3 4 0.00001 
r2 4 0 1 
r10 3 0 1000k
gk 3 0 2 0 10 
.AC DEC 100 0.0001 10e7
.print ac vdb(4)
.end
