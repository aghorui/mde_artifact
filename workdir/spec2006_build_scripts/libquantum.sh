/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o classic.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O2 -fno-strict-aliasing       -DSPEC_CPU_LP64 -DSPEC_CPU_LINUX        classic.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o complex.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O2 -fno-strict-aliasing       -DSPEC_CPU_LP64 -DSPEC_CPU_LINUX        complex.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o decoherence.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O2 -fno-strict-aliasing       -DSPEC_CPU_LP64 -DSPEC_CPU_LINUX        decoherence.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o expn.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O2 -fno-strict-aliasing       -DSPEC_CPU_LP64 -DSPEC_CPU_LINUX        expn.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o gates.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O2 -fno-strict-aliasing       -DSPEC_CPU_LP64 -DSPEC_CPU_LINUX        gates.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o matrix.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O2 -fno-strict-aliasing       -DSPEC_CPU_LP64 -DSPEC_CPU_LINUX        matrix.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o measure.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O2 -fno-strict-aliasing       -DSPEC_CPU_LP64 -DSPEC_CPU_LINUX        measure.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o oaddn.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O2 -fno-strict-aliasing       -DSPEC_CPU_LP64 -DSPEC_CPU_LINUX        oaddn.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o objcode.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O2 -fno-strict-aliasing       -DSPEC_CPU_LP64 -DSPEC_CPU_LINUX        objcode.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o omuln.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O2 -fno-strict-aliasing       -DSPEC_CPU_LP64 -DSPEC_CPU_LINUX        omuln.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o qec.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O2 -fno-strict-aliasing       -DSPEC_CPU_LP64 -DSPEC_CPU_LINUX        qec.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o qft.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O2 -fno-strict-aliasing       -DSPEC_CPU_LP64 -DSPEC_CPU_LINUX        qft.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o qureg.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O2 -fno-strict-aliasing       -DSPEC_CPU_LP64 -DSPEC_CPU_LINUX        qureg.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o shor.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O2 -fno-strict-aliasing       -DSPEC_CPU_LP64 -DSPEC_CPU_LINUX        shor.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o version.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O2 -fno-strict-aliasing       -DSPEC_CPU_LP64 -DSPEC_CPU_LINUX        version.c
/usr/lib/llvm-14/bin/clang -c -g -S -emit-llvm -o specrand.ll -DSPEC_CPU -DNDEBUG    -std=gnu90 -O2 -fno-strict-aliasing       -DSPEC_CPU_LP64 -DSPEC_CPU_LINUX        specrand.c
/usr/lib/llvm-14/bin/llvm-link -S       classic.ll complex.ll decoherence.ll expn.ll gates.ll matrix.ll measure.ll oaddn.ll objcode.ll omuln.ll qec.ll qft.ll qureg.ll shor.ll version.ll specrand.ll                -o libquantum.ll

