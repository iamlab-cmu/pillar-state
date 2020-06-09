# pillar-state

# Clone the directory
`git clone --recursive git@github.com:iamlab-cmu/pillar-state.git`
`cd pillar-state`
`git branch develop`
`git submodule update --recursive --remote`

The last command grabs the latest `pybind11` code.

# Build

Recommend creating a virtual environment first:

`virtualenv -p /usr/bin/python3 ~/envs/pillar-state`
`source ~/envs/pillar-state/bin/activate`

Then install:

`pip install .`

(You could install it with `-e`, but because the code is in C++ under the hood, it doesn't make much sense to use editable mode.)

This should compile and build the C++ code and the Python bindings.

If you get a bunch of errors about `CMake Error: The current CMakeCache.txt directory ... is different than the directory ... where CMakeCache.txt was created`, then manually remove the `build/` directory and try again.

If you want to only compile the C++ code, run `./make_pillar_state.sh` and then you can see an example C++ program by running `./build/pillar_state_exec`

# Examples

```
>>> import pillar_state_py
>>> state = pillar_state_py.State()
Hello there.
>>> state.has_high_ground()
True
```

# TBD

Clean up output of CMake etc by putting into different build/bin folders
