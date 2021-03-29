##########################################################
# CMake configuration for Hipo                           #
#                                                        #
# @author Maurik Holtrop, University of New Hampshire    #
#                                                        #
##########################################################

# minimum version of CMake, which is determined by Geant4's requirements
cmake_minimum_required(VERSION 3.8)

project(HIPO VERSION 1.3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    # use, i.e. don't skip the full RPATH for the build tree
    set(CMAKE_SKIP_BUILD_RPATH  FALSE)

    # when building, don't use the install RPATH already
    # (but later on when installing)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

    set(CMAKE_INSTALL_RPATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

    # add the automatically determined parts of the RPATH
    # which point to directories outside the build tree to the install RPATH
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

    # the RPATH to be used when installing, but only if it's not a system directory
    list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" isSystemDir)
    IF("${isSystemDir}" STREQUAL "-1")
        set(CMAKE_INSTALL_RPATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
        message("CMAKE_INSTALL_RPATH set to ${CMAKE_INSTALL_RPATH} ")

    ENDIF("${isSystemDir}" STREQUAL "-1")
endif(APPLE)
#
# Dependencies
#
# add path containing CMake macros for this project
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(LZ4)
#
# Set a default build type if none was specified
#
set(default_build_type "RELEASE")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
                 "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(HIPO_SOURCE_FILES
    hipo4/bank.cpp
    hipo4/datastream.cpp
    hipo4/dictionary.cpp
    hipo4/event.cpp
    hipo4/reader.cpp
    hipo4/record.cpp
    hipo4/recordbuilder.cpp
    hipo4/utils.cpp
    hipo4/wrapper.cpp
    hipo4/writer.cpp)

set(HIPO_HEADERS
    hipo4/bank.h
    hipo4/datastream.h
    hipo4/dictionary.h
    hipo4/event.h
    hipo4/hipoexceptions.h
    hipo4/reader.h
    hipo4/record.h
    hipo4/recordbuilder.h
    hipo4/utils.h
    hipo4/writer.h)

include(CMakePackageConfigHelpers)

add_library(hipo4 SHARED ${HIPO_SOURCE_FILES} )
add_library(hipo4_static STATIC ${HIPO_SOURCE_FILES} )
set_target_properties(hipo4_static PROPERTIES OUTPUT_NAME hipo4)   # So that the lib is not called libhipo4_static.a
target_include_directories(hipo4
                           PUBLIC
                           $<INSTALL_INTERFACE:include>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/hipo4>
                           )

target_link_libraries(hipo4 PUBLIC )

install(TARGETS hipo4
        EXPORT hipo4-export
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(TARGETS hipo4_static
        EXPORT hipo4-export
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)


install(EXPORT hipo4-export
        FILE hipo4Targets.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/hipo4)

install(FILES ${HIPO_HEADERS}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

export(TARGETS hipo4
       FILE "${PROJECT_BINARY_DIR}/hipo4Targets.cmake")
#
#
export(PACKAGE hipo4)

configure_package_config_file( cmake/hipo4Config.cmake.in
                               ${CMAKE_CURRENT_BINARY_DIR}/hipo4Config.cmake
                               INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/hipo4
                               )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/hipo4Config.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/hipo4)
