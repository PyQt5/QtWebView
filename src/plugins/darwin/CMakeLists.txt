# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# ##############################################################################
# QDarwinWebViewPlugin Plugin:
# ##############################################################################

cmake_minimum_required(VERSION 3.16)

project(QDarwinWebViewPlugin LANGUAGES CXX C)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui)

include_directories("${PROJECT_SOURCE_DIR}/../../webview")

set(PROJECT_SOURCES qdarwinwebview.mm qdarwinwebview_p.h qdarwinwebviewplugin.h
                    qdarwinwebviewplugin.cpp)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::CorePrivate
          "-framework WebKit" "-framework AppKit" "-framework Foundation")
