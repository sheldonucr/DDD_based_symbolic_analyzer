#include <stdio.h>
main(int argc, char **argv)
{
	int sec, i, j;
	int rcout = 1;

	sec = atoi(argv[1]);

	printf("*This is %d sections ladder circuit\n",sec);

	for( i = 1; i <= sec; i++){
		// printf("R%d %d %d 1k\n",rcout++, i, 0);
		printf("C%d %d %d 1pf\n",rcout++, i, 0);
		printf("R%d %d %d 1k\n",rcout++, i, i+1);
		/*printf("c%d %d %d 1pf\n",rcout++, i, 1); */
	}
	printf("R%d %d %d 1k\n\n",rcout++, i, 0);
	
	printf("vin 1 0 dc 0 ac 1\n");
	printf(".ac dec 1000 1 10\n");
	printf(".plot v(%d)\n", sec+1);
	
	printf(".end\n");
}
		
