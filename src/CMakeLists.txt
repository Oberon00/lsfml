# Part of the LSFML library -- Copyright (c) Christian Neumüller 2012--2013
# This file is subject to the terms of the BSD 2-Clause License.
# See LICENSE.txt or http://opensource.org/licenses/BSD-2-Clause

set (LSFML_HDRS_PUBLIC
    "string.hpp"
    vector.hpp)

set (LSFML_HDRS
    open_util.hpp)

set (LSFML_SOURCES
    open.cpp
    open_system.cpp)

# http://stackoverflow.com/a/13104057/2128694
macro(prepend_to_all _srcs _path)
    unset(_tmp)
    foreach(src ${${_srcs}})
        list(APPEND _tmp ${_path}${src})
    endforeach()
    set(${_srcs} ${_tmp})
endmacro()

prepend_to_all(LSFML_HDRS_PUBLIC "../include/lsfml/")

set(COMP_DEFS "SFML_STATIC")

if (MSVC)
    add_definitions(
        /W4     # warning level 4
        /MP)    # multicore compilation
    #list(APPEND COMP_DEFS "_CRT_SECURE_NO_WARNINGS" "_SCL_SECURE_NO_WARNINGS")

    # Disable auto-linkage
    list(APPEND COMP_DEFS  "BOOST_ALL_NO_LIB")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
    # see http://stackoverflow.com/a/11004675/2128694
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        execute_process(
            COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
        if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
            message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
        endif ()
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_definitions("-Weverything"
            "-Wno-weak-vtables"
            "-Wno-padded"
            "-Wno-covered-switch-default"
            "-Wno-c++98-compat" "-Wno-c++98-compat-pedantic")
        list(APPEND LSFML_HDRS "clang_stdlib_config.hpp")
        set(stdlibcfg "${CMAKE_CURRENT_SOURCE_DIR}/clang_stdlib_config.hpp")
        add_definitions("-DBOOST_STDLIB_CONFIG=\"${stdlibcfg}\""
            "-D_GLIBCXX_USE_WCHAR_T")
    else ()
        message(FATAL_ERROR "Your C++ compiler is unknown.")
    endif ()
endif ()

if (WIN32)
    find_package(SFML REQUIRED system-s graphics-s window-s audio-s main)
else ()
    find_package(SFML REQUIRED system graphics window audio)
endif ()
find_package(Lua52 REQUIRED)
find_package(Apollo REQUIRED)

set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_MULTITHREADED  ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED)

include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/include"
    ${SFML_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${APOLLO_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS})

add_library(lsfml SHARED ${LSFML_HDRS} ${LSFML_HDRS_PUBLIC} ${LSFML_SOURCES})

set_target_properties(lsfml PROPERTIES COMPILE_DEFINITIONS "${COMP_DEFS}")

target_link_libraries(lsfml
    ${SFML_LIBRARIES}
    ${LUA_LIBRARIES}
    ${APOLLO_LIBRARIES}
    ${Boost_LIBRARIES})

if (WIN32)
    target_link_libraries(lsfml "winmm.lib")
endif()

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(lsfml dl)
endif()

install(TARGETS lsfml RUNTIME DESTINATION bin)
