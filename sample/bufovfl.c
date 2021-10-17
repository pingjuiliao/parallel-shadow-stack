#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bufovfl(void) ;
int
main(int argc, char ** argv) {
    bufovfl() ;
    return 0;
}

void 
bufovfl(void) {
    char buf[10] ;
    char* r ;
    puts("Type anything to overflow this buffer") ;
    if ( (r = fgets(buf, 50, stdin)) == NULL) {
        exit(-1) ;
    }

    puts(buf) ;
    
}
