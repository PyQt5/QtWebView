// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwebview2webviewplugin.h"
#include "qwebview2webview_p.h"

QT_BEGIN_NAMESPACE

QAbstractWebView *QWebView2WebViewPlugin::create(const QString &key, QObject *parent) const
{
    return (key == QLatin1String("webview")) ? new QWebView2WebViewPrivate(parent) : nullptr;
}

void QWebView2WebViewPlugin::prepare() const { }

QT_END_NAMESPACE

#include "qwebview2webviewplugin.moc"
