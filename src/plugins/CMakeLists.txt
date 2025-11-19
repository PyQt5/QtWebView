# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_subdirectory(darwin)
endif()

if(WIN32)
    add_subdirectory(windows)
endif()

if(LINUX)
    add_subdirectory(linux)
endif()
