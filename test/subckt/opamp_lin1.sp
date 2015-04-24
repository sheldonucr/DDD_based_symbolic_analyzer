optimized ua741 linear moodel circuit with cost 0.017 out(v(4))
****************

*.subckt opamp 2 3 4
rii 2 3 2.477422e+07
ri1 2 0 8.953649e+08
ri2 3 0 6.137621e+07
g1 1 0 2 3 1.610646e-01
c1 1 0 1.210598e-05
r1 1 0 1.092436e+01
g2 4 0 1 0 1.725431e+00
rld 4 0 3.160853e+02
vi 2 3 dc 0 ac 1
.ac dec 10 10 100000
.PRINT AC VR(4) Vi(4)
.end
