cmake_minimum_required(VERSION 3.12)
set(CODE_VERSION 0.0.1)
project(code VERSION ${CODE_VERSION})

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# set(CMAKE_C_COMPILER "/usr/local/gcc-7.2.0/bin/gcc") set(CMAKE_CXX_COMPILER
# "/usr/local/gcc-7.2.0/bin/g++")

set(default_build_type "Release")
message(
  STATUS "Setting build type to '${default_build_type}' as none was specified.")
set(CMAKE_BUILD_TYPE
    "${default_build_type}"
    CACHE STRING "Choose the type of build." FORCE)
# Set the possible values of build type for cmake-gui
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                                             "MinSizeRel" "RelWithDebInfo")

find_package(Threads REQUIRED)
set(CMAKE_CXX_FLAGS "-pthread ${CMAKE_CXX_FLAGS}") # force pthread to
# avoid bugs in some cmake setups
set(CMAKE_C_FLAGS "-pthread ${CMAKE_C_FLAGS}")

set(ADDITIONAL_CXX_FLAGS
    "-Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -Wno-deprecated-declarations -Wno-write-strings"
)
set(ADDITIONAL_C_FLAGS
    "-Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -Wno-deprecated-declarations -Wno-write-strings"
)

set(CMAKE_CXX_FLAGS "${ADDITIONAL_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${ADDITIONAL_C_FLAGS} ${CMAKE_C_FLAGS}")
string(REGEX REPLACE "-O0" "-Og" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
string(REGEX REPLACE "-O3" "-Ofast" CMAKE_CXX_FLAGS_RELEASE
                     ${CMAKE_CXX_FLAGS_RELEASE})
string(REGEX REPLACE "-O0" "-Og" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
string(REGEX REPLACE "-O3" "-Ofast" CMAKE_C_FLAGS_RELEASE
                     ${CMAKE_C_FLAGS_RELEASE})

file(GLOB headers "${CMAKE_CURRENT_LIST_DIR}/src/*.h")
list(APPEND headers ${CMAKE_CURRENT_LIST_DIR}/arguments.h)

file(GLOB sources "${CMAKE_CURRENT_LIST_DIR}/src/*.cc"
     "${CMAKE_CURRENT_LIST_DIR}/src/*.c")
list(APPEND sources ${CMAKE_CURRENT_LIST_DIR}/arguments.c
     ${CMAKE_CURRENT_LIST_DIR}/showUsage.c)
list(APPEND sources ${CMAKE_CURRENT_LIST_DIR}/main.cc)

add_executable(code ${sources} ${headers})

target_link_libraries(code PRIVATE Threads::Threads)
