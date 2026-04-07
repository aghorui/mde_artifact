set -o errexit
set -o nounset
set -x

MDE_SOURCE=$(realpath ../mde)
PROJECT_ROOT=$(pwd)

# Install MDE
cd "$MDE_SOURCE"
mkdir -p "build"
cd "build"
rm -rf *
cmake .. -DENABLE_EXAMPLES=0
make -j4
make install

# Install SLIM
cd "$PROJECT_ROOT/../SLIM"
mkdir -p "build"
cd "build"
rm -rf *
cmake .. -DDISABLE_IGNORE_EFFECT=1
make -j4
make install

# Run VASCO-LFCPA
cd "$PROJECT_ROOT/VASCO-LFCPA"
mkdir -p build
./run.sh $@

set +x