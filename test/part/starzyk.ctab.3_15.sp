* Testing Starzyk's example
* Testing opamps
* 
*
* format for opamp
* opamp -node out +node

.subckt opamp 3 4 2
rii 2 3 2meg
ri1 2 0 500meg
ri2 3 0 500meg
g1 1 0 2 3 2
c1 1 0 26.6u
r1 1 0 1k
g2 4 0 1 0 1.33333
rld 4 0 75
.ends

.subckt blk2 1 19 22 7 6 
r1 1 2 1.0 
r2 12 8 1.0 
r3 2 0 1.0
r4 8 3 1.0
R5 3 4 1.0
r6 4 5 1.0
c6 4 5 1.0
r7 5 6 1.0
c8 6 7 1.0
r9 7 8 1.0
xo1 8 3 2 opamp
xo2 4 5 0 opamp
xo3 6 7 0 opamp

* circuit 2
r10 5 9 1.0 
r11 19 15 1.0 
r12 9 0 1.0
r13 10 15 1.0
r14 10 11 1.0
r15 11 12 1.0
c15 11 12 1.0
r16 12 13 1.0
c17 13 14 1.0
r18 14 15 1.0
xo4 15 10 9 opamp
xo5 11 12 0 opamp
xo6 13 14 0 opamp

r19 12 16 1.0
r21 16 0 1.0
r22 22 17 1.0
r23 17 18 1.0
r24 18 19 1.0
c24 18 19 1.0
xo7 22 17 16 opamp
xo8 18 19 0 opamp
.ends


.subckt blk1 19 22 
r25 19 20 1.0
c26 20 21 1.0
r27 21 22 1.0
xo9 20 21 0 opamp
.ends

.subckt blk0 19 22 30 
r20 22 26 1.0
r28 19 23 1.0
r29 29 30 1.0
r30 23 0 1.0
r31 24 29 1.0
r32 24 25 1.0
r33 25 26 1.0
c33 25 26 1.0
r34 26 27 1.0
c35 27 28 1.0
r36 28 29 1.0
xo10 29 24 23 opamp
xo11 25 26 0 opamp
xo12 27 28 0 opamp
 
* circuit 5
r37 26 31 1.0
r38 32 0 1.0
r39 31 0 1.0
r40 32 30 1.0
xo13 32 30 31 opamp
.ends


xoo1 19 22 blk2
xoo2 1 19 22 7 6 blk2
xoo0 19 22 30 blk0

vin 1 0 dc 0 ac 1
.ac dec 100 1 1000meg
.print vdb(30)
.end
