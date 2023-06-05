#==============================================================================
# Preamble
#==============================================================================
cmake_minimum_required(VERSION 3.24)
set(PCDATA_VERSION 0.1.0)
project(PCData.Main VERSION ${PCDATA_VERSION} LANGUAGES CXX)

#==============================================================================
# Set compiler settings
#==============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(IGNORE_ME ${CMAKE_C_COMPILER}) # Suppress CMake warning message

#==============================================================================
# Add subdirectories
#==============================================================================
add_subdirectory(PCData)
add_subdirectory(demo)

