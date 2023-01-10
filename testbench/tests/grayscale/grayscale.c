#include <stdio.h>
//#include "variables.h"
#include <stdlib.h>
//#include "arrays.h"

#define N 32
//#define P_INPUT
#define P_OUTPUT

void grayscale(unsigned char src[N][4][N], unsigned char dst[N][N]){
	unsigned char color;
    int r, g, b;
	for (int i = 0; i<N; i++)
		for (int j = 0; j<N; j++){
            //r = src[i][0][j];
            //g = src[i][1][j];
            //b = src[i][2][j];
            //printf("before: r=%d, g=%d, b=%d\n", r, g, b);
            //r = src[i][0][j]>>2;
            //g = src[i][1][j]>>2;
            //b = src[i][2][j]>>2;
            //printf("after: r=%d, g=%d, b=%d\n", r, g, b);
			dst[i][j] = (src[i][0][j]>>2) + (src[i][1][j]>>2) + (src[i][2][j]>>2);
		}
}

void print_in(unsigned char src[N][4][N]) {
	printf("P3\n%d %d\n255\n",N,N);
	for (int i = 0; i<N; i++){
		for (int j = 0; j<N; j++){
			printf("%d %d %d %d ", src[i][0][j], src[i][1][j], src[i][2][j], src[i][3][j]);
		}
		printf("\n");
	}
}
void print(unsigned char src[N][N]) {
	printf("P3\n%d %d\n255\n",N,N);
	for (int i = 0; i<N; i++){
		for (int j = 0; j<N; j++){
			printf("%d %d %d ", src[i][j], src[i][j], src[i][j]);
		}
		printf("\n");
	}
}

void init(unsigned char A[N][4][N]){
	unsigned int previous = 134775813U;
	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++) {
			previous = previous*48271U;
			A[i][0][j] = previous%256; //rand()%10
			previous = previous*48271U;
			A[i][1][j] = previous%256; //rand()%10;
			previous = previous*48271U;
			A[i][2][j] = previous%256; //rand()%10;
			A[i][3][j] = 255;
		}
}

int main(){
	unsigned char source[N][4][N];// = {IMAGE_ARRAY};
	unsigned char dest[N][N];
	init(source);
	puts("GRAY BEGIN");
	grayscale(source, dest);
	puts("GRAY END");
#ifdef P_INPUT
	print_in(source);
#endif
#ifdef P_OUTPUT
	print(dest);
#endif
}


