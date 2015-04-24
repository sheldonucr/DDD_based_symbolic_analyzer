CMOS cascode opamp from Q. Yu paper in ICCAD 1994 
*
*m1 16 5 13 3 pmos w=80.00u l=5.00u ad=480.00p as=280.00p pd=184.00u
*+  ps=94.00u
gmm1 16 13 5 13 9.380000e-04
gmbm1 16 13 0 13 4.880000e-05
rdsm1 16 13 0.184713e+04
cbdm1 0 16 4.290000e-14
cbsm1 0 13 3.370000e-14
cgsm1 5 13 1.180000e-13
cgdm1 5 16 0.000000e+00
cgbm1 5 0 0.000000e+00

*m2 17 6 13 3 pmos w=80.00u l=5.00u ad=280.00p as=280.00p pd=94.00u
*+  ps=94.00u
gmm2 17 13 6 13 9.390000e-04
gmbm2 17 13 0 13 4.890000e-05
rdsm2 17 13 3.174603e+04
cbdm2 0 17 2.460000e-14
cbsm2 0 13 3.370000e-14
cgsm2 6 13 1.180000e-13
cgdm2 6 17 0.000000e+00
cgbm2 6 0 0.000000e+00

*m3 16 12 4 4 nmos w=300.00u l=5.00u ad=1800.00p as=1050.00p
*+  pd=624.00u ps=314.00u
gmm3 16 0 12 0 4.810000e-03
gmbm3 16 0 0 0 2.020000e-03
rdsm3 16 0 1.203369e+04
cbdm3 0 16 7.170000e-13
cbsm3 0 0 1.110000e-12
cgsm3 12 0 4.350000e-13
cgdm3 12 16 0.000000e+00
cgbm3 12 0 0.000000e+00

*m4 17 12 4 4 nmos w=300.00u l=5.00u ad=1050.00p as=1050.00p
*+  pd=314.00u ps=314.00u
gmm4 17 0 12 0 4.810000e-03
gmbm4 17 0 0 0 2.020000e-03
rdsm4 17 0 1.206273e+04
cbdm4 0 17 3.880000e-13
cbsm4 0 0 1.110000e-12
cgsm4 12 0 4.350000e-13
cgdm4 12 17 0.000000e+00
cgbm4 12 0 0.000000e+00

c2 16 0 6.5607e-15

*m7 16 16 12 4 nmos w=10.00u l=2.50u ad=77.50p as=77.50p pd=35.50u
*+  ps=35.50u
gmm7 16 12 16 12 1.550000e-04
gmbm7 16 12 0 12 3.460000e-05
rdsm7 16 12 2.272727e+05
cbdm7 0 16 3.600000e-14
cbsm7 0 12 4.770000e-14
cgsm7 16 12 6.840000e-15
cgdm7 16 16 0.000000e+00
cgbm7 16 0 0.000000e+00


c5 16 0 3.19403e-15

*m12 17 17 12 4 nmos w=10.00u l=2.50u ad=77.50p as=77.50p pd=35.50u
*+  ps=35.50u
gmm12 17 12 17 12 1.610000e-04
gmbm12 17 12 0 12 3.590000e-05
rdsm12 17 12 2.136752e+05
cbdm12 0 17 3.590000e-14
cbsm12 0 12 4.770000e-14
cgsm12 17 12 6.840000e-15
cgdm12 17 17 0.000000e+00
cgbm12 17 0 0.000000e+00


c7 17 0 3.19403e-15

c10 12 0 3.97095e-15

 
*.equiv vdd = 1
*.equiv gnd = 2
*.equiv avdd = 3
*.equiv avss = 4
*.equiv non = 5
*.equiv inv = 6
*.equiv out = 7
* opamp1 1 2 3 4 5 6 7
 

*vdd 3 0 dc 12.0v
*vss 0 4 dc 12.0v
*vininv 6 0 dc 0.0v 
vinnon 5 0 dc 10.100810mv ac 1.0v 0
rfb 6 7 100k
 
*.width out=80
*.options list limpts=4000 itl5=0 numdgt=6
 
*.op
 
*.dc vinnon 10.100800mv 10.100820mv 0.001uv
*.plot dc v(5) v(7)
*.print dc v(5) v(7)
 
.ac dec 5 1 800meg
.plot ac vdb(17)
*.plot ac vp(7)
.end
