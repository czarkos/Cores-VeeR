#include <stdio.h>
#include "variables.h"
#include <stdlib.h>

//#include "arrays.h"

//#define N 32
//#define P_INPUT
//#define P_OUTPUT

void grayscale(unsigned char src[N][4][N], unsigned char dst[N][N]){
	unsigned char color;
    int r, g, b;
    int shift = 2;
	for (int i = 0; i<N; i++)
		for (int j = 0; j<N; j++){
			//dst[i][j] = (src[i][0][j]>>2) + (src[i][1][j]>>2) + (src[i][2][j]>>2);
			dst[i][j] = (src[i][0][j]>>shift) + (src[i][1][j]>>shift) + (src[i][2][j]>>shift);
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

unsigned char source[N][4][N];// = {IMAGE_ARRAY};
int main(){
    int a = 0;
    //clock_t start_t, end_t;
	unsigned char dest[N][N];
	init(source);
	puts("GRAY BEGIN");
    // these two instructions are here to find were to start counting the cycles
    asm("add_ %0, %1, %2" : "=r"(a) : "r"(a), "r"(a)); //a*x + b
    //start_t = clock();
	grayscale(source, dest);
    //end_t = clock();
    //printf("cycles=%lu", end_t-start_t);
    asm("add_ %0, %1, %2" : "=r"(a) : "r"(a), "r"(a)); //a*x + b
	puts("GRAY END");
#ifdef P_INPUT
	print_in(source);
#endif
#ifdef P_OUTPUT
	print(dest);
#endif
}


