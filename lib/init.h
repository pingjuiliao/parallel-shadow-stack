#include <asm/prctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
int arch_prctl(int code, unsigned long *addr) ;
void __attribute__((constructor)) init_shadow_stack(void) ;
