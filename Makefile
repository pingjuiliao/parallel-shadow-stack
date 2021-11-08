CC=llvm-project-13.0.0.src/build/bin/clang
CXX=llvm-project-13.0.0.src/build/bin/clang++

LOADLIB=-L./lib -lShadowStack -Wl,-rpath,./lib 
.PHONY: all lib bufovfl
all: bufovfl inline fibo test
test: ./sample/test.c
	$(CC) -o test.exe ./sample/test.c -O0
bufovfl: ./sample/bufovfl.c lib
	$(CC) -o bufovfl.exe ./sample/bufovfl.c ${LOADLIB}
inline: ./sample/inline.c lib
	$(CC) -o inline.exe ./sample/inline.c -O1 ${LOADLIB}
fibo:  ./sample/fibonacci.c lib
	$(CC) -o fibo.exe ./sample/fibonacci.c -O1 ${LOADLIB}
cppbufovfl: ./sample/cppBufovfl.cpp lib 
	$(CXX) -o cppBufovfl.exe ./sample/cppBufovfl.cpp ${LOADLIB}
lib: ./lib/init.c
	clang -shared -o ./lib/libShadowStack.so ./lib/init.c
clean:
	rm *.exe ./lib/libShadowStack.so
