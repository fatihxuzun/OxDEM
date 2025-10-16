# ==============================================================
# OxDEM: Discrete Element Method Enhanced by Strain Energy Formulation
# Author: Fatih Uzun
# DOI: https://doi.org/10.1016/j.triboint.2025.110933
# ==============================================================
cmake_minimum_required(VERSION 3.10)
project(OxDEM VERSION 1.0 LANGUAGES CXX)

# ==============================================================
# Build configuration
# ==============================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

option(USE_OPENMP "Enable OpenMP parallelization" ON)
option(BUILD_SHARED_LIBS "Build shared library instead of static" OFF)

# ==============================================================
# Include directories
# ==============================================================
include_directories(${PROJECT_SOURCE_DIR}/include)

# ==============================================================
# Library linking (secure precompiled solver)
# ==============================================================
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# Try linking precompiled library first
if (EXISTS ${LIB_DIR}/OxDEM_library.o)
    message(STATUS "Using precompiled OxDEM library: lib/OxDEM_library.o")
    add_library(OxDEM_lib STATIC IMPORTED)
    set_target_properties(OxDEM_lib PROPERTIES IMPORTED_LOCATION ${LIB_DIR}/OxDEM_library.o)
else()
    message(WARNING "No precompiled solver found — compiling OxDEM_library.cpp directly.")
    add_library(OxDEM_lib ${SRC_DIR}/OxDEM_library.cpp)
endif()

# ==============================================================
# Main executable
# ==============================================================
add_executable(OxDEM ${SRC_DIR}/OxDEM.cpp)
target_link_libraries(OxDEM PRIVATE OxDEM_lib)

# ==============================================================
# OpenMP support
# ==============================================================
if (USE_OPENMP)
    find_package(OpenMP)
    if (OpenMP_CXX_FOUND)
        target_link_libraries(OxDEM PRIVATE OpenMP::OpenMP_CXX)
        message(STATUS "OpenMP parallelization enabled.")
    else()
        message(WARNING "OpenMP not found — compiling without parallel support.")
    endif()
endif()

# ==============================================================
# Output directories
# ==============================================================
set_target_properties(OxDEM PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)