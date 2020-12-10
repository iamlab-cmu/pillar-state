import io
import os

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
