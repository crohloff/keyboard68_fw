# SPDX-License-Identifier: Apache-2.0

set(BOARD_ROOT ${CMAKE_CURRENT_LIST_DIR})

cmake_minimum_required(VERSION 3.13.1)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(keyboard_fw)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})
