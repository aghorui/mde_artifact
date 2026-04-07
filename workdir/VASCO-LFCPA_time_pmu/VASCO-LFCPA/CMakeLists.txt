cmake_minimum_required(VERSION 3.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 17)
project(iplfcpa)

# we need LLVM_HOME in order not automatically set LLVM_DIR
set(ENV{LLVM_DIR} /usr/lib/llvm-14/lib/cmake/llvm)

option(PRINT "To run the print statements" OFF)

if (PRINT)
    add_definitions(-DPRINT=1)
endif()

find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
add_definitions($ENV{LFCPA_DEFS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

include(GNUInstallDirs)
include_directories(/usr/local/include/slim)
include_directories("./include")
#include_directories(slim/include)
add_library(slim SHARED IMPORTED)
set_target_properties(slim PROPERTIES IMPORTED_LOCATION "/usr/local/lib/libslim.so")
#llvm_map_components_to_libnames(llvm_libs ${LLVM_TARGETS_TO_BUILD} support core irreader codegen mc mcparser option passes)
llvm_map_components_to_libnames(llvm_libs ${LLVM_TARGETS_TO_BUILD} all)
#execute_process(COMMAND llvm-config --libs all OUTPUT_VARIABLE llvm_libs)

#add_subdirectory(slim)

#add_executable(${PROJECT_NAME} main.cpp)
add_compile_options("-g" "-Ofast")

set(SOURCE_FILES
    "include/TransitionGraph.cpp"
    "lfcpa.cpp"
    "FSSP_Kill.cpp"
    "FISP_Gen.cpp"
    "FISP_Def.cpp"
    "Profiling.cpp"
    "CommonAll.cpp")

if(DEFINED ENV{NAIVE_MODE})
    list(APPEND SOURCE_FILES "CommonNaive.cpp")
    add_definitions(-DNAIVE_MODE=1)
elseif(DEFINED ENV{MDE_MODE})
    list(APPEND SOURCE_FILES "Common.cpp")
    add_definitions(-DMDE_MODE=1)
elseif(DEFINED ENV{SINGLE_LEVEL_MODE})
    list(APPEND SOURCE_FILES "CommonSingleLevel.cpp")
    add_definitions(-DSINGLE_LEVEL_MODE=1)
elseif(DEFINED ENV{ZDD_MODE})
    list(APPEND SOURCE_FILES "ZDDWrapper.cpp")
    add_definitions(-DZDD_MODE=1)
elseif(DEFINED ENV{SPARSEBV_MODE})
    add_definitions(-DSPARSEBV_MODE=1)
endif()

add_executable(
    ${PROJECT_NAME}
    ${SOURCE_FILES}
)


target_link_libraries(${PROJECT_NAME} slim LLVM cudd)