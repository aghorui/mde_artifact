/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o mcf.ll -DSPEC_CPU -DNDEBUG  -DWANT_STDC_PROTO  -std=gnu90 -fno-strict-aliasing       -DSPEC_CPU_LP64         mcf.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o mcfutil.ll -DSPEC_CPU -DNDEBUG  -DWANT_STDC_PROTO  -std=gnu90 -fno-strict-aliasing       -DSPEC_CPU_LP64         mcfutil.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o readmin.ll -DSPEC_CPU -DNDEBUG  -DWANT_STDC_PROTO  -std=gnu90 -fno-strict-aliasing       -DSPEC_CPU_LP64         readmin.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o implicit.ll -DSPEC_CPU -DNDEBUG  -DWANT_STDC_PROTO  -std=gnu90 -fno-strict-aliasing       -DSPEC_CPU_LP64         implicit.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o pstart.ll -DSPEC_CPU -DNDEBUG  -DWANT_STDC_PROTO  -std=gnu90 -fno-strict-aliasing       -DSPEC_CPU_LP64         pstart.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o output.ll -DSPEC_CPU -DNDEBUG  -DWANT_STDC_PROTO  -std=gnu90 -fno-strict-aliasing       -DSPEC_CPU_LP64         output.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o treeup.ll -DSPEC_CPU -DNDEBUG  -DWANT_STDC_PROTO  -std=gnu90 -fno-strict-aliasing       -DSPEC_CPU_LP64         treeup.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o pbla.ll -DSPEC_CPU -DNDEBUG  -DWANT_STDC_PROTO  -std=gnu90 -fno-strict-aliasing       -DSPEC_CPU_LP64         pbla.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o pflowup.ll -DSPEC_CPU -DNDEBUG  -DWANT_STDC_PROTO  -std=gnu90 -fno-strict-aliasing       -DSPEC_CPU_LP64         pflowup.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o psimplex.ll -DSPEC_CPU -DNDEBUG  -DWANT_STDC_PROTO  -std=gnu90 -fno-strict-aliasing       -DSPEC_CPU_LP64         psimplex.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o pbeampp.ll -DSPEC_CPU -DNDEBUG  -DWANT_STDC_PROTO  -std=gnu90 -fno-strict-aliasing       -DSPEC_CPU_LP64         pbeampp.c
/usr/lib/llvm-14/bin/llvm-link mcf.ll mcfutil.ll readmin.ll implicit.ll pstart.ll output.ll treeup.ll pbla.ll pflowup.ll psimplex.ll pbeampp.ll                -o mcf.ll

/usr/lib/llvm-14/bin/opt -S -instnamer -mem2reg mcf.ll -o mcf.ll
