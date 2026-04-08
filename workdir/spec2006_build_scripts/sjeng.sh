/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o attacks.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         attacks.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o book.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         book.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o crazy.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         crazy.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o draw.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         draw.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o ecache.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         ecache.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o epd.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         epd.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o eval.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         eval.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o leval.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         leval.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o moves.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         moves.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o neval.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         neval.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o partner.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         partner.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o proof.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         proof.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o rcfile.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         rcfile.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o search.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         search.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o see.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         see.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o seval.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         seval.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o sjeng.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         sjeng.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o ttable.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         ttable.c
/usr/lib/llvm-11/bin/clang -c -g -S -emit-llvm -o utils.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O0 -Xclang -disable-O0-optnone -fno-strict-aliasing       -DSPEC_CPU_LP64         utils.c
/usr/lib/llvm-11/bin/llvm-link -S   attacks.ll book.ll crazy.ll draw.ll ecache.ll epd.ll eval.ll leval.ll moves.ll neval.ll partner.ll proof.ll rcfile.ll search.ll see.ll seval.ll sjeng.ll ttable.ll utils.ll                     -o sjeng.ll

/usr/lib/llvm-11/bin/opt -S -instnamer -mem2reg sjeng.ll -o sjeng.ll
