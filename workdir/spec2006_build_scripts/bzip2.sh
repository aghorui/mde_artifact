/usr/lib/llvm-14/bin/clang -g -S -emit-llvm -O0 -Xclang -disable-O0-optnone -fno-discard-value-names -c -o  spec.ll   spec.c
/usr/lib/llvm-14/bin/clang -g -S -emit-llvm -O0 -Xclang -disable-O0-optnone -fno-discard-value-names -c -o     blocksort.ll blocksort.c
/usr/lib/llvm-14/bin/clang -g -S -emit-llvm -O0 -Xclang -disable-O0-optnone -fno-discard-value-names -c -o bzip2.ll bzip2.c
/usr/lib/llvm-14/bin/clang -g -S -emit-llvm -O0 -Xclang -disable-O0-optnone -fno-discard-value-names -c -o bzlib.ll bzlib.c
/usr/lib/llvm-14/bin/clang -g -S -emit-llvm -O0 -Xclang -disable-O0-optnone -fno-discard-value-names -c -o compress.ll compress.c
/usr/lib/llvm-14/bin/clang -g -S -emit-llvm -O0 -Xclang -disable-O0-optnone -fno-discard-value-names -c -o         crctable.ll crctable.c
/usr/lib/llvm-14/bin/clang -g -S -emit-llvm -O0 -Xclang -disable-O0-optnone -fno-discard-value-names -c -o         decompress.ll decompress.c
/usr/lib/llvm-14/bin/clang -g -S -emit-llvm -O0 -Xclang -disable-O0-optnone -fno-discard-value-names -c -o         huffman.ll huffman.c
/usr/lib/llvm-14/bin/clang -g -S -emit-llvm -O0 -Xclang -disable-O0-optnone -fno-discard-value-names -c -o         randtable randtable.c
/usr/lib/llvm-14/bin/llvm-link -S spec.ll blocksort.ll bzip2.ll bzlib.ll compress.ll crctable.ll decompress.ll huffman.ll randtable.ll                     -o bzip2.ll
/usr/lib/llvm-14/bin/opt -S -instnamer -mem2reg -mergereturn bzip2.ll -o bzip2.ll
