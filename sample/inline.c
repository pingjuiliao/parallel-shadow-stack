#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static inline void
swap(unsigned int* x, unsigned int* y) {
    unsigned int tmp = *x;
    *x = *y ;
    *y = tmp ;
}

void performInlineSwap(unsigned int n) ;

int
main(int argc, char** argv) {
    char buf[20];
    unsigned int x;
    memset(buf, 0 , sizeof(buf)) ;
    fgets(buf, 20, stdin);
    x = (unsigned int) atoi(buf);
    performInlineSwap(x ) ;
    return 0 ; 
}

void 
performInlineSwap(unsigned int n) { 
    unsigned int x = 1, y = 2;

    for ( unsigned int i = 0; i < n; i++ ) {
        y += i ;
    }
    printf("x: %d, y: %d\n",x, y);
    puts("Swap !!");
    swap(&x, &y );
    printf("x: %d, y: %d\n",x, y);
    return; 
}
