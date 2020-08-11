# pillar-state

# Requirements

## Install Protobuf

Instructions are also described: https://github.com/protocolbuffers/protobuf/blob/master/src/README.md

```
sudo apt install autoconf automake libtool curl make g++ unzip
```

Download the latest protobuf release (not pre-release) for all languages: https://github.com/protocolbuffers/protobuf/releases

For example, for release 3.12.4, download the `protobuf-all-3.12.4.tar.gz` file. Then extract it.

```
cd path/to/protobuf-3.12.4
./configure
make
make check
sudo make install
sudo ldconfig # refresh shared library cache.
```

# Build

## CPP

Do this to build (optional `-c | --clean`):

`./make_scripts/make_proto_cpp.sh --clean`
`./make_scripts/make_view_cpp.sh --clean`

Run tests:

`./cpp/build/pillar_state_test`

## Python

TBD
