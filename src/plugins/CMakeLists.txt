# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(ANDROID)
    add_subdirectory(android)
elseif(IOS OR MACOS)
    add_subdirectory(darwin)
elseif(WASM)
    add_subdirectory(wasm)
endif()

if(QT_FEATURE_webview_webview2)
    add_subdirectory(windows)
elseif(WINRT)
    add_subdirectory(winrt)
endif()
if(TARGET Qt::WebEngineCore)
    add_subdirectory(webengine)
endif()
