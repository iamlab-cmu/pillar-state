cmake_minimum_required (VERSION 3.4)

# Read version from text file
# https://stackoverflow.com/questions/47066115/cmake-get-version-from-multiline-text-file
file(READ "../version.txt" ver)

string(REGEX MATCH "VERSION_MAJOR ([0-9]*)" _ ${ver})
set(ver_major ${CMAKE_MATCH_1})

string(REGEX MATCH "VERSION_MINOR ([0-9]*)" _ ${ver})
set(ver_minor ${CMAKE_MATCH_1})

string(REGEX MATCH "VERSION_PATCH ([0-9]*)" _ ${ver})
set(ver_patch ${CMAKE_MATCH_1})

message("version: ${ver_major}.${ver_minor}.${ver_patch}")

project (pillar_state
  VERSION ${ver_major}.${ver_minor}.${ver_patch}
  LANGUAGES CXX
)

# message("PILLAR State version: ${PROJECT_VERSION}")

# Flags ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# TODO: is this needed?
add_compile_options(-Wall -Wextra)

# Dependencies ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
find_package(Protobuf REQUIRED)
#add_subdirectory(proto)
find_package(yaml-cpp REQUIRED)

# Library ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
add_library(pillar_state SHARED
  include/pillar_state/proto/pillar_state.pb.cc # TODO: this should probably be moved out of the include
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories(${PROTOBUF_INCLUDE_DIR})
include_directories(include)
include_directories(pybind11/include)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

target_link_libraries(pillar_state PUBLIC
  #proto
  #${Protobuf_LIBRARIES}
  ${PYTHON_LIBRARIES}
  yaml-cpp # TODO: add_library this before?
)

# Note line below is needed if for cmake below 3.6
# previously, you could link to ${Protobuf_LIBRARIES} and call it a day
# https://stackoverflow.com/questions/41043184/cmake-doesnt-work-with-google-protobuf
target_link_libraries(pillar_state INTERFACE ${PROTOBUF_LIBRARIES})

target_include_directories(pillar_state PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Installation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
include(GNUInstallDirs)
install(TARGETS pillar_state
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  # RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Tests ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
	../test/pillar_state_test.cpp
)

target_link_libraries(pillar_state_test
  pillar_state
  gtest_main
)

add_test(NAME pillar_state_test COMMAND pillar_state_test)

# Bindings ######################################
add_subdirectory(pybind11)
pybind11_add_module(pillar_state_py
  src/py_bindings.cpp
)

target_link_libraries(pillar_state_py PRIVATE
  #${Protobuf_LIBRARIES}
  pillar_state
)