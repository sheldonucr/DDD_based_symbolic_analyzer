* Example 3 from Rutkowski's paper0/22/87

c1 0 8 1p
c2 2 8 1p
c3 7 8 1p
c4 1 2 1p

r5 0 3 10k
r6 2 9 1e-5
r7 7 10 1e-5 
r8 0 5 1k
r9 5 6 110k
r10 0 1 10.04
r11 1 3 10k
r12 0 2 1k
r13 0 6 1k
r14 0 7 1k
r15 5 7 1k
r16 6 7 1k
r17 2 3 1k
r18 2 7 1k
r19 7 9 0.5k
r20 2 10 20k
r21 9 10 500
v1 8 0 DC 0 AC 1
*.AC DEC 100 0.01 1000
.AC DEC 200 1 10000meg
.PRINT ac vdb(9)
.END
