#include "init.h"
#define STACKSIZE 0x100000
#define QWORDSIZE 0x8



int 
arch_prctl(int code, unsigned long *addr)
{    
    return syscall(SYS_arch_prctl, code, addr);    
}


void
__attribute__((constructor)) init_shadow_stack(void) {
    void *gs_base, *stack_base, *stack_top ; 
    gs_base    = mmap(NULL, 0x1000, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) ;
    arch_prctl(ARCH_SET_GS, (unsigned long *) gs_base) ;
    stack_base = mmap(NULL, STACKSIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) ;
    stack_top  = stack_base + STACKSIZE - QWORDSIZE;
    __asm__ volatile(
            "movq %0, %%gs:0x108"
            : /* no output */  
            : "r"(stack_top)
            );
}
