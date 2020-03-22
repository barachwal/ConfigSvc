#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(ConfigSvcExamples)

include_directories(${lConfigSvc_INCLUDE_DIR})

add_executable(ex1 ex1/ex1.cc)
target_link_libraries(ex1 lConfigSvc)