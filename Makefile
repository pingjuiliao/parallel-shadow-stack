CC=llvm-project-13.0.0.src/build/bin/clang
CXX=llvm-project-13.0.0.src/build/bin/clang++
.PHONY: all lib clean
all: bufovfl inline fibo lib
bufovfl: ./sample/bufovfl.c
	$(CC) -o bufovfl.exe ./sample/bufovfl.c
inline: ./sample/inline.c
	$(CC) -o inline.exe ./sample/inline.c -O1
fibo:  ./sample/fibonacci.c  
	$(CC) -o fibo.exe ./sample/fibonacci.c -O1
lib: ./lib/init.c
	clang -shared -o ./lib/init.so ./lib/init.c
clean:
	rm *.exe ./lib/init.so
