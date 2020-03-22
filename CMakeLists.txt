# Setup the project
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(ConfigSvc)

#----------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_DEFAULT 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

#----------------------------------------------------------------------------
# Top level option:
option(BUILD_EXAMPLES "Build examples of ConfigSvc library usage" ON)

#----------------------------------------------------------------------------
# Configure the components (subprojects)
# NOTE: The order matters! The most independent ones should go first.
#       If A and B really depend on each other, CMake  can build such correctly,
#       if you specify full path to the include directlry
add_subdirectory(library)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


#----------------------------------------------------------------------------
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})
message(STATUS "Build project with usage examples: " ${BUILD_EXAMPLES})