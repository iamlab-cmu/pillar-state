# Script variables
clean_build_dir=

# http://linuxcommand.org/lc3_wss0120.php
usage()
{
    echo "usage: make_pillar_state [[-c] | [-h]]"
    echo " -c | --clean: removes build directory before building"
}

# Parse input arguments
while [ "$1" != "" ]; do
    case $1 in
        -c | --clean )          shift
                                clean_build_dir=1
                                ;;
        -h | --help )           usage
                                exit
                                ;;
        * )                     usage
                                exit 1
    esac
    shift
done

cd cpp

# Remove directory if we clean first
if [ "$clean_build_dir" = "1" ]; then
    rm -rf build
fi

# Build
# https://stackoverflow.com/questions/24174394/cmake-is-not-able-to-find-python-libraries
mkdir -p build && cd build
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DPYTHON_INCLUDE_DIR=$(python -c "from distutils.sysconfig import get_python_inc; print(get_python_inc())")  \
    -DPYTHON_LIBRARY=$(python -c "import distutils.sysconfig as sysconfig; import os; print(os.path.join(sysconfig.get_config_var('LIBDIR'), sysconfig.get_config_var('LDLIBRARY')))") \
    -DPYBIND11_PYTHON_VERSION=$(python -c "import sys; print(sys.version[:3])")
cmake --build . --config -- -j$nproc

cd ../..

# Copy .so files to python bindings
echo "Copying shared libraries to python bindings"
src_dir=cpp/build/ 
target_dir=python/pillar_state/_bindings/linux-x86_64

mkdir -p $target_dir

cp $src_dir\/libpillar_state.so $target_dir
cp $src_dir\/pillar_state_py.*.so $target_dir\/pillar_state_py.so
