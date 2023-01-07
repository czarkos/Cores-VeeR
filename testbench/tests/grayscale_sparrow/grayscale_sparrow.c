#include <stdio.h>
//#include "variables.h"
#include <stdlib.h>
//#include "arrays.h"

#define N 16
//#define P_INPUT
#define P_OUTPUT

void grayscale( unsigned char src[N][4][N], unsigned int dst[N][N/4]){
	int shift = 0xfcfcfcfc;
	int scr, scr_shf;
	int r, g, b, gray;
	//asm volatile("scrrd %0":"=r"(scr):);
	scr_shf = (scr & 0xffffe01f) | (0x1B << 5);
	for (int i = 0; i<N; i++)
		for (int j = 0; j<N; j+=4){
			//asm("scrwr %0"::"r"(scr_shf));
			r = *((int *) &src[i][0][j]);
			g = *((int *) &src[i][1][j]);
			b = *((int *) &src[i][2][j]);
            /*
            //printf("r=%x, g=%x, b=%x\n", r, g, b);
			asm("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tshft_ %0, %1, %2" : "=r"(r) : "r"(r), "r"(shift));
			//asm("scrwr %0"::"r"(scr));
			asm("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tshft_ %0, %1, %2" : "=r"(g) : "r"(g), "r"(shift));
			asm("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tshft_ %0, %1, %2" : "=r"(b) : "r"(b), "r"(shift));
            //printf("r=%x, g=%x, b=%x\n", r, g, b);
			asm("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tusadd_ %0, %1, %2" : "=r"(gray) : "r"(r), "r"(g));
            //printf("gray=%x\n", gray);
			asm("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tusadd_ %0, %1, %2" : "=r"(gray) : "r"(gray), "r"(b));
			asm("nop");
            */

			asm("shft_ %0, %1, %2" : "=r"(r) : "r"(r), "r"(shift));
			asm("shft_ %0, %1, %2" : "=r"(g) : "r"(g), "r"(shift));
			asm("shft_ %0, %1, %2" : "=r"(b) : "r"(b), "r"(shift));
			asm("usadd_ %0, %1, %2" : "=r"(gray) : "r"(r), "r"(g));
			asm("usadd_ %0, %1, %2" : "=r"(gray) : "r"(gray), "r"(b));
			asm("nop");
            //printf("gray=%x\n", gray);
			dst[i][j/4]=gray;
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

void print(unsigned int src[N][N/4]) {
	printf("P3\n%d %d\n255\n",N,N);
	for (int i = 0; i<N; i++){
		for (int j = 0; j<N/4; j++){
			printf("%d %d %d ", 0xff & src[i][j],     0xff & src[i][j],     0xff & src[i][j]);
			printf("%d %d %d ", 0xff & src[i][j]>>8,  0xff & src[i][j]>>8,  0xff & src[i][j]>>8);
			printf("%d %d %d ", 0xff & src[i][j]>>16, 0xff & src[i][j]>>16, 0xff & src[i][j]>>16);
			printf("%d %d %d ", src[i][j]>>24, src[i][j]>>24, src[i][j]>>24);
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
	unsigned char source[N][4][N]; //= {IMAGE_ARRAY};
	unsigned int dest[N][N/4];
	init(source);
	puts("GRAY SIMD BEGIN");
	grayscale(source, dest);
	puts("GRAY SIMD END");
#ifdef P_INPUT
	print_in(source);
#endif
#ifdef P_OUTPUT
	print(dest);
#endif
}

