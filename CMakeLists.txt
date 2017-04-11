# This file is part of libReimu.
# Copyright (C) 2016-2017  CloudyReimu <cloudyreimu@gmail.com>
#
# libReimu is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libReimu is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with libReimu.  If not, see <http://www.gnu.org/licenses/>.


cmake_minimum_required(VERSION 2.8)
project(libReimu)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ReimuCXXFlags} -std=c++14")

set(SOURCE_FILES IPEndPoint/IPEndPoint.cpp IPEndPoint/IPEndPoint.hpp libReimu.hpp Exception/Exception.cpp Exception/Exception.hpp)


add_library(Reimu SHARED ${SOURCE_FILES} SQLAutomator/SQLAutomator.cpp SQLAutomator/SQLAutomator.hpp SQLAutomator/ColumnSpec.cpp UniversalType/UniversalType.cpp UniversalType/UniversalType.hpp SQLAutomator/SQLite3/SQLite3.cpp Tasker/Tasker.cpp Tasker/Tasker.hpp Base64/Base64.cpp Base64/Base64.hpp FileSystem/FileSystem.cpp FileSystem/FileSystem.hpp)
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

add_executable(reimu_unitest Tests.cpp)
target_link_libraries(reimu_unitest Reimu sqlite3 pthread)
set_property(SOURCE Tests.cpp APPEND_STRING PROPERTY COMPILE_FLAGS " -I. ")