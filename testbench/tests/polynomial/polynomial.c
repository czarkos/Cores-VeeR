#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "variables.h"

//#define N 16
//#define P_INPUT 1
//#define P_OUTPUT 1

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#ifndef N
#define N 4
#endif

//#define A 3
//#define B 5
//#define C -15

int A = 3;
int B = 5;
int C = -15;

void equation(char X[N], char Y[N]){
    for (int i = 0; i<N; i++){
        char x = X[i];
        Y[i] = A*x*x+B*x+C;
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
    int a = 0;
    char X[N], Y[N];
    srand(N);
    init(X);
    // these two instructions are here to find were to start counting the cycles
    asm("add_ %0, %1, %2" : "=r"(a) : "r"(a), "r"(a)); //a*x + b
    equation(X,Y);
    asm("add_ %0, %1, %2" : "=r"(a) : "r"(a), "r"(a)); //a*x + b
    #ifdef P_INPUT
    print(X);
    #endif
    #ifdef P_OUTPUT
    print(Y);
    #endif
}
