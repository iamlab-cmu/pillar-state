cmake_minimum_required (VERSION 3.4)
project (pillar-state
    VERSION 0.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# TODO: is this needed?
add_compile_options(-Wall -Wextra)

## Dependencies
find_package(Protobuf REQUIRED)

add_subdirectory(proto)

include_directories(${PROTOBUF_INCLUDE_DIR})

# ## Library
# add_library(pillar-state SHARED
# )

# # include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../franka-interface-common")
# # include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../libfranka")
# include_directories("${CMAKE_CURRENT_BINARY_DIR}")
# include_directories(${PROTOBUF_INCLUDE_DIR})
# include_directories(include)

# target_link_libraries(pillar-state PUBLIC
#     proto
#     ${Protobuf_LIBRARIES}
# )

# target_include_directories(pillar-state PUBLIC
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#   $<INSTALL_INTERFACE:include>
# )

# ## Installation
# include(GNUInstallDirs)
# install(TARGETS pillar-state
#   # EXPORT FrankaTargets
#   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#   # RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

# install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
