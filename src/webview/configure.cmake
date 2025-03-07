# Copyright (C) 2025 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#### Inputs

#### Libraries
if(WIN32)
    qt_find_package(WebView2 PROVIDED_TARGETS WebView2::WebView2 MODULE_NAME core QMAKE_LIB webview2)
endif()
#### Tests

#### Features

qt_feature("webview-webview2" PUBLIC
    LABEL "WebView - Microsoft Edge WebView2 plugin"
    PURPOSE "Provides native Microsoft Edge WebView2 control as a plugin for Qt WebView."
    CONDITION WIN32 AND MSVC AND WebView2_FOUND
)

qt_configure_add_summary_section(NAME "Qt WebView Plugin")
qt_configure_add_summary_entry(ARGS "webview-webview2"
                               CONDITION WIN32)
qt_configure_end_summary_section()

qt_configure_add_report_entry(
    TYPE WARNING
    MESSAGE "No WebView2 SDK found, compiling QtWebView without WebView2 plugin."
    CONDITION WIN32 AND MSVC AND NOT QT_FEATURE_webview_webview2
)
