#include <stdio.h>

static inline void
swap(int* x, int* y) {
    int tmp = *x;
    *x = *y ;
    *y = tmp ;
}

void performInlineSwap(void) ;

int
main(int argc, char** argv) {
    performInlineSwap() ;
    return 0 ; 
}


void 
performInlineSwap(void) {
    int x = 1, y = 2;
    printf("x: %d, y: %d\n",x, y);
    puts("Swap !!");
    swap(&x, &y );
    printf("x: %d, y: %d\n",x, y);
    return; 
}
