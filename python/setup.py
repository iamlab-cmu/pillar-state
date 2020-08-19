import sys
import os

from setuptools import setup, find_packages

def collect_files(target_dir):
    file_list = []
    for (root, dirs, files) in os.walk(target_dir,followlinks=True):
        for filename in files:
            file_list.append(os.path.join('..', root, filename))
    return file_list

def _do_setup():
    packages = find_packages('.')
    package_files = collect_files('pillar_state_py/_bindings/linux-x86_64')

    setup(name='pillar_state_py',
          version='0.0.0',
          author='Timothy Lee, Jacky Liang',
          author_email='tel1@andrew.cmu.edu',
          description='Library for state space modeling with PILLAR.',
          packages=packages,
          package_data={
              'pillar_state_py': package_files
          },
          install_requires = [
            'numpy',
          ],
         )

_do_setup()
