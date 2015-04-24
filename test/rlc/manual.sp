*This is a sample input deck for SCAPP
v1 1 0 DC 0 AC 1
*I1 1 0 10m
r1 1 2 5meg
r2 2 0 10K
c1 2 3 15u
l1 3 0 45m
C2 3 4 30N
L2 4 0 10M
R3 4 0 200k
.AC DEC 100 100 100000 
.print ac vr(4)
.end
