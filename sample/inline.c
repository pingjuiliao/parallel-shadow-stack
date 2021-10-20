#include <stdio.h>

static inline void
swap(int* x, int* y) {
    int tmp = *x;
    *x = *y ;
    *y = tmp ;
}
int fibonacci(int n) ;
void performInlineSwap(void) ;

int
main(int argc, char** argv) {
    performInlineSwap() ;
    return 0 ; 
}

int
fibonacci(int n) {
    if ( n <= 1 ) return 1 ;
    return fibonacci(n-1) + fibonacci(n-2) ;
}

void 
performInlineSwap(void) {
    int x = 1, y = fibonacci(16);
    printf("x: %d, y: %d\n",x, y);
    puts("Swap !!");
    swap(&x, &y );
    printf("x: %d, y: %d\n",x, y);
    return; 
}
