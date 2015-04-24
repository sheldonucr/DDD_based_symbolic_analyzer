PINO-MILLER-OPAMP
*
* MILLER opamp, pMOS input pair, n-well technology
* ------------------------------------------------
*
* CIRCUIT DESCRIPTION
*
*M1A  4 7 3 3 P W=10.2U  L=17.3U  AD=99P   AS=99P   PD=40U   PS=40U
*mxxx d g s b 
gm1a  4 3 7 3 5.89e-06
*gmb1a 4 3 3 3 1.51e-06
rds1a 4 3 9.26e+7
cbd1a 3 4 2.04e-14 
*cbs1a 3 3 5.03e-14
cgs1a 7 3 4.78e-14
*cgd1a 7 4 0.0
*cgb1a 7 3 0.0
*M1B  5 8 3 3 P W=10.2U  L=17.3U  AD=99P   AS=99P   PD=40U   PS=40U
*mxxx d g s b
gm1b  5 3 8 3 5.89e-06
*gmb1b 5 3 3 3 1.51e-06 
rds1b 5 3 9.26e+7
cbd1b 3 5 2.04e-14 
*cbs1b 3 3 5.03e-14
cgs1b 8 3 4.78e-14
*cgd1b 8 5 0.0
*cgb1b 8 3 0.0
*M2A  4 4 2 2 N W=100U L=20.3U AD=99P   AS=99P   PD=40U   PS=40U
*mxxx d g s b
gm2a  4 0 4 0 3e-05
*gmb2a 4 0 0 0 5.16e-06
rds2a 4 0 2.46e+7
cbd2a 0 4 1.64e-14 
*cbs2a 0 0 2.25e-14
cgs2a 4 0 5.5e-13
*cgd2a 4 4 0.0
*cgb2a 4 0 0.0
*M2B  5 4 2 2 N W=100U L=20.3U AD=99P   AS=99P   PD=40U   PS=40U
*mxxx d g s b
gm2b  5 0 4 0 3e-05
*gmb2b 5 0 0 0 5.16e-06
rds2b 5 0 2.46e+7
cbd2b 0 5 1.64e-14 
*cbs2b 0 0 2.25e-14
cgs2b 4 0 5.5e-13
*cgd2b 4 5 0.0
*cgb2b 4 0 0.0
*M3   6 5 2 2 NOUT W=172.4U L=4.3U  AD=1647P AS=1647P PD=384U  PS=384U
*mxxx d g s b
gm3	 6 0 5 0 0.000406
*gmb3 6 0 0 0 6.98e-05
rds3 6 0 4.95e+5 
cbd3 0 6 9.56e-14 
*cbs3 0 0 2.65e-13
cgs3 5 0 2.01e-13 
*cgd3 5 6 0.0
*cgb3 5 0 0.0
*M4   6 9 1 1 POUT W=86.2U  L=4.3U   AD=783P  AS=783P  PD=192U  PS=192U
*mxxx d g s b
*gm4	 6 0 0 0 8e-05
*gmb4 6 0 0 0 2.23e-05
rds4 6 0 3.62e+3
cbd4 0 6  3.05e-13  
*cbs4 0 0 3.43e-13
*cgs4 0 0 8.23e-14 
cgd4 0 6 6.71e-14 
*cgb4 0 1 0.0
*M5   3 9 1 1 P W=10.2U  L=20.3U  AD=99P   AS=99P   PD=40U   PS=40U
*mxxx d g s b
*gm5 3 0 0 0 7.71e-06
*gmb5 3 0 0 0 1.85e-06 
rds5 3 0 4.695e+7
cbd5 0 3 2.45e-14 
*cbs5 0 0 5.03e-14
*cgs5 0 0 5.61e-14
*cgd5 9 3 0.0
*cgb5 9 0 0.0
*
CC 5 6 1P
CL 6 0 10P
RL 6 0 100K

RV- 7 6 100k
*
*VDD 1 0 DC 5
*VSS 2 0 DC -5
V+  8 0 DC 0 AC 1
*V-  7 0 DC 0 
*VB  9 1 DC -2
*VB  9 1 DC 0
*IOUT 0 6
* CONTROL CARDS
*
.AC DEC 10 1 1000MEG
.PRINT VDB(6)
.END
