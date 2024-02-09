#
#  Find the LZ4 library and include files and configure target for it.
#
set(ENV{PKG_CONFIG_PATH} "${PKG_CONFIG_PATH}:${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")
find_package(PkgConfig REQUIRED)
pkg_check_modules(LZ4 IMPORTED_TARGET "liblz4")

if (NOT LZ4_FOUND)
    message(STATUS "No system version of LZ4 found.")
else()
    add_library(LZ4 INTERFACE IMPORTED)
    set_target_properties(LZ4 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LZ4_INCLUDE_DIR}")
    message(STATUS "Found LZ4: ${LZ4_LIBRARIES}")
    mark_as_advanced(LZ4_INCLUDE_DIRS LZ4_INCLUDE_DIR LZ4_LIBRARIES LZ4_LIBRARY)
endif (NOT LZ4_FOUND)
