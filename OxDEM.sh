#!/bin/bash
# ==========================================================
# OxDEM automated execution script
# Author: Dr Fatih Uzun
# Purpose: Build, run, post-process, and archive simulation
# ==========================================================

# Create a temporary working directory
myFolder=$(mktemp -d pbg_XXXXX)

# Copy project files
cp post_processing.py $myFolder
cp -a include $myFolder
cp -a src $myFolder
cp -a lib $myFolder
cp CMakeLists.txt $myFolder

cd $myFolder || exit

# ==========================================================
# Build with CMake (uses precompiled coreSolver.o or .a)
# ==========================================================
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
cd ..

# ==========================================================
# Run the OxDEM simulation
# ==========================================================
./bin/OxDEM > simulation_output.txt 2>&1

# ==========================================================
# Collect and organize results
# ==========================================================
cd ..
mv "$myFolder/solution.log" "$myFolder/" 2>/dev/null || true
cd "$myFolder" || exit

mkdir -p time_frame

# ==========================================================
# Post-processing with Python
# ==========================================================
python3 post_processing.py

# ==========================================================
# Package results
# ==========================================================
cd ..
zip -r -m "${myFolder}.zip" "$myFolder"

echo "✅ Simulation complete. Results archived in ${myFolder}.zip"