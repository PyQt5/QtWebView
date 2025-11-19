# Copyright (C) 2022 The Qt Company Ltd. SPDX-License-Identifier: BSD-3-Clause

# ##############################################################################
# QLinuxWebView Plugin:
# ##############################################################################

cmake_minimum_required(VERSION 3.16)

project(QLinuxWebViewPlugin LANGUAGES CXX C)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED IMPORTED_TARGET gtk+-3.0)
pkg_search_module(WEBKIT2GTK REQUIRED webkit2gtk-4.1 webkit2gtk-4.0)

include_directories("${PROJECT_SOURCE_DIR}/../../webview" ${GTK3_INCLUDE_DIRS}
                    ${WEBKIT2GTK_INCLUDE_DIRS})

set(PROJECT_SOURCES qlinuxwebview.cpp qlinuxwebview_p.h qlinuxwebviewplugin.h
                    qlinuxwebviewplugin.cpp)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::CorePrivate
          ${WEBKIT2GTK_LIBRARIES})
