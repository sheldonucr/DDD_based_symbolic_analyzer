/*
**	Define the speical noise spectrum density functions
**	for nouse sources.
**	The funciton is always named noise_special_func();
*/

/*
 *    $RCSfile: noise_src.cc,v $
 *    $Revision: 1.3 $
 *    $Date: 2002/05/20 04:22:01 $
 */
	

#include<math.h>
#include"acan.h"
static double pre_f;
static double pre_res;
double
noise_source_func(double f)
{
	double out;

	if(f == pre_f){
		return pre_res;
		printf("hit!\n");
	}
/*
**	equalivant noise source of cascode circuit 
*/

   double t1 = f*f;
   double    t2 = t1*t1;
   double    t3 = t2*t1;
   double    t4 = t2*t2;
   double    t5 = t4*t3;
   double    t6 = t4*t4;
   double    t7 = t6*t6;
   double    t8 = t7*t5;
   double    t9 = t2*f;
   double    t10 = t4*t9;
   double    t12 = t4*t2;
   double    t13 = t7*t12;
   double    t14 = t1*f;
   double    t15 = t4*t14;
   double    t17 = t4*t1;
   double    t18 = t7*t17;
   double    t19 = t4*f;
   double    t21 = t7*t4;
   double    t22 = t2*t14;
   double    t24 = t7*t3;
   double    t26 = t7*t2;
   double    t28 = 0.1651238283E-271*t8+0.253402491E-262*t7*t10+0.2867277026E-253*t13+
0.6407597706E-244*t7*t15+0.1861483759E-235*t18+0.6148742324E-226*t7*t19+
0.5485622334E-218*t21+0.2763313709E-208*t7*t22+0.699487678E-201*t24+
0.5785077542E-191*t7*t9+0.4087997497E-184*t26+0.4993638761E-174*t7*t14;
  double     t29 = t7*t1;
  double     t33 = t6*t5;
  double     t35 = t6*t12;
  double     t37 = t6*t17;
  double     t39 = t6*t4;
  double     t41 = 0.391445929E-167*t29+0.1834525786E-157*t7*f+0.1035021418E-150*t7+
0.1930688651E-140*t6*t4*t22+0.4088184128E-134*t33-0.442400886E-125*t6*t10+
0.1043247495E-117*t35+0.7390527121E-108*t6*t15+0.2317448602E-102*t37+
0.101373365E-92*t6*t19+0.1161633104E-87*t39+0.2298758728E-78*t6*t22;
  double t44 = 0.1407600324E-272*t7*t6+0.188292054E-254*t8+0.87099576E-237*t13+
0.1913493E-219*t18+0.28292848E-202*t21+0.2334507E-185*t24-0.557129E-169*t26+
0.10654597E-151*t29-0.69823725E-135*t7+0.41647062E-119*t33+0.366710015E-102*t35
+0.1078092935E-86*t37+0.6112362579E-72*t39;
      out = (t28+t41)/t44;

	  pre_f = f;
	  pre_res = out;

	  return out;
}
