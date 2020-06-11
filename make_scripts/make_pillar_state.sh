# TODO: add --clean option
cd pillar_state

# Build
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config -- -j$nproc

cd ../..
