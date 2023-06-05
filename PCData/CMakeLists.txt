#==============================================================================
# Preamble
#==============================================================================
cmake_minimum_required(VERSION 3.24)
project(PCData.Library VERSION ${PCDATA_VERSION} LANGUAGES CXX)
include(FetchContent)

#==============================================================================
# Set compiler settings
#==============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#==============================================================================
# Import PugiXML
#==============================================================================
FetchContent_Declare(pugi
        GIT_REPOSITORY https://github.com/zeux/pugixml.git
        GIT_TAG a0e0643
        )
FetchContent_MakeAvailable(pugi)

#==============================================================================
# Import JSON library
#==============================================================================
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG bc889af
        )
FetchContent_MakeAvailable(json)

#==============================================================================
# Import ETL library
#==============================================================================
FetchContent_Declare(
        etl
        GIT_REPOSITORY https://github.com/ETLCPP/etl.git
        GIT_TAG ac236c9
)

FetchContent_MakeAvailable(etl)

add_library(PCDataLib STATIC PCData.cpp)
target_include_directories(PCDataLib SYSTEM PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
target_link_libraries(PCDataLib PRIVATE pugixml-static nlohmann_json::nlohmann_json etl::etl)
