find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_SPOOF1090 gnuradio-spoof1090)

FIND_PATH(
    GR_SPOOF1090_INCLUDE_DIRS
    NAMES gnuradio/spoof1090/api.h
    HINTS $ENV{SPOOF1090_DIR}/include
        ${PC_SPOOF1090_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_SPOOF1090_LIBRARIES
    NAMES gnuradio-spoof1090
    HINTS $ENV{SPOOF1090_DIR}/lib
        ${PC_SPOOF1090_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-spoof1090Target.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_SPOOF1090 DEFAULT_MSG GR_SPOOF1090_LIBRARIES GR_SPOOF1090_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_SPOOF1090_LIBRARIES GR_SPOOF1090_INCLUDE_DIRS)
