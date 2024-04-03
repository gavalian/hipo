##########################################################
# CMake configuration for Hipo                           #
#                                                        #
# @author Maurik Holtrop, University of New Hampshire    #
#                                                        #
##########################################################

cmake_minimum_required(VERSION 3.22)

project(HipoDataFrame VERSION 0.1)

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

if(NOT DATAFRAME_IN_MAIN)
    find_package(hipo4 REQUIRED)
    if( ${hipo4_FOUND})
        message(STATUS "Hipo4 found. ")
    endif()
endif()

find_package(ROOT COMPONENTS Core ROOTDataFrame ROOTVecOps)
if(NOT ROOT_FOUND)
    message(STATUS "**ROOT*******************************************************************")
    message(STATUS "* We did not find a compatible version of the ROOT libraries.           *")
    message(STATUS "* These libraries are needed to compile the RDataFrame funcionality of  *")
    message(STATUS "* HIPO, which allows you to do fast and efficient data analysis with    *")
    message(STATUS "* ROOT.                                                                 *")
    message(STATUS "* To enable this, please make sure you have initialized a ROOT version  *")
    message(STATUS "* that was compiled with -DCMAKE_CXX_STANDARD=17                        *")
    message(STATUS "*************************************************************************")
else()
    message(STATUS "ROOT found at: ${ROOT_DIR}")
    #---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)

    include(${ROOT_USE_FILE})

    set(DS_HEADERS
            RHipoDS.hxx)

    set(DS_SOURCE_FILES
            RHipoDS.cxx)

    set(DS_LINKDEF_FILES
            RHipoDS_LinkDef.h)

    add_library(HipoDataFrame SHARED ${DS_SOURCE_FILES} )
    target_link_libraries(HipoDataFrame PUBLIC hipo4 ROOT::ROOTDataFrame ROOT::ROOTVecOps ) #  ${ROOT_LIBRARIES}

    ROOT_GENERATE_DICTIONARY(HipoDataFrame_Dict ${DS_HEADERS}  LINKDEF ${DS_LINKDEF_FILES} MODULE HipoDataFrame)
    # See this discussion why ROOT_GENERATE_DICTIONARY come *after* add_library() -- https://github.com/root-project/root/issues/8308
    list(APPEND ADDITIONAL_CLEAN_FILES HipoDataFrame_Dict.cxx)

    add_executable(test_hipo_ds test_hipo_ds.cxx)
    target_link_libraries(test_hipo_ds HipoDataFrame ROOT::Core ROOT::Hist)

    add_executable(test_hipo_speed test_hipo_speed.cxx)
    target_link_libraries(test_hipo_speed HipoDataFrame ROOT::Core ROOT::Hist)

    add_executable(test_root_speed test_root_speed.cxx)
    target_link_libraries(test_root_speed ROOT::Core ROOT::Hist ROOT::ROOTDataFrame ROOT::ROOTVecOps)


    include(CMakePackageConfigHelpers)
    # Write a package versioning file based on the project version (see top)
    write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/HipoDataFrameConfigVersion.cmake"
            VERSION ${PROJECT_VERSION} COMPATIBILITY AnyNewerVersion)

    target_include_directories(HipoDataFrame PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/hipo4>
            $<INSTALL_INTERFACE:include>
            $<INSTALL_INTERFACE:include/hipo4>
            )

    install(TARGETS HipoDataFrame
            EXPORT HipoDataFrame-export
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)

    install(FILES ${DS_HEADERS}
            DESTINATION include/hipo4)

    install(FILES ${PROJECT_BINARY_DIR}/libHipoDataFrame_rdict.pcm DESTINATION lib)
    install(FILES ${PROJECT_BINARY_DIR}/libHipoDataFrame.rootmap DESTINATION lib)

    install(EXPORT HipoDataFrame-export
            FILE HipoDataFrameTargets.cmake
            DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/HipoDataFrame)

    export(TARGETS HipoDataFrame
            FILE "${PROJECT_BINARY_DIR}/HipoDataFrameTargets.cmake")

    configure_package_config_file( cmake/HipoDataFrameConfig.cmake.in
            ${CMAKE_CURRENT_BINARY_DIR}/HipoDataFrameConfig.cmake
            INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/HipoDataFrame
            )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/HipoDataFrameConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/HipoDataFrameConfigVersion.cmake
            DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/HipoDataFrame)

    list(APPEND PKGCONFIG_LIBS HipoDataFrame)
    set(PKGCONFIG_LIBS ${PKGCONFIG_LIBS} PARENT_SCOPE)
endif()
