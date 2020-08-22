"""Pillar State bindings wrapper module"""

from __future__ import print_function, division, absolute_import

import importlib
import json
import sys
import os


def _format_path(pathstr):
    if os.name == "nt":
        # need to flip backslashes and convert to lower case
        return pathstr.replace("\\", "/").lower()
    else:
        return pathstr


def _import_active_version():
    major = sys.version_info[0]
    minor = sys.version_info[1]

    # print("Running with Python %d.%d" % (major, minor))

    # get the directory that contains the native libs
    lib_dir = os.path.realpath(os.path.join(os.path.dirname(__file__), "_bindings"))

    if os.name == "nt":
        platform = "windows-x86_64"
        ext = "pyd"
        comp = ''
    else:
        platform = "linux-x86_64"
        ext = "so"
        comp = 'x86_64-linux-gnu'

    module_name = "pillar_state_py"
    module_dir = os.path.join(lib_dir, platform)
    module_path = os.path.join(module_dir, "%s.%s" % (module_name, ext))
    package_path = "pillar_state_py._bindings.%s.%s" % (platform, module_name)

    # print(module_name)
    # print(module_dir)
    # print(module_path)
    # print(package_path)

    if os.path.isfile(module_path):
        sys.path.append(module_path)

        print("Importing module '%s' (%s)" % (module_name, module_path))
        module = importlib.import_module(package_path)
        # https://stackoverflow.com/questions/28826127/dynamically-load-all-names-from-module-in-python
        # Now extract the attributes into the globals() namespace, as 'from ... import *' would do.
        # A module can define __all__ to explicitly allow all symbols to be imported.
        # Otherwise, we import all names that do not start with an underscore.
        if hasattr(module, "__all__"):
            attrs = {key: getattr(module, key) for key in module.__all__}
        else:
            attrs = {key: value for key, value in module.__dict__.items() if key[0] != "_"}
        globals().update(attrs)

    else:
        raise RuntimeError("No pillar state module found for the active version of Python (%d.%d)" % (major, minor))


_import_active_version()
