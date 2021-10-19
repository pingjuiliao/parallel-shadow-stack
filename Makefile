CC=llvm-project-13.0.0.src/build/bin/clang
CXX=llvm-project-13.0.0.src/build/bin/clang++

all: bufovfl inline
bufovfl: ./sample/bufovfl.c
	$(CC) -o bufovfl.exe ./sample/bufovfl.c
inline: ./sample/inline.c
	$(CC) -o inline.exe ./sample/inline.c -O1
clean:
	rm *.exe
