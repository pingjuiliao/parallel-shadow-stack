# Shadow stack
## It's still a traditional shadow stack, not a parallel shadow stack

## Setup
```
## Unix Makefiles build
./setup.py
```

## build programs with shadow stack 
```
# Use the new clang binary
./llvm-project-13.0.0.src/build/bin/clang ./sample/bufovfl.c -o bufovfl.exe

## or makefile
make
```

## run the program
```
# because shadow stack is initialized in c default constructor
LD_PRELOAD=./lib/init.so ./bufovfl.exe

# DONT JUST RUN THE BINARY
./bufovfl.exe # segmentation fault
```

As it suppose to protect buffer overflow protection
