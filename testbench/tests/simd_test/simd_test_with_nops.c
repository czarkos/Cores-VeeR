#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void __bcc_init70(void){
    unsigned int * apbuart_ctrl = (unsigned int*) 0xfc001008;
    //enable UART
    *apbuart_ctrl = 0x3;
}

int main()
{
    int a,b,c,d;
    
    //test nop
    /*
    a = 0x01020304;
    b = 0x00010203;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop_ %0, %1":"=r"(c):"r"(a));
    //printf("NOP (move) c=%#010x, expected 0x01020304\n", c);
    printf("NOP (move) c=0x%x, expected 0x01020304\n", c);
    */

    a = 0x01020304;
    b = 0x00010203;
    //test add
    asm("add_ %0, %1, %2":"=r"(c):"r"(a), "r"(b));
    //add a b 
    //printf("add: c=%#010x, expected result 0x01030507\n", c);
    printf("add: c=0x%x, expected result 0x01030507\n", c);

    /*
    //test sadd
    a=0x8180ff01;
    b=0x81ff7f7f;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("sadd_ %0, %1, %2":"=r"(c):"r"(a), "r"(b));
    //printf("sadd: c=%#010x, expected result 0x80807e7f\n", c);
    printf("sadd: c=0x%x, expected result 0x80807e7f\n", c);

    //test sub
    a=0x0a0a0a0a;
    b=0x00050a0b;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("sub_ %0, %1, %2":"=r"(c):"r"(a), "r"(b));
    //printf("sub: c=%#010x, expected result 0x0a0500ff\n", c);
    printf("sub: c=0x%x, expected result 0x0a0500ff\n", c);
    
    //test ssub
    a=0x807f0afb;
    b=0x05fffb0a;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("ssub_ %0, %1, %2":"=r"(c):"r"(a), "r"(b));
    //printf("ssub: c=%#010x, expected result 0x807f0ff1\n", c);
    printf("ssub: c=0x%x, expected result 0x807f0ff1\n", c);

    //test Max MAX signed
    a=0x0204080a;
    b=0x204080a0;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("max_max %0, %1, %2":"=r"(c):"r"(a), "r"(b));
    asm("nop");
    //printf("signed max max: c=%#010x, expected result 0x00000040\n", c);
    printf("signed max max: c=0x%x, expected result 0x00000040\n", c);

    //test Max MAX unsigned
    asm("umax_umax %0, %1, %2":"=r"(c):"r"(a), "r"(b));
    asm("nop");
    //printf("unsigned max max: c=%#010x, expected result 0x000000a0\n", c);
    printf("unsigned max max: c=0x%x, expected result 0x000000a0\n", c);

    //test Min MIN unsigned
    asm("umin_umin %0, %1, %2":"=r"(c):"r"(a), "r"(b));
    asm("nop");
    //printf("unsigned min min: c=%#010x, expected result 0x00000002\n", c);
    printf("unsigned min min: c=0x%x, expected result 0x00000002\n", c);

    //test Min MIN signed
    asm("min_min %0, %1, %2":"=r"(c):"r"(a), "r"(b));
    asm("nop");
    //printf("signed min min: c=%#010x, expected result 0xffffff80\n", c);
    printf("signed min min: c=0x%x, expected result 0xffffff80\n", c);

    //test dot product
    a=0x01020304;
    b=0x00010203;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("dot %0, %1, %2":"=r"(c):"r"(a), "r"(b));
    asm("nop");
    //printf("dot product: c=%#010x, expected result 0x00000014\n", c);
    printf("dot product: c=0x%x, expected result 0x00000014\n", c);
   
    //test dot product2
    a=0xfffe03fc;
    b=0x00ff0203;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("dot %0, %1, %2":"=r"(c):"r"(a), "r"(b));
    asm("nop");
    //printf("dot product2: c=%#010x, expected result 0xfffffffc\n", c);
    printf("dot product2: c=0x%x, expected result 0xfffffffc\n", c);
    

    int sum = 0;
    a=0x04020000;
    b=0x02000504;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
  //c=0x7f81807f
    asm("usdot %0, %1, %2":"=r"(c):"r"(a), "r"(b));
    asm("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tusadd_ %1, %1, %2" :"=r"(sum):"r"(sum), "r"(c));
    //printf("saturate mul: c=%#010x, expected result 0x7f81807f\n",c);
    printf("usdot: c=0x%x, expected result 0x8\n",c);

	int shift = 0xfcfcfcfc;
	int r, g, b, gray;
    //r = 0xe5abc5cb;
    //g = 0xeb850b65;
    //b = 0x454b256b;
    r = 0x000000cb;
    g = 0x00000065;
    b = 0x0000006b;
    printf("r=0x%x=%d, g=0x%x=%d, b=0x%x=%d\n", r, r, g, g, b, b);
    asm("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tshft_ %0, %1, %2" : "=r"(r) : "r"(r), "r"(shift));
    //asm("scrwr %0"::"r"(scr));
    asm("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tshft_ %0, %1, %2" : "=r"(g) : "r"(g), "r"(shift));
    asm("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tshft_ %0, %1, %2" : "=r"(b) : "r"(b), "r"(shift));
    printf("r=0x%x=%d, g=0x%x=%d, b=0x%x=%d\n", r, r, g, g, b, b);
    asm("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tusadd_ %0, %1, %2" : "=r"(gray) : "r"(r), "r"(g));
    printf("gray=%x=%d\n", gray, gray);
    asm("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tusadd_ %0, %1, %2" : "=r"(gray) : "r"(gray), "r"(b));
    asm("nop");
    printf("gray=%x=%d\n", gray, gray);

    //test saturated mul
    a=0x7f7ffdff;
    b=0x7fff7f80;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
  //c=0x7f81807f
    asm("smul_ %0, %1, %2":"=r"(c):"r"(a), "r"(b));
    //printf("saturate mul: c=%#010x, expected result 0x7f81807f\n",c);
    printf("saturate mul: c=0x%x, expected result 0x7f81807f\n",c);

    int y = 0;
    int x = 0x0209f707;
    int A = 0x03030303;
    int B = 0x05050505;
    int C = 0x0F0F0F0F;
    asm("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tusmul_ %0, %1, %2" : "=r"(y) : "r"(x), "r"(A));  //a*x
    asm("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tsadd_ %0, %1, %2" : "=r"(y) : "r"(y), "r"(B)); //a*x + b
    asm("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tusmul_ %0, %1, %2" : "=r"(y) : "r"(y), "r"(x)); //(a*x + b)*x
    asm("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tssub_ %0, %1, %2" : "=r"(y) : "r"(y), "r"(C)); //(a*x + b)*x + c
    printf("y=0x%x expected result =0x%x%x%x%x\n", y, 7, 17, 183, 167);

    //test nand as not (a nand a = !a)
    a=0xdeadbeaf;
    asm("nand_ %0, %1, %2":"=r"(c):"r"(a), "r"(a));
    //printf("nand: c=%#010x, expected result 0x21524150\n", c);
    printf("nand: c=0x%x, expected result 0x21524150\n", c);

    //test xor reduction
    a=0xfeedcafe;
    asm("nop_xor %0, %1, %2":"=r"(c):"r"(a), "r"(a));
    asm("nop");
    //printf("xor reduce: c=%#010x, expected result 0x00000027\n", c);
    printf("xor reduce: c=0x%x, expected result 0x00000027\n", c);

    //puts("END OF TEST");
    */
    return 0x60A1;
}
