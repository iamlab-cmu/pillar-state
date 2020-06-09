# Get CPU core count
n_cores=$(grep ^cpu\\scores /proc/cpuinfo | uniq |  awk '{print $4}')

# Make build folder, exiting if there's a problem
mkdir -p build || exit 1

# Make and test package
cd build
cmake -j$n_cores -DCMAKE_BUILD_TYPE=Release .. && make
ctest
cd ..
