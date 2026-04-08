#!/bin/bash

CC="$LLVM_HOME/bin/clang-14"
OPT="$LLVM_HOME/bin/opt-14"
DRIVER="./build/iplfcpa"

LFCPA_DEFS=`python3 ./HandleArgs.py ${@:3}`

if [[ "$?" -ne 0 ]]; then
    echo "Argument handler exited with error. Aborting."
    exit 1
fi

if [[ "$2" == "naive" ]]; then
    echo "Enabling NAIVE Mode"
    NAIVE_MODE=1
    export NAIVE_MODE
elif [[ "$2" == "mde" ]]; then
    echo "Enabling MDE Mode"
    MDE_MODE=1
    export MDE_MODE
elif [[ "$2" == "singlelevel" ]]; then
    echo "Enabling Single-Level MDE Mode"
    SINGLE_LEVEL_MODE=1
    export SINGLE_LEVEL_MODE
elif [[ "$2" == "zdd" ]]; then
    echo "Enabling ZDD Mode"
    ZDD_MODE=1
    export ZDD_MODE
elif [[ "$2" == "sparsebv" ]]; then
    echo "Enabling SPARSEBV Mode"
    SPARSEBV_MODE=1
    export SPARSEBV_MODE
else
    echo "Error: Unknown mode '$2'"
fi

export LFCPA_DEFS

echo "CMake Flags: ${LFCPA_DEFS}"

mkdir -p build
cd build
make clean
cmake ..
make
cd ..

FILENAME=$(basename $1 .c)

echo -e "=======================Generating LL Files using CLANG======================="

rm -f *.ll
$OPT -S -instnamer -mem2reg -mergereturn ./samples/$FILENAME.ll -o $FILENAME.ll

echo -e "==================Done Generating LL Files using CLANG======================="
echo -e "==============================Running Analysis===============================\n"

TIME_EXEC=/usr/bin/time
"$TIME_EXEC" -f 'Wallclock Time: %e, Max RSS (KB): %M' $DRIVER $FILENAME.ll

#gdb --ex run --args $DRIVER ./samples/$FILENAME.ll


echo -e "\n"
echo -e "==============================Analysis Over================================="
