#include<stdio.h>
#include<string.h>

void manyChar(void) ;
void manyAsm(void) ;
int 
main(int argc, char **argv) {
    manyChar() ;
    manyAsm() ;
    return 0 ;
}

void
manyChar(void) {
    char a, b, c ;
    char buf[27] ;

    memset(buf, 0 ,sizeof(buf)) ;
    a = 'c' ;
    b = 'c' ;
    char x = 'y' ;
}


void
manyAsm(void) {
    unsigned long int foo= 1337 ; // 8 byte 
    unsigned long int bae= 1314 ; 
    // break the alignment of %rbp, %rsp
    __asm__ volatile(
        "subq $1, %%rsp;\n"
        "subq $3, %%rbp;\n"
        "push %%rsp    ;\n"
        "pop %0        ;\n"
        "push %%rbp    ;\n"
        "pop %1        ;\n"
        : "=a"(foo), "=b"(bae)
        : "a"(foo), "b"(bae)
    );
    printf("foo: %lu\n", foo) ;
    printf("bae: %lu\n", bae) ;
}
