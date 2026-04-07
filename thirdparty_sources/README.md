IR Build Scripts
================

This directory contains sources for the `nano` and `bzip2` samples used to run
VASCO-LFCPA on. Compiled versions are already included in this repository to
use. This also contains sourcecode for the
[CUDD BDD/ZDD library](https://github.com/cuddorg/cudd) which is a dependency
in VASCO-LFCPA for the ZDD-based implementation.

No copyright is claimed on the above projects and are provided here solely for
research and exploratory use. Licenses for each individual project are provided
in their respective source directories.

These are based on the source code fetched from the Ubuntu/Debian repositories.

## Building

The Python program `wllvm` (https://github.com/travitch/whole-program-llvm) is
required as a prerequisite. It can be installed with `pip` or `pipx`:

```
pipx install wllvm
```

Each directory has a `build.sh` file that contains all of the required commands
for the build setup. Running this should be enough to generate the LLVM IR
files.

```
bash build.sh
```

## Additional Processing

You may need to additionally run the `flatten_constexpr_pass` on the generated
IR files supplied with the artifact inside of the docker image. Please refer to
the `/workdir/sources/flatten_constexpr_pass` folder. This ensures that the
instructions within the IR file remain supported by decomposing all
`llvm::ConstExpr` expressions in instructions.