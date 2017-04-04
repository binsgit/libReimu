cmake_minimum_required(VERSION 2.8)
project(libReimu)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ReimuCXXFlags} -std=c++14")

set(SOURCE_FILES IPEndPoint/IPEndPoint.cpp IPEndPoint/IPEndPoint.hpp libReimu.hpp Exception/Exception.cpp Exception/Exception.hpp)


add_library(Reimu SHARED ${SOURCE_FILES} SQLAutomator/SQLAutomator.cpp SQLAutomator/SQLAutomator.hpp SQLAutomator/ColumnSpec.cpp UniversalType/UniversalType.cpp UniversalType/UniversalType.hpp SQLAutomator/SQLite3/SQLite3.cpp Tasker/Tasker.cpp Tasker/Tasker.hpp)
add_library(Reimu_static STATIC ${SOURCE_FILES})

set_target_properties(Reimu_static PROPERTIES OUTPUT_NAME Reimu)

file(GLOB_RECURSE REIMU_HEADERS "${PROJECT_SOURCE_DIR}/*/*.hpp")

set(BASE "${PROJECT_SOURCE_DIR}/")

foreach(ITEM ${REIMU_HEADERS})
    get_filename_component(ITEM_PATH ${ITEM} PATH)
    string(REPLACE ${BASE} "" ITEM_PATH ${ITEM_PATH})
    install(FILES ${ITEM} DESTINATION include/libReimu/${ITEM_PATH})
endforeach()

install(FILES libReimu.hpp DESTINATION include/libReimu)

install(TARGETS Reimu Reimu_static DESTINATION lib)