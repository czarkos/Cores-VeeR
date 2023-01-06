#include <stdio.h>
#include <stdlib.h>
//#include "variables.h"

#define N 8
#define P_INPUT 1
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
    unsigned char sum = 0;
    unsigned char aux;
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                aux = min((int) A[i][k] * (int) B[j][k], 255);
                sum = min(aux+sum, 255);
            }
            C[i][j] = sum;
            sum = 0;
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
	unsigned char A[N][N], B[N][N], C[N][N];
	//srand(N);
	init(A,B);
  	puts("MULT BEGIN");
	product(A,B,C);
  	puts("MULT END");
    #ifdef P_INPUT
	print('A',A);
	print('B',B);
    #endif
    #ifdef P_OUTPUT
	print('C',C);
    #endif
}
