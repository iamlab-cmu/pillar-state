# Script variables
clean_build_dir=

# http://linuxcommand.org/lc3_wss0120.php
usage()
{
    echo "usage: make_proto_cpp [[-c] | [-h]]"
    echo " -c | --clean: removes build files before building"
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

PROTO_OUT_DIR=cpp/include/pillar_state/proto/

# Remove directory if we clean first
if [ "$clean_build_dir" = "1" ]; then
    rm -rf $PROTO_OUT_DIR
fi

# Build
mkdir -p $PROTO_OUT_DIR
protoc -I proto/ --cpp_out=$PROTO_OUT_DIR proto/pillar_state.proto
