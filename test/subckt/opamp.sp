* linear opamp circuit  out(v(4))
*
*   reference: jiri vlach and kishore singhal, 'computer
*              methods for circuit analysis and design',
*              van nostrand reinhold co., 1983, pages 142
*              and 494 to 496.
*
*  pole/zero analysis and using vcvs as an ideal op-amp.
*  for just pole/zero analysis .ac statement is not required.

rii 2 3 2meg
ri1 2 0 500meg
ri2 3 0 500meg
g1 1 0 2 3 2
c1 1 0 26.6u
r1 1 0 1k
g2 4 0 1 0 1.33333
rld 4 0 75
vin 2 3 dc 0 ac 1
*.print vm(20) vm(10) vp(20)
.ac dec 10 1 100meg
.print vdb(4)
.end
