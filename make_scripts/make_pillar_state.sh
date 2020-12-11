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

# Remove directory if we clean first
if [ "$clean_build_dir" = "1" ]; then
    rm -rf build
fi

# Build
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config -- -j$nproc

cd ..
