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

# This method was adapted from code in
#  https://github.com/albumentations-team/albumentations
def get_version():
    current_dir = os.path.abspath(os.path.dirname(__file__))
    version_file = os.path.join(current_dir, "pillar_state_py", "__init__.py")
    with io.open(version_file, encoding="utf-8") as f:
        return re.search(r'^__version__ = [\'"]([^\'"]*)[\'"]', f.read(), re.M).group(1)

def _do_setup():
    packages = find_packages('.')
    package_files = collect_files('pillar_state_py/_bindings/linux-x86_64')

    setup(name='pillar_state_py',
          version=get_version(),
          author='Timothy Lee, Jacky Liang',
          author_email='tel1@andrew.cmu.edu',
          description='Library for state space modeling with PILLAR.',
          packages=packages,
          package_data={
              'pillar_state_py': package_files
          },
          install_requires = [
            'numpy', 'pytest'
          ],
         )

_do_setup()
