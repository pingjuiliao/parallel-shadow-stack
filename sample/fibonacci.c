#include <stdio.h>
#include <stdint.h>
uint64_t
fibo(uint64_t n) {
    if (n <= 1) return 1 ;
    return fibo(n-1) + fibo(n-2) ;
}

int
main(int argc, char** argv) {
    uint64_t r = 0;
    r = fibo(30);
    printf("fibonacci(30) == %lu\n", r) ;
    return 0 ;
}
