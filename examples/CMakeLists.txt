#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(ConfigSvcExamples)

include_directories(${lConfigSvc_INCLUDE_DIR})

add_definitions(-DEXAMPLES=\"${PROJECT_SOURCE_DIR}\")

add_executable(ex1 ex1/ex1.cc)
add_executable(ex2 ex2/ex2.cc)
target_link_libraries(ex1 lConfigSvc)
target_link_libraries(ex2 lConfigSvc)