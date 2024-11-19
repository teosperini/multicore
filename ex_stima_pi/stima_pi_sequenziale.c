#import <stdio.h>
#import <stdlib.h>

int main(int argc, char** argv){
	double factor = 1.0;
	double sum = 0.0;
	int n = 8;
	for(i = 0; i<n; i++, factor = -factor){
		sum += factor/(2*1+1);
	}
	int pi = 4.0 * sum;
	printf("pi = %d", pi);
	return 0;
}