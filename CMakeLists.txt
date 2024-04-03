##########################################################
# CMake configuration for Hipo                           #
#                                                        #
# @author Maurik Holtrop, University of New Hampshire    #
#                                                        #
##########################################################

# minimum version of CMake, which is determined by Geant4's requirements
cmake_minimum_required(VERSION 3.20)

project(HIPO VERSION 4.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

#if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
#    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib )
#endif()


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
    if("${isSystemDir}" STREQUAL "-1")
        set(CMAKE_INSTALL_RPATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    endif("${isSystemDir}" STREQUAL "-1")

endif(APPLE)

#
# Dependencies
#
# add path containing CMake macros for this project
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(LZ4 QUIET CONFIG PATHS ${PROJECT_SOURCE_DIR}/cmake )

if(NOT LZ4_FOUND)
    message(STATUS "**LZ4********************************************************************")
    message(STATUS "* We did not find a system (or local build) LZ4 package.                *")
    message(STATUS "* Please install lz4 (Centos: yum install lz4  MacOS: brew install lz4) *")
    message(STATUS "* Or build lz4 from the included sources.                               *")
    message(STATUS "*                                                                       *")
    message(STATUS "* To make this possible, please make sure you initialized the submodule.*")
    message(STATUS "* If you did not include --recurse-submodules at initial clone do:      *")
    message(STATUS "* git submodule init                                                    *")
    message(STATUS "* git submodule update                                                  *")
    message(STATUS "* Then: 'cd lz4' followed by:                                           *")
    message(STATUS "* make PREFIX=${CMAKE_INSTALL_PREFIX} CFLAGS='-fPIC' install ")
    message(STATUS "*************************************************************************")
    message(FATAL_ERROR "stop processing until lz4 is available")
endif()

add_compile_definitions(__LZ4__)

set(DATAFRAME_IN_MAIN TRUE)
add_subdirectory(extensions/dataframes)

set(HIPO_SOURCE_FILES
        hipo4/bank.cpp
        hipo4/datastream.cpp
        hipo4/dictionary.cpp
        hipo4/event.cpp
        hipo4/fusion.cpp
        hipo4/node.cpp
        hipo4/parser.cpp
        hipo4/reader.cpp
        hipo4/recordbuilder.cpp
        hipo4/record.cpp
        hipo4/tuple.cpp
        hipo4/utils.cpp
        hipo4/wrapper.cpp
        hipo4/writer.cpp
        )

set(HIPO_HEADERS
        hipo4/bank.h
        hipo4/datastream.h
        hipo4/dictionary.h
        hipo4/event.h
        hipo4/fizika.h
        hipo4/fusion.h
        hipo4/hipoexceptions.h
        hipo4/json.h
        hipo4/jsonutil.h
        hipo4/node.h
        hipo4/parser.h
        hipo4/reaction.h
        hipo4/reader.h
        hipo4/recordbuilder.h
        hipo4/record.h
        hipo4/tuple.h
        hipo4/twig.h
        hipo4/utils.h
        hipo4/writer.h
        )

add_library(hipo4_objs   OBJECT ${HIPO_SOURCE_FILES})
add_library(hipo4        SHARED $<TARGET_OBJECTS:hipo4_objs>)
add_library(hipo4_static STATIC $<TARGET_OBJECTS:hipo4_objs>)
set_target_properties(hipo4_static PROPERTIES OUTPUT_NAME hipo4)   # So that the lib is not called libhipo4_static.a

# Required on Unix OS family to be able to be linked into shared libraries.
set_target_properties(hipo4_objs PROPERTIES POSITION_INDEPENDENT_CODE ON)
# Note: Neither add_dependencies(hipo4_objs PkgConfig::LZ4) nor target_include_directories(hipo4_objs PUBLIC PkgConfig::LZ4)
# work in an Intel Mac.
target_include_directories(hipo4_objs PUBLIC ${LZ4_INCLUDE_DIRS})

# Include the macros for creating export package.
include(CMakePackageConfigHelpers)
# Write a package versioning file based on the project version (see top)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/hipo4ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION} COMPATIBILITY AnyNewerVersion)

target_include_directories(hipo4 PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/hipo4>
                           $<INSTALL_INTERFACE:include>
                           $<INSTALL_INTERFACE:include/hipo4>
                           )

target_link_libraries(hipo4 PUBLIC PkgConfig::LZ4 )
target_link_libraries(hipo4_static PUBLIC PkgConfig::LZ4)

install(TARGETS hipo4
        EXPORT hipo4-export
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(TARGETS hipo4_static
        EXPORT hipo4-export
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(FILES ${HIPO_HEADERS}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/hipo4)

install(EXPORT hipo4-export
        FILE hipo4Targets.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/hipo4)

install(FILES cmake/LZ4Config.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/hipo4)

export(TARGETS hipo4
       FILE "${PROJECT_BINARY_DIR}/hipo4Targets.cmake")

configure_package_config_file( cmake/hipo4Config.cmake.in
                               ${CMAKE_CURRENT_BINARY_DIR}/hipo4Config.cmake
                               INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/hipo4
                               )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/hipo4Config.cmake ${CMAKE_CURRENT_BINARY_DIR}/hipo4ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/hipo4)

configure_file(${PROJECT_SOURCE_DIR}/cmake/hipo4.pc.in
               ${PROJECT_BINARY_DIR}/hipo4.pc
               @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/hipo4.pc
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
