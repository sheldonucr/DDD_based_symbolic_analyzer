#include <stdio.h>

void
main(int argc, char **argv)
{
   FILE *fp;
   int   sum = 0,val;
   char  buf[1024];
   if(argc < 2)
      exit(1);
   fp = fopen(argv[1],"r");
   while(fgets(buf,1023,fp)){
      if(sscanf(buf,"%d", &val) == 0)
         break;
      sum += val;
   }
   printf("sum=%d\n", sum);
}
