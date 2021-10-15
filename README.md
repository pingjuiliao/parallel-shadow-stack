# Shadow stack
## It's still a traditional shadow stack, not a parallel shadow stack

## Setup
```
## Unix Makefiles build
./setup.py
```

## Run 
```
# Use the new clang binary
./llvm-project-13.0.0.src/build/bin/clang ./sample/bufovfl.c -o bufovfl
```
As it suppose to protect buffer overflow protection
