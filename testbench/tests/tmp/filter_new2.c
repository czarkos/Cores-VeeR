#include <stdio.h>
//#include "variables.h"
#include <stdlib.h>
//#include "arrays.h"

#define N 4
#define P_OUTPUT


#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

void __bcc_init70(void){
    unsigned int * apbuart_ctrl = (unsigned int*) 0xfc001008;
    //enable UART
    *apbuart_ctrl = 0x3;
}

char f[3][3] = {-1,-1,-1,
                -1,8,-1,
                -1,-1,-1};

void filter(unsigned char src[N+4][N+4], unsigned char dst[N][N]){
    for (int i = 2; i<N+2; i++)
        for (int j = 2; j<N+2; j++){
            int ret = 0;
            int imageX,imageY;
            for (int k = 0; k<3; k++)
                for(int l=0; l<3; l++){
                    imageY = i-1+k;
                    imageX = j-1+l;
                    if (imageY < 2 || imageY >= N+2) continue;
                    else if (imageX < 2 || imageX >= N+2) continue;
                    char filt = f[k][l];
                    //ret += ((int)src[imageY][imageX]>>1)*f[k][l];
                    ret += ((int)src[imageY][imageX])*f[k][l];
                }
            //if(ret > 127)
            //    ret = 127;
            //if(ret < 0)
            //    ret = 0;
            //ret = min(ret,127);
            //ret = max(ret,0);
            dst[i-2][j-2] = ret;
        }
}

void print(unsigned char src[N+4][N+4]) {
    printf("P3\n%d %d\n255\n",N,N);
    for (int i = 0; i<N+4; i++){
        for (int j = 0; j<N+4; j++){
            //printf("%d %d %d ", src[i][j]<<1, src[i][j]<<1, src[i][j]<<1);
            printf("%d ", src[i][j]);
        }
        printf("\n");
    }
}

void print_out(unsigned char src[N][N]) {
    printf("P3\n%d %d\n255\n",N,N);
    for (int i = 0; i<N; i++){
        for (int j = 0; j<N; j++){
            //printf("%d %d %d ", src[i][j]<<1, src[i][j]<<1, src[i][j]<<1);
            printf("%d ", src[i][j]);
        }
        printf("\n");
    }
}
    
void init(unsigned char A[N+4][N+4]){
	unsigned int previous = 134775813U;
    /*
    for(int i = 0; i < N+4; i ++){
        A[i][0]   = 0;
        A[i][1]   = 0;
        A[i][N+2] = 0;
        A[i][N+3] = 0;
    }
    for(int j = 0; j < N+4; j++){
        A[0][j]   = 0;
        A[1][j]   = 0;
        A[N+2][j] = 0;
        A[N+3][j] = 0;
    }
    */
	for(int i=2; i<N+2; i++) {
	    for(int j=2; j<N+2; j++) {
	        previous = previous*48271U;
	        A[i][j] = previous%7; //rand()%10
            //printf("A[%d][%d]=%d ", i, j, A[i][j]);
        }
        //printf("\n");
    }
}

int main(){
    unsigned char source[N+4][N+4] = {0}; //= {GRAY_ARRAY};
    unsigned char dest[N][N];
    init(source);
    print(source);
    puts("FILTER BEGIN");
    filter(source, dest);
    puts("FILTER END");
    #ifdef P_OUTPUT
    print_out(dest);
    #endif
}
