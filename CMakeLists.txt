cmake_minimum_required(VERSION 3.22)

project(player LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ADD MinimalSocket LIBRARY

include(FetchContent)
set(BUILD_MinimalCppSocket_SAMPLES OFF CACHE BOOL "" FORCE) # you don't want the samples in this case
FetchContent_Declare(
min_sock
GIT_REPOSITORY https://github.com/andreacasalino/Minimal-Socket
GIT_TAG        master
)
FetchContent_MakeAvailable(min_sock)

# ADD PATH FOR FINDING HEADERS
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/_deps/min_sock-src/src/header)

# ADD PATH FOR FINDING LIBRARIES
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/_deps/min_sock-src/build)

# Set source files
set(SOURCE_FILES
    main.cpp
    functions.cpp
    CosteVSRecompensa.cpp
)

add_executable(player ${SOURCE_FILES})

# LINK TO MinimalSocket
target_link_libraries(player MinimalSocket)

install(TARGETS player
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)