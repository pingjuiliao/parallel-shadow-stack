#include <iostream>
using namespace std; 
void bufovfl(void) ;
// end .h
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
    cin >> buf ;
    puts(buf) ;
    
}
