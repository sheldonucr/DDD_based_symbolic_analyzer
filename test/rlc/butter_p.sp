* Another example

v1 1 0 DC 0 AC 1
rS  1 2 1
c1  2 0 .0498
l2  2 3 .1445
c3  3 0 .2251
l4  3 4 .2836
c5  4 0 .3144
l6  4 5 .3144
c7  5 0 .2836
l8  5 6 .2251
c9  6 0 .1445
l10 6 7 .0498
rl  7 0 1

*.AC DEC 200 0.001 10
.AC DEC 10 0.1 1meg
.print vdb(7)
.end

