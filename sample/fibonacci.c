#include <stdio.h>
#include <stdint.h>
uint64_t
fibo(uint64_t n) {
    if (n <= 1) return 1 ;
    return fibo(n-1) + fibo(n-2) ;
}

int
main(int argc, char** argv) {
    uint64_t r ;
    r = fibo(30);
    printf("%lu\n", r) ;
    return 0 ;
}
