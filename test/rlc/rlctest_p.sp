* Example 3 from Rutkowski's paper0/22/87

c1 0 8 1
c2 2 8 1
c3 7 8 1
c4 1 2 1

r5 0 3 1
r6 2 9 1
r7 7 10 1 
r8 0 5 1
r9 5 6 1
r10 0 1 10.04
r11 1 3 1
r12 0 2 1
r13 0 6 1
r14 0 7 1
r15 5 7 1
r16 6 7 1
r17 2 3 1
r18 2 7 1
r19 7 9 1
r20 2 10 1
r21 9 10 1
v1 10 0 DC 0 AC 1
*.AC DEC 100 0.01 1000
*.AC DEC 2 1 100meg
.AC DEC 1000 1 10
.PRINT ac vdb(8)
.END
