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
# cxxopts
CPMAddPackage(
    GITHUB_REPOSITORY jarro2783/cxxopts
    VERSION 3.1.1
    OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)
# Gurobi
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
find_package(Gurobi REQUIRED)
list(REMOVE_AT CMAKE_MODULE_PATH -1)
# boost
find_package(Boost COMPONENTS timer REQUIRED)

if(Boost_FOUND)
    add_library(boost INTERFACE IMPORTED)
    set_target_properties(boost PROPERTIES IMPORTED_LOCATION ${Boost_LIBRARIES})
    target_include_directories(boost SYSTEM INTERFACE ${Boost_INCLUDE_DIRS})
    target_link_libraries(boost INTERFACE ${Boost_LIBRARIES})
endif()

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(knapsack-solver ${SOURCE_DIR}/knapsack-solver.cpp)
target_include_directories(knapsack-solver PRIVATE $SOURCE_DIR)
target_link_libraries(knapsack-solver PRIVATE Brkga_mp_ipr::Brkga_mp_ipr cxxopts::cxxopts Gurobi::CXX boost)
target_compile_options(knapsack-solver PRIVATE -ggdb3)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(knapsack-solver PRIVATE DEBUG)
endif()
