cmake_minimum_required(VERSION 3.10)
project(ComplexityAnalyzer)

# Set LLVM and Clang directories
set(LLVM_DIR "/usr/lib/llvm-10/lib/cmake/llvm")
set(CLANG_DIR "/usr/lib/llvm-10/lib/cmake/clang")

# Find LLVM and Clang packages
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

# Include directories
include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})

# Specify library directories
link_directories(/usr/lib/llvm-10/lib)

# Add the executable
add_executable(ComplexityAnalyzer ComplexityAnalyzer.cpp)

target_link_libraries(ComplexityAnalyzer
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangRewrite
  clangRewriteFrontend
  clangDriver
  clangParse
  clangSerialization
  clangSema
  clangEdit
  clangAnalysis
  clangLex
  clangTooling # Add this library
  LLVM-10 # If needed, depending on your setup
)

# LLVM specific settings
set_target_properties(ComplexityAnalyzer PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
