#==============================================================================
# Preamble
#==============================================================================
cmake_minimum_required(VERSION 3.24)
project(PCData.Demo VERSION ${PCDATA_VERSION} LANGUAGES CXX)

#==============================================================================
# Set compiler settings
#==============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#==============================================================================
# Add executable target
#==============================================================================
add_executable(demo demo.cpp)
target_link_libraries(demo PRIVATE PCDataLib)
file(COPY_FILE ../database/ChemSep8.32.xml ${CMAKE_CURRENT_BINARY_DIR}/pcd.db)