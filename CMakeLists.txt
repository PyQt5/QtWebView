# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

include(.cmake.conf)
project(QtWebView
    VERSION "${QT_REPO_MODULE_VERSION}"
    DESCRIPTION "Qt WebView Libraries"
    HOMEPAGE_URL "https://qt.io/"
    LANGUAGES CXX C
)

if(WIN32)
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindWebView2.cmake")
endif()

add_subdirectory(src)
add_subdirectory(examples)
