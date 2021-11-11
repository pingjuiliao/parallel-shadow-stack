# Shadow stack
## It's still a traditional shadow stack, not a parallel shadow stack

## Setup
```
## Unix Makefiles build
./setup.py
```

## build programs with shadow stack 
```
## use makefile, this includes -Wl flags to load libShadowStack.so
make
```

## run the program
```
# because shadow stack is initialized in c default constructor
./test

```

As it suppose to protect buffer overflow protection
