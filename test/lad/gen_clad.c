#include <stdio.h>
main(int argc, char **argv)
{
	int sec, i, j;
	int rcout = 1;

	sec = atoi(argv[1]);
	printf("#sec: %d\n",sec);
	for( i = 1; i <= sec; i++){
		printf("R%d %d %d 1k\n",rcout++, i, i+2);
		printf("c%d %d %d 1pf\n",rcout++, i+2, 0);
		if(i%2 == 1)
			printf("cc%d %d %d 1pf\n",rcout++, i, i+1);
	}
}
		
