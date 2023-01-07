#include <stdio.h>
#include <stdlib.h>
//#include "variables.h"

#define N 8
//#define P_INPUT 1
#define P_OUTPUT 1

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })


void __bcc_init70(void){
    unsigned int * apbuart_ctrl = (unsigned int*) 0xfc001008;
    //enable UART
    *apbuart_ctrl = 0x3;
}

void print(char c, unsigned char src[N][N]) {
    printf("%c:\n", c);
    for (int i = 0; i<N; i++){
        for (int j = 0; j<N; j++){
            if (c=='B') printf("%d ", src[j][i]);
            else printf("%d ", src[i][j]);
        }
        printf("\n");
    }
}

void product(unsigned char A[N][N], unsigned char B[N][N], unsigned char C[N][N]){
    int sum = 0;
    int aux;
    int matA, matB, matC;
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++){
            for(int k=0; k<N/4; k++){
                //puts("Hello Marc!");
                matA = *((int *) &A[i][k*4]);
                matB = *((int *) &B[j][k*4]);
                asm("usdot %0, %1, %2":"=r"(matC):"r"(matA), "r"(matB));
                //printf("matC=%d=0x%x\n", matC, matC);
                asm("usadd_ %1, %1, %2" :"=r"(sum):"r"(sum), "r"(matC));
                //printf("sum=%d=0x%x\n", sum, sum);
                //printf("matC=%d=0x%x\n", matC, matC);
            }

            C[i][j] = sum;
            sum = 0;
            //printf("matA=%d=0x%x\n", matA, matA);
            //printf("matB=%d=0x%x\n", matB, matB);
            //printf("sum=%d\n", sum);
            //printf("matC=%d=0x%x\n", matC, matC);
        }
}

void init(unsigned char A[N][N], unsigned char B[N][N]){
	unsigned int previous = 134775813U;
	for(int i=0; i<N; i++)
	    for(int j=0; j<N; j++) {
	        previous = previous*48271U;
	        A[i][j] = previous%7; 
	        previous = previous*48271U;
	        B[j][i] = previous%7; 
        }
}

int main()
{
	unsigned char A[N][N], B[N][N], C[N][N], D[N][N];
	//srand(N);
	init(A,B);
	puts("MULT SIMD BEGIN");
	product(A,B,C);
	puts("MULT SIMD END");
    #ifdef P_INPUT
	print('A',A);
	print('B',B);
    #endif
    #ifdef P_OUTPUT
	print('C',C);
    #endif
}
