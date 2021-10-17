CC=llvm-project-13.0.0.src/build/bin/clang
CXX=llvm-project-13.0.0.src/build/bin/clang++

all: bufovfl
bufovfl: ./sample/bufovfl.c
	$(CC) -o bufovfl.exe ./sample/bufovfl.c

