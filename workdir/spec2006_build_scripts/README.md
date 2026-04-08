SPEC CPU 2006 Benchmark Build Scripts
=====================================

This directory contains the build scripts for creating LLVM IR files for
selected SPEC benchmarks. To use these, place them in the `src` folder of
the corresponding benchmark (e.g. `benchspec/CPU2006/401.bzip2/src`), and run
the script.

Please note that the IR files were originally built using LLVM-11 rather than
LLVM-14, and thus may have differences from the ones used in evaluation.
However the trends of running the analysis on these files remains the same.