cmake -B build -S . -DCMAKE_BUILD_TYPE="Release"
cmake --build build -j 2
echo "Press any key to exit..."
read quit
