cmake_minimum_required(VERSION 3.14)

project(
    knapsack
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(cmake/get_cpm.cmake)
# Brkga_mp_ipr
CPMAddPackage(
  NAME Brkga_mp_ipr
  VERSION 3.0.0
  URL https://github.com/ceandrade/brkga_mp_ipr_cpp/archive/refs/tags/v3.0.0.tar.gz
  DOWNLOAD_ONLY YES
)
if(Brkga_mp_ipr_ADDED)
    add_library(Brkga_mp_ipr::Brkga_mp_ipr INTERFACE IMPORTED)
    target_include_directories(Brkga_mp_ipr::Brkga_mp_ipr INTERFACE "${Brkga_mp_ipr_SOURCE_DIR}/brkga_mp_ipr")
endif()
# Gurobi
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
find_package(Gurobi REQUIRED)
list(REMOVE_AT CMAKE_MODULE_PATH -1)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(main ${SOURCE_DIR}/main.cpp)
target_include_directories(main PRIVATE $SOURCE_DIR)
target_link_libraries(main PRIVATE Brkga_mp_ipr::Brkga_mp_ipr Gurobi::CXX)
target_compile_options(main PRIVATE -ggdb3)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(main PRIVATE DEBUG)
endif()
