# FindGuorbi.cmake
#
# Instructions for using this file:
# 1. Copy this file, 'FindGurobi.cmake', to the directory '${CMAKE_SOURCE_DIR}/cmake/'.
# 2. Set Gurobi's install path. By default it is '/opt/gurobi/linux64/'.
#   If it is not installed there, you can do one of the following:
#   2.1. Set the environment variable 'GUROBI_HOME' to the directory where Gurobi is installed.
#   2.2. Set the variable 'GUROBI_HOME' to the directory where Gurobi is installed.
# 3. Set the variable 'GUROBI_VERBOSE' to 'ON' if you want to see some extra info of this file.
# 4. Add the following lines to your 'CMakeLists.txt' file:
#   set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
#   find_package(Gurobi REQUIRED)
#   target_link_libraries(${ANY_LIBRARY_OF_YOURS} PUBLIC Gurobi::CXX)
#
# Targets created by this file:
#   1. Gurobi::C: The Gurobi C library.
#   2. Gurobi::CXX: The Gurobi C++ library.
#
# Variables set by this file:
#   1. GUROBI_INCLUDE_DIR: The directory where the header files of Gurobi are located.
#   2. GUROBI_C_LIBRARY: The path to the Gurobi C library.
#   3. GUROBI_CXX_LIBRARY: The path to the Gurobi C++ library.
#   4. GUROBI_FOUND: True if Gurobi is found, false otherwise.
# Normally, one does not need to use these variables directly,
# because one can use the targets Gurobi::C and Gurobi::CXX.

cmake_minimum_required(VERSION 3.26)

# auxiliary variables
set(GUROBI_DEFAULT_INSTALL_DIR "/opt/gurobi1000/linux64/")
set(GUROBI_HINTS ${GUROBI_HOME} $ENV{GUROBI_HOME} ${GUROBI_DEFAULT_INSTALL_DIR})
set(GUROBI_C_HEADER gurobi_c.h)
set(GUROBI_CXX_HEADER gurobi_c++.h)

# find Gurobi

find_path(
    GUROBI_INCLUDE_DIR
    NAMES ${GUROBI_C_HEADER} ${GUROBI_CXX_HEADER}
    HINTS ${GUROBI_HINTS}
    PATH_SUFFIXES include
    REQUIRED
    DOC "directory with the header files of Gurobi"
)

find_library(
    GUROBI_C_LIBRARY
    NAMES gurobi gurobi81 gurobi90 gurobi95 gurobi100 gurobi110
    HINTS ${GUROBI_HINTS}
    PATH_SUFFIXES lib
    REQUIRED
    DOC "Gurobi C library"
)

if(${CMAKE_COMPILER_IS_GNUCXX} AND (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.8")) # GCC >=4.8: Use libgurobi_g++5.2.a for newer C++ compilers
    find_library(
        GUROBI_CXX_LIBRARY
        NAMES gurobi_g++4.8
        HINTS ${GUROBI_HINTS}
        PATH_SUFFIXES lib
        REQUIRED
        DOC "Gurobi C++ library"
    )
else()
    find_library(
        GUROBI_CXX_LIBRARY
        NAMES gurobi_g++5.2 gurobi_g++8.5
        HINTS ${GUROBI_HINTS}
        PATH_SUFFIXES lib
        REQUIRED
        DOC "Gurobi C++ library"
    )
endif()

mark_as_advanced(GUROBI_INCLUDE_DIR GUROBI_C_LIBRARY GUROBI_CXX_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    Gurobi
    REQUIRED_VARS
        GUROBI_INCLUDE_DIR
        GUROBI_C_LIBRARY
        GUROBI_CXX_LIBRARY
)

# logs
if(GUROBI_VERBOSE)
    message("")
    message("Find Gurobi result:")
    message("GUROBI_INCLUDE_DIR = ${GUROBI_INCLUDE_DIR}")
    message("GUROBI_C_LIBRARY = ${GUROBI_C_LIBRARY}")
    message("GUROBI_CXX_LIBRARY = ${GUROBI_CXX_LIBRARY}")
    message("GUROBI_FOUND = ${GUROBI_FOUND}")
    message("")
endif()

# check if header files exist
if(GUROBI_FOUND)
    if(NOT EXISTS "${GUROBI_INCLUDE_DIR}/${GUROBI_C_HEADER}")
        message(FATAL_ERROR "Gurobi C header file not found: ${GUROBI_INCLUDE_DIR}/${GUROBI_C_HEADER}")
    endif()
    if(NOT EXISTS "${GUROBI_INCLUDE_DIR}/${GUROBI_CXX_HEADER}")
        message(FATAL_ERROR "Gurobi C++ header file not found: ${GUROBI_INCLUDE_DIR}/${GUROBI_CXX_HEADER}")
    endif()
endif()

# set targets
if((NOT TARGET Gurobi::C) AND (NOT TARGET Gurobi::CXX) AND GUROBI_FOUND)
    add_library(Gurobi::C UNKNOWN IMPORTED)
    set_target_properties(Gurobi::C PROPERTIES IMPORTED_LOCATION ${GUROBI_C_LIBRARY})
    target_include_directories(Gurobi::C SYSTEM INTERFACE ${GUROBI_INCLUDE_DIR})
    target_link_libraries(Gurobi::C INTERFACE ${GUROBI_C_LIBRARY})

    add_library(Gurobi::CXX UNKNOWN IMPORTED)
    set_target_properties(Gurobi::CXX PROPERTIES IMPORTED_LOCATION ${GUROBI_CXX_LIBRARY})
    target_link_libraries(Gurobi::CXX INTERFACE Gurobi::C)
endif()
