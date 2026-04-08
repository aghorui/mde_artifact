# MDE Evaluation Artifact

This software package is meant to demonstrate the validity of the claims made by
the accompanying publication for [Multilevel Deduplication Engine](https://aghorui.github.io/mde/).

All of the tools presented here should be runnable on a standard, recent Linux
machine, however we can only confirm them to be in a working state in the
experimental setup described below. Further explanations and instructions for
each experiment performed are given later in the document.

[**Please download the release here**](https://github.com/aghorui/mde_artifact/releases/latest)

## Quick Start

If you are only interested in replicating the results from the paper, do the
following:

* Start a container from the [Docker image](./mde-artifact-docker.tar.gz):
  `python tool.py shell` (or supply your own commands).

* Change directory to either `/workdir`.

* Run `sudo bash VASCO-LFCPA_time_pmu/run_bulk.sh` for time and peak memory usage statistics.

* Run `sudo bash VASCO-LFCPA_stats/run_bulk.sh` for other statistics like operation count, union, intersection, etc.

Please note that running all of the tests may require at least **48-96 hours of
time** and about **64 GB of RAM** (upper limit) on a recent computer with
**Ubuntu 24.04** and **Docker** installed. Please ensure you have a machine that
matches these specifications.

## Experimental Setup

This package has been tested on the following machine configuration:
* An institute-maintained data center with 2 10-core CPUs, and 256 GB of RAM.
* A virtual machine hosted on the data center with:
  * Ubuntu 22.04,
  * 4 vCPUs,
  * 64 GB of RAM, and
  * 75 GB of disk space.
* GCC 11.4.0, with no optimization flags supplied.
* LLVM-14.

The specification of the hardware, except for the availability of high amount
of RAM, is not important for replicating the trends of each experiment. Hence,
since this setup may be difficult to replicate, any recent machine with a large
amount of RAM should be sufficient.

### Docker Image

We provide a Docker image that will replicate the software testing environment
on a machine that supports docker. The entire docker image is provided in
['mde-artifact-docker.tar.gz'](./mde-artifact-docker.tar.gz).

Please note that the image is based on Ubuntu 24.04 rather than Ubuntu 22.04. We
don't believe the OS version and accompanying userland applications impart a
difference on the experiments.

## Running the Experiments

The experiment files are provided separately from the Docker image in the folder
[`workdir`](./workdir), and is bind-mounted to the container. This allows a user
to freely explore the files without having to go through Docker.

A Python 3 script called `tool.py` is supplied which can be used to
automatically run the required docker commands for setting up the environment.
To start the container, run the following command:

```
python3 tool.py shell
```

For the majority of the time, you would likely want this to be ran as a
background process on which you can attach a shell to check on progress. To do
so, please use the `--background` flag:

```
python3 tool.py shell --background
```

To attach to the container, use:

```
python3 tool.py attach
```

Finally, to clean up after the container is stopped, or to start a new
container, you will have to run the `clean` command. You may have to run this
everytime you want to start a new run:

```
python3 tool.py clean
```

If you do not want to use the script, you must do the following:

* Register the image `mde-artifact-docker.tar.gz` into Docker.
* Start a new container and open a shell.
* Ensure that the session can be detached and run in the background (or as per
  your preferences).
* Change the directory to `/workdir`.

## Supplied MDE Implementation

A known, working version of the MDE library is supplied in
`/workdir/mde`. The experiments should ideally work with
any future MDE version, but a copy is provided here as contingency.

## SPEC Benchmarks

We supply build scripts for selected SPEC benchmarks in
`/workdir/spec2006_build_scripts`. We do not supply the benchmark IR files
themselves as they would be non-redistributable.

## Running Experiments and Expected Results

Since the SPEC CPU 2006 Benchmarks cannot be supplied separately, we supply
alternate data to be ran with the analysis. They are as follows:

* `nano`: GNU Nano text editor, compiled to a single LLVM IR file.
* `bzip2-stock`: The Bzip2 stock source code compiled to a single LLVM IR file.
  This is different from the SPEC CPU 2006 benchmark version.

Both of these programs have been compiled with the
[`wllvm`](https://github.com/travitch/whole-program-llvm) tool. The build
scripts for each are provided in the `thirdparty_sources` archive.

The IR files are generated with `-O0` level of optimization.
You may need to additionally run the `flatten_constexpr_pass` on the generated
IR files supplied with the artifact inside of the docker image. Please refer to
the `/workdir/sources/flatten_constexpr_pass` folder. This ensures that the
instructions within the IR file remain supported by decomposing all
`llvm::ConstExpr` expressions in instructions. Changing the build flags or
omitting passes may change the run-time behavior of VASCO-LFCPA.

### Contents

Contents of the artifact are listed below:

* `sources/`:

  Contains the `flatten_constexpr_pass` LLVM pass mentioned earlier.

* `mde/`:

  Implementation of MDE as a library.

* `SLIM/`:

  Implementation of the `SLIM` LLVM helper library.

* `spec2006_build_scripts/`:

  Build scripts for selected SPEC benchmarks used in the evaluation.

* `VASCO-LFCPA_time_pmu/`

  Implementation of VASCO-LFCPA with configuration set up for measuring time and
  peak memory usage. The naive, ZDD, Sparse Bit-Vector, Single-level and
  Multi-level MDE implementations of the data representations are available
  in this folder.

* `VASCO-LFCPA_time_pmu/`

**In each `VASCO-LFCPA_*` directory, the following files are available:**

* `past_outputs/`
  Contains data from past runs of the analysis.

* `run_new.sh`

  Sets up the experiment and runs it for the specified IR file in the
  `workdir/VASCO-LFCPA_*/samples` folder. Note that this script requires
  **superuser access** for installing SLIM onto the system for VASCO-LFCPA. This
  can be circumvented by either installing SLIM manually and editing the script
  to remove the installation routine, or updating the CMake build scripts in
  VASCO-LFCPA such that it links against a locally-available build of SLIM.

  For the sake of simplicity and not disturbing the existing configurations too
  much, we have left this part of the process as-is.

* `run_bulk.sh`

  Runs all available experiments. Invokes `run_new.sh`.

**To run the experiments:** Run `bash run_bulk.sh` in the respective folders.
Output logs will be generated in the same directory.

**Expected time:** ~96 hours for entire run. Individual times vary.

### Notes on VASCO-LFCPA

The VASCO-LFCPA C++ implementation we integrate MDE on is a work-in-progress
effort. It is implemented on top of LLVM-14's infrastructure, with an in-house
abstraction layer called SLIM (Simplified LLVM IR Modeling). Because of this,
the results of the analysis may not necessarily be correct for all programs.
Reimplementing this analysis from scratch is infeasible in a reasonable time
frame and is beyond the scope of this work. Instead, we present this evaluation
as an observation of the performance of MDE within a realistic and
representative setting, demonstrating its impact under practical constraints.

Due to the usage of non-deterministic data structures (like
`std::unordered_map`) and the program objects being internally
represented with pointers, statistics such as instructions processed and
operations performed are not constant with multiple runs on the same input.
They are, however, consistent within a certain range of error.

The application currently only supports LLVM IR files created from C programs.
For many real-world programs the implementation also requires the corresponding
LLVM IR files used as input to be given a set of special transformations in
order to accommodate features not covered by it, such as decomposition of
multi-argument Get Element Pointer instructions, constant expressions in
instructions, and so on. The SPEC benchmark programs are the only ones where
there has been substantial testing with this implementation, and hence are
the ones used for evaluation.

To accommodate the sparse bit-vector and ZDD-based implementations, the code has
been modified to add a mapping routine that assigns a unique integer identifier
to each variable object present in VASCO-LFCPA, which are represented using
pointers. Adding a mapping is a one-time cost and subsequent look-ups to the
same pointer or integer have no additional cost besides the look-up itself.

To actually receive analysis output from VASCO-LFCPA, the flag `--SHOW_OUTPUT`
must be passed to `run_new.sh`. Please see `run_bulk.sh` for invocation
examples.

## Other Features in the MDE Project

If you would like to experiment with MDE itself, we suggest the following lines
of inquiry.

* **Cache-like Hits and Misses**:

  This one is mentioned in the paper, and can be enabled with the compile-time
  switch `MDE_ENABLED_PERFORMANCE_STATISTICS`. One may obtain outputs that look
  like the following:

```
Performance Profile:
unions
      Hits       : 932
      Equal Hits : 608
      Subset Hits: 0
      Empty Hits : 1732
      Cold Misses: 99
      Edge Misses: 0

differences
      Hits       : 808
      Equal Hits : 731
      Subset Hits: 0
      Empty Hits : 1385
      Cold Misses: 57
      Edge Misses: 50

intersections
      Hits       : 1322
      Equal Hits : 465
      Subset Hits: 0
      Empty Hits : 1702
      Cold Misses: 53
      Edge Misses: 56

property_sets
      Hits       : 10338
      Equal Hits : 0
      Subset Hits: 0
      Empty Hits : 0
      Cold Misses: 278
      Edge Misses: 0


Profiler Statistics:
    'register_set': 188.121 ms
    'set_difference': 3.967 ms
    'set_intersection': 3.913 ms
    'set_union': 5.156 ms
    'store_subset': 0.328 ms
```

* **Parallelism, Set Eviction and SERIALIZATION**:

  These can be enabled with flags `MDE_ENABLE_PARALLEL`,
  `MDE_ENABLE_EVICTION`, and `MDE_ENABLE_SERIALIZATION`. Please do keep in mind
  that these are nascent features and have not yet been substantially used or
  tested. These may contain bugs.

* **Unit Tests**:

  There are various unit tests written for MDE using the GTest framework. These
  are present in the `src/tests/` directory of the
  MDE source tree. These may be useful for illustrations of constraints,
  edge-cases, and usage demonstrations.

We recommend going through the sourcecode and documentation for a fuller
picture, and the CMake build script for a full list of available compile-time
flags.

## License
Unless otherwise specified in individual directories of `/workdir`, and
excluding the `thirdparty_sources` archive supplied in this package, the
work is licenced under GNU Affero General Public License Version 3.

```
Copyright (C) 2025 Anamitra Ghorui and Aditi Raste

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>.
```

Please refer to [LICENSE](./LICENSE) for the full license text.