import io
import os
import re
import sys

from setuptools import setup, find_packages

def collect_files(target_dir):
    file_list = []
    for (root, dirs, files) in os.walk(target_dir,followlinks=True):
        for filename in files:
            file_list.append(os.path.join('..', root, filename))
    return file_list

# Duplication of function in pillar_state_py.utilities
# Move out to a separate general file reading library
def read_verion_from_file(version_file):
    assert os.path.exists(version_file), \
        "Expected version file \"{}\" to exist, but it does not.".format(version_file)

    with io.open(version_file, encoding="utf-8") as f:
        # Major version
        major_ver_line_split = f.readline().split()
        assert major_ver_line_split[0] == 'VERSION_MAJOR'
        ver_major = major_ver_line_split[1]

        # Minor version
        minor_ver_line_split = f.readline().split()
        assert minor_ver_line_split[0] == 'VERSION_MINOR'
        ver_minor = minor_ver_line_split[1]

        # Patch version
        patch_ver_line_split = f.readline().split()
        assert patch_ver_line_split[0] == 'VERSION_PATCH'
        ver_patch = patch_ver_line_split[1]

    version = '{}.{}.{}'.format(ver_major, ver_minor, ver_patch)
    return version

def pillar_state_version():
    current_dir = os.path.abspath(os.path.dirname(__file__))
    version_file = os.path.join(current_dir, "..", "version.txt")
    return read_verion_from_file(version_file)

def _do_setup():
    packages = find_packages('.')
    package_files = collect_files('pillar_state/_bindings/linux-x86_64')

    setup(name='pillar_state',
          version=pillar_state_version(),
          author='Timothy Lee, Jacky Liang',
          author_email='tel1@andrew.cmu.edu',
          description='Library for state space modeling with PILLAR.',
          packages=packages,
          package_data={
              'pillar_state': package_files
          },
          install_requires = [
            'numpy', 'pytest'
          ],
         )

_do_setup()
