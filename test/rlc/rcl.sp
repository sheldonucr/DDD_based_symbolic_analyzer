*This is a sample input deck for SCAPP
v1 1 0 DC 0 AC 1

r1 1 2 10k
c1 2 0 14pf
r4 2 3 20k
c2 3 0 10pf
l5 3 4 1
c3 4 0 4pf
r6 4 5 3k
c6 5 0 20pf
.AC DEC 1 1 10
.PRINT ac vdb(5)
.end
