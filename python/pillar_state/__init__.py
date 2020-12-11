import os

from .utilities import *

current_dir = os.path.dirname(__file__)
version_file = os.path.abspath(os.path.join(current_dir, "..", "..", "version.txt"))
__version__ = read_verion_from_file(version_file)

from .pillar_state import State
