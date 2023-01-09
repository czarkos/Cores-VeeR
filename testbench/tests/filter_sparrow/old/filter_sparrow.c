#include <stdio.h>
//#include "variables.h"
#include <stdlib.h>
//#include "arrays.h"

#define N 2
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
    int scr;
    //asm volatile ("scrrd  %0":"=r"(scr):);
    //printf("scr = %#010x\n",scr);
    for (int i = 0; i<N; i++)
        for (int j = 0; j<N; j++){
            int sum = 0;
            //if (j==0) asm("scrwr %0, 0x19"::"r"(scr)); //in limit, only operate with 2 values
            //else if (j==N-1) asm("scrwr %0, 0x1c"::"r"(scr)); //in limit, only operate with 2 values
            //else asm("scrwr %0, 0x18"::"r"(scr)); //otherwise operate with 3
            for(unsigned int k = 0; k < 3; ++k)
            {
                int value = 0;
                int dot;
                int	kern = *((int *) &f[k][0]);
                if (!((i == 0 && k==0) || (i==N-1 && k==2)))
                {
                    if( j%4==0){
                        value = *((short*) &src[i-1+k][j])<<8;
                        if (j!=0) value |= src[i-1+k][j-1];
                    }
                    else if( j%4==1) value = *((int*) &src[i-1+k][j-1]);
                    else if( j%4==2) value = (*((int*) &src[i-1+k][j-2]))>>8;
                    else if( j%4==3){
                        value = (*((short*) &src[i-1+k][j-1])) ;
                        if (j!=N-1) value |= src[i-1+k][j+1]<<16;
                    }
                    //unsigned char v0 = src[(i-1+k)][j-1];
                    //unsigned char v1 = src[(i-1+k)][j];
                    //unsigned char v2 = src[(i-1+k)][j+1];
                    //value = v0+(v1<<8)+(v2<<16);
                    int shifted;
                    int multed;
                    int vts = 0x1e1e1e1e;
                    asm("shft_ %0, %1, %2" : "=r"(shifted) : "r"(value), "r"(vts));
                    asm("dot %0, %1, %2" : "=r"(dot) : "r"(shifted), "r"(kern));
                    asm("nop");
                    asm("nop");
                    sum += dot;
                }
            }
            sum = min(sum,127);
            sum = max(sum,0);
            dst[i][j] = sum;
        }
}

void print( int src[N][N]) {
    printf("P3\n%d %d\n255\n",N,N);
    for (int i = 0; i<N; i++){
        for (int j = 0; j<N; j++){
            printf("%d %d %d ", src[i][j]<<1, src[i][j]<<1, src[i][j]<<1);
        }
        printf("\n");
    }
}

void print_source(unsigned char src[N][N]) {
    printf("P3\n%d %d\n255\n",N,N);
    for (int i = 0; i<N; i++){
        for (int j = 0; j<N; j++){
            printf("%d %d %d ", src[i][j]<<1, src[i][j]<<1, src[i][j]<<1);
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
