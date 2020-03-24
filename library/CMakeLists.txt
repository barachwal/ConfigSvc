#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(lConfigSvc)

set(HEADERS_DIR ${PROJECT_SOURCE_DIR}/include)
set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories(${HEADERS_DIR})

set(SOURCES ${SOURCES_DIR}/ModuleConfig.cc
            ${SOURCES_DIR}/Configurable.cc
            ${SOURCES_DIR}/ConfigSvc.cc)

# This needs to be done universally to any static library
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Define the library
add_library(${PROJECT_NAME} STATIC ${SOURCES} )

# Expose project's public includes to other subprojects through cache variable.
set(${PROJECT_NAME}_INCLUDE_DIR ${HEADERS_DIR} CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)