cmake_minimum_required (VERSION 3.4)
project (pillar_state
  VERSION 0.0.0
  LANGUAGES CXX
)

# Flags #########################################
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Package dependencies ##########################
find_package(Eigen3 3.3 REQUIRED)

# Create library ################################
add_library(pillar_state SHARED
  src/pillar_state.cc
)

include_directories(pillar_state
  include
  ${EIGEN3_INCLUDE_DIR}
)

# Create executable #############################
add_executable(pillar_state_exec
  src/pillar_state_exec.cc
)

target_link_libraries(pillar_state_exec
  pillar_state
)

# Tests #########################################
enable_testing()

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(pillar_state_test
	test/pillar_state_test.cc
)
target_link_libraries(pillar_state_test
  gtest_main
)
add_test(NAME pillar_state_test COMMAND pillar_state_test)

# # Bindings ######################################
# add_subdirectory(pybind11)
# pybind11_add_module(pillar_state_py
#   src/py_bindings.cc
# )
