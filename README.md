# locationConsistancy
LC LLVM Work

original files were taken from [Adrian Samson](https://www.cs.cornell.edu/~asampson/blog/llvm.html) blog and its [examples](https://github.com/sampsyo/llvm-pass-skeleton)

#Initial run commands
Here is how I am running this code so far

```shell
mkdir build
cd build
LLVM_DIR=/usr/local/lib64/cmake/llvm/ cmake ..
make
cd ..
clang -Xclang -load -Xclang build/skeleton/libSkeletonPass.so example.c
```

