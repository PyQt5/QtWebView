# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# ##############################################################################
# QWebview2Webview Plugin:
# ##############################################################################

cmake_minimum_required(VERSION 3.16)

project(QWebView2WebViewPlugin LANGUAGES CXX C)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui)

include_directories("${PROJECT_SOURCE_DIR}/../../webview"
                    ${WEBVIEW2_INCLUDE_DIR})

set(PROJECT_SOURCES qwebview2webview.cpp qwebview2webview_p.h
                    qwebview2webviewplugin.h qwebview2webviewplugin.cpp)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::CorePrivate
          ${WEBVIEW2_LIBRARY})
