# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT spoof1090_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT spoof1090_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################
list(APPEND spoof1090_python_files
    adsb_mod_async_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(spoof1090
   ../../..
   gr::spoof1090
   "${spoof1090_python_files}")

# copy in bindings .so file for use in QA test module
add_custom_target(
  copy_bindings_for_tests ALL
  COMMAND
    ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/*.so"
    ${CMAKE_BINARY_DIR}/test_modules/gnuradio/spoof1090/
  DEPENDS spoof1090_python)

install(TARGETS spoof1090_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/spoof1090 COMPONENT pythonapi)
