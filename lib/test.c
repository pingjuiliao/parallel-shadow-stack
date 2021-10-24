#include <stdio.h>
#include <asm/prctl.h>
#include <sys/syscall.h>
#include <unistd.h>
#include "init.h"

int
main(int argc, char** argv) {
    void* p = NULL;
    arch_prctl(ARCH_GET_GS, p) ;
    printf("%p\n", p);
    return 0 ;
}
