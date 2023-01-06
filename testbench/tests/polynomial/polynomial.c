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

#define A 3
#define B 5
#define C -15

void equation(char X[N], char Y[N]){
    for (int i = 0; i<N; i++){
        char x = X[i];
        /*char tmp_x = x;
        printf("x=%d\n", tmp_x);
        tmp_x = A * x;
        printf("A*x=%d\n", tmp_x);
        tmp_x = tmp_x * x;
        printf("A*x*x=%d\n", tmp_x);
        tmp_x = tmp_x + B*x;
        printf("A*x*x+B*x=%d\n", tmp_x);
        tmp_x = tmp_x + C;
        printf("A*x*x+B*x+C=%d\n", tmp_x);*/
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
    char X[N], Y[N];
    srand(N);
    init(X);
    equation(X,Y);
    #ifdef P_INPUT
    print(X);
    #endif
    #ifdef P_OUTPUT
    print(Y);
    #endif
}
