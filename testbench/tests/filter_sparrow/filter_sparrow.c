#include <stdio.h>
//#include "variables.h"
#include <stdlib.h>
//#include "arrays.h"

#define N 4
#define P_OUTPUT

#ifndef N
#define N 2
#endif

#define min(a,b) a < b ? a : b
#define max(a,b) a > b ? a : b

void __bcc_init70(void){
    unsigned int * apbuart_ctrl = (unsigned int*) 0xfc001008;
    //enable UART
    *apbuart_ctrl = 0x3;
}

char f[3][4] = {-1,-1,-1, 0,
                -1, 8,-1, 0,
                -1,-1,-1, 0};

void filter(unsigned char src[N][N], int dst[N][N]){
    for (int i = 0; i<N; i++)
        for (int j = 0; j<N; j++){
            int sum = 0;
            for(unsigned int k = 0; k < 3; ++k)
            {
                int value = 0;
                int dot;
                int	kern = *((int *) &f[k][0]);
                if (!((i == 0 && k==0) || (i==N-1 && k==2)))
                {
                    unsigned char v0;
                    if(j == 0)
                        v0 = 0;
                    else
                        v0 = src[(i-1+k)][j-1];
                    unsigned char v1 = src[(i-1+k)][j];
                    unsigned char v2;
                    if(j == N-1)
                        v2 = 0;
                    else
                        v2 = src[(i-1+k)][j+1];
                    //value = v0+(v1<<8)+(v2<<16);
                    value = (v2<<16)+(v1<<8)+v0;
                    //printf("i=%d, j=%d, k=%d\n", i, j, k);
                    //printf("v0=%d, v1=%d, v2=%d\n", v0, v1, v2);
                    //printf("value=0x%x\n", value);
                    //printf("kernel=0x%x\n", kern);
                    int shifted;
                    int multed;
                    //int vts = 0x1e1e1e1e;
                    //asm("shft_ %0, %1, %2" : "=r"(shifted) : "r"(value), "r"(vts));
                    //asm("dot %0, %1, %2" : "=r"(dot) : "r"(shifted), "r"(kern));
                    asm("dot %0, %1, %2" : "=r"(dot) : "r"(value), "r"(kern));
                    //asm("nop");
                    //asm("nop");
                    //printf("dot_prod=0x%x=%d\n", dot, dot);
                    sum += dot;
                }
            }
            //printf("sum=%d\n", sum);
            //return;
            sum = min(sum,127);
            sum = max(sum,0);
            dst[i][j] = sum;
        }
}

void print( int src[N][N]) {
    printf("P3\n%d %d\n255\n",N,N);
    for (int i = 0; i<N; i++){
        for (int j = 0; j<N; j++){
            //printf("%d %d %d ", src[i][j]<<1, src[i][j]<<1, src[i][j]<<1);
            printf("%d ", src[i][j]);
        }
        printf("\n");
    }
}

void print_source(unsigned char src[N][N]) {
    printf("P3\n%d %d\n255\n",N,N);
    for (int i = 0; i<N; i++){
        for (int j = 0; j<N; j++){
            printf("%d", src[i][j]);
        }
        printf("\n");
    }
}
    
void init(unsigned char A[N][N]){
	unsigned int previous = 134775813U;
	for(int i=0; i<N; i++) {
	    for(int j=0; j<N; j++) {
	        previous = previous*48271U;
	        A[i][j] = previous%7; //rand()%10
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
}

int main(){
    unsigned char source[N][N];// = {GRAY_ARRAY};
    int dest[N][N];
    init(source);
    //print_source(source);
    puts("FILTER SIMD BEGIN");
    filter(source, dest);
    puts("FILTER SIMD END");
    #ifdef P_OUTPUT
    print(dest);
    #endif
}
