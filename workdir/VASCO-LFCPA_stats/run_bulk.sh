set -o errexit
set -o nounset
set -x

PROJECT_ROOT=$(pwd)
run_analysis() {
	echo "@@@@@@@@@ RUNNING FOR $1 @@@@@@@@@"
	bash ./run_new.sh "$1" --FUNPTR  2>&1 | tee "output_$1_funptr.txt"
}

run_analysis_naive() {
	echo "@@@@@@@@@ RUNNING FOR $1 @@@@@@@@@"
	bash ./run_new.sh "$1" --FUNPTR --NAIVE_MODE  2>&1 | tee "output_$1_funptr_naive.txt"
}

run_analysis "bzip2-stock"
run_analysis "nano"

set +x
