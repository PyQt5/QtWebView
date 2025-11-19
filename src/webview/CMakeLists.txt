# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# ##############################################################################
# WebView Module:
# ##############################################################################

cmake_minimum_required(VERSION 3.16)

project(QWebView LANGUAGES CXX C)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui WebChannel)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui WebChannel)

include_directories("${PROJECT_SOURCE_DIR}")

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  include_directories("${PROJECT_SOURCE_DIR}/../plugins/darwin")
  set(PLUGIN_LIBS QDarwinWebViewPlugin "-framework WebKit" "-framework AppKit"
                  "-framework Foundation")
endif()

if(WIN32)
  include_directories("${PROJECT_SOURCE_DIR}/../plugins/windows")
  set(PLUGIN_LIBS QWebView2WebViewPlugin)
endif()

if(LINUX)
  include_directories("${PROJECT_SOURCE_DIR}/../plugins/linux")
  set(PLUGIN_LIBS QLinuxWebViewPlugin)
endif()

set(LIBRARY_TYPE STATIC)
if(BUILD_SHARED_LIBS)
  set(LIBRARY_TYPE SHARED)
endif()

add_library(
  ${PROJECT_NAME}
  ${LIBRARY_TYPE}
  qabstractwebview_p.h
  qtwebviewfunctions.cpp
  qtwebviewfunctions.h
  qwebview.cpp
  qwebview_p.h
  qwebview_global.h
  qwebviewfactory.cpp
  qwebviewfactory_p.h
  qwebviewinterface_p.h
  qwebviewloadrequest.cpp
  qwebviewloadrequest_p.h
  qwebviewplugin.cpp
  qwebviewplugin_p.h)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui
          Qt${QT_VERSION_MAJOR}::WebChannel Qt${QT_VERSION_MAJOR}::CorePrivate
          Qt${QT_VERSION_MAJOR}::GuiPrivate ${PLUGIN_LIBS})

target_compile_definitions(${PROJECT_NAME} PRIVATE QT_BUILD_WEBVIEW_LIB)
