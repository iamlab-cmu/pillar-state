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

## Install Python 3.6

This version of `pillar-state` requires Python 3.6. This may be relaxed in a future release.

```
sudo apt install python3.6 python3.6-dev
```

## Install other dependencies

The following installs dependencies that are separate from Protobuf:

```
sudo apt install libyaml-cpp-dev
```

# (Recommended) Create and source virtual env

Create a Python 3.6 virtual environment. The following commands will create a virtual environment named `pillar-state` in `~/envs`. However, you can change the location or name of the virtual environment if you prefer.

```
virtualenv -p /usr/bin/python3.6 ~/envs/pillar-state
```

Source the environment:

```
source ~/envs/pillar-state/bin/activate
```

This ensures that the `pip install` command in the next step installs to the correct environment.

# Build

## Clone this repo and its submodules:

`git clone --recursive git@github.com:iamlab-cmu/pillar-state.git`

## C++11

Note: You don't need to compile the C++ view if you're just using the Python view.
The repo comes with prebuilt C++ shared libraries for Python.

Do this to build (optional `-c | --clean`):

`./make_scripts/make_proto_cpp.sh --clean`
`./make_scripts/make_view_cpp.sh --clean`

Run tests:

`./cpp/build/pillar_state_test`

## Python

`pip install -e python`

# Run Tests

## C++

`./cpp/build/pillar_state_test`

## Python

`pytest --capture=no test/pillar_state_py_test.py`
