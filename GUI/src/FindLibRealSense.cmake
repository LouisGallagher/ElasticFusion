# -Find LibRealSense
# Definitions:
#  LIBREALSENSE_FOUND - System has LibRealSense
#  LIBREALSENSE_INCLUDE_DIRS - The LibRealSense include directories
#  LIBREALSENSE_LIBRARIES - The libraries needed to use LibRealSense
#  LIBREALSENSE_DEFINITIONS - Compiler switches required for using LibRealSense

find_package(PkgConfig)
pkg_check_modules(PC_LIBREALSENSE QUIET librealsense)
set(LIBREALSENSE_DEFINITIONS ${PC_LIBREALSENSE_CFLAGS_OTHER})

find_path(LIBREALSENSE_INCLUDE_DIR librealsense/
          HINTS ${PC_LIBREALSENSE_INCLUDEDIR} ${PC_LLIBREALSENSE_INCLUDE_DIRS}
          PATH_SUFFIXES librealsense )

find_library(LIBREALSENSE_LIBRARY NAMES realsense librealsense
             HINTS ${PC_LIBREALSENSE_LIBDIR} ${PC_LIBREALSENSE_LIBRARY_DIRS} )

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(LibRealSense  DEFAULT_MSG
								  LIBREALSENSE_LIBRARY  LIBREALSENSE_INCLUDE_DIR)

mark_as_advanced(LIBREALSENSE_INCLUDE_DIR LIBREALSENSE_LIBRARY)

set(LIBREALSENSE_LIBRARIES ${LIBREALSENSE_LIBRARY} )
set(LIBREALSENSE_INCLUDE_DIRS ${LIBREALSENSE_INCLUDE_DIR} )