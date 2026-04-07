set -o errexit
set -o nounset
set -x

PROJECT_ROOT=$(pwd)
run_analysis_mde() {
	echo "@@@@@@@@@ RUNNING FOR $1 MDE @@@@@@@@@"
	bash ./run_new.sh "$1" mde --FUNPTR 2>&1 | tee "output_$1_funptr_mde.txt"
}

run_analysis_singlelevel() {
	echo "@@@@@@@@@ RUNNING FOR $1 SINGLE_LEVEL @@@@@@@@@"
	bash ./run_new.sh "$1" singlelevel --FUNPTR 2>&1 | tee "output_$1_funptr_singlelevel.txt"
}

run_analysis_naive() {
	echo "@@@@@@@@@ RUNNING FOR $1 NAIVE @@@@@@@@@"
	bash ./run_new.sh "$1" naive --FUNPTR 2>&1 | tee "output_$1_funptr_naive.txt"
}

run_analysis_zdd() {
	echo "@@@@@@@@@ RUNNING FOR $1 ZDD @@@@@@@@@"
	bash ./run_new.sh "$1" zdd --FUNPTR 2>&1 | tee "output_$1_funptr_zdd.txt"
}

run_analysis_sparsebv() {
	echo "@@@@@@@@@ RUNNING FOR $1 SPARSEBV @@@@@@@@@"
	bash ./run_new.sh "$1" sparsebv --FUNPTR 2>&1 | tee "output_$1_funptr_sparsebv.txt"
}

run_variants() {
	run_analysis_naive "$1"
	run_analysis_singlelevel "$1"
	run_analysis_mde "$1"
	run_analysis_zdd "$1"
	run_analysis_sparsebv "$1"
}

run_variants_non_naive() {
	run_analysis_singlelevel "$1"
	run_analysis_mde "$1"
	run_analysis_zdd "$1"
	run_analysis_sparsebv "$1"
}

run_variants "bzip2-stock"
run_variants "nano"

set +x
