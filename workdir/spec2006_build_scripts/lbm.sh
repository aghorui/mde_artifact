/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o lbm.ll -DSPEC_CPU     -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         lbm.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o main.ll -DSPEC_CPU     -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         main.c
/usr/lib/llvm-11/bin/llvm-link -S   lbm.ll main.ll  -o lbm.ll

/usr/lib/llvm-11/bin/opt -S -instnamer -mem2reg lbm.ll -o lbm.ll
