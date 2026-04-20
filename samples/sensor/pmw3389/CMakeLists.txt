# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(BOARD teensy41)

get_filename_component(PMW3389_MODULE
    "${CMAKE_CURRENT_LIST_DIR}/../../../../../modules/pmw3389"
    ABSOLUTE
)
get_filename_component(LPSPI_MODULE
    "${CMAKE_CURRENT_LIST_DIR}/../../../../../modules/lpspi"
    ABSOLUTE
)

list(APPEND EXTRA_ZEPHYR_MODULES 
	${PMW3389_MODULE}
	${LPSPI_MODULE}
)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(pmw3389_sample)

FILE(GLOB app_sources src/*.cpp)
target_sources(app PRIVATE ${app_sources})
