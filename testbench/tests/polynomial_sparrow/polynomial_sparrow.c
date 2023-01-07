#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//#include "variables.h"

#define N 16
#define P_INPUT 1
#define P_OUTPUT 1

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#ifndef N
#define N 4
#endif

//#define A 1 //3 imm -> 0b00001 imm = imm(4)(-) 2^(1+imm(3-1))+imm(0) = 2^1+1 = 3
//#define B 5 
//#define C 15 //-15 use sub instead

int A = 0x03030303;
int B = 0x05050505;
int C = 0x0F0F0F0F;

void equation(char X[N], char Y[N]){
    int x, y;
    for (int i = 0; i<N; i+=4){
        x = *((int *) &X[i]);
        //printf("x=0x%x\n", x);
        asm("mul_ %0, %1, %2" : "=r"(y) : "r"(x), "r"(A));  //a*x
        asm("add_ %0, %1, %2" : "=r"(y) : "r"(y), "r"(B)); //a*x + b
        asm("mul_ %0, %1, %2" : "=r"(y) : "r"(y), "r"(x)); //(a*x + b)*x
        asm("sub_ %0, %1, %2" : "=r"(y) : "r"(y), "r"(C)); //(a*x + b)*x + c

        int *intpointer = ((int *) &Y[i]);
        *intpointer = y;
    }
}

void init(char array[N]){
    for (int i = 0; i<N; i++)
        array[i] = rand()%20 - 10;
}

void print(char array[N]){
    for (int i = 0; i<N; i++) 
        printf("%d ",array[i]);
    printf("\n");
}

int main()
{
    char X[N], Y[N];
    srand(N);
    init(X);
    #ifdef P_INPUT
    print(X);
    #endif
    equation(X,Y);
    #ifdef P_OUTPUT
    print(Y);
    #endif
}
