// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qlinuxwebviewplugin.h"
#include "qlinuxwebview_p.h"

QT_BEGIN_NAMESPACE

QAbstractWebView *QLinuxWebViewPlugin::create(const QString &key, QObject *parent) const
{
    return (key == QLatin1String("webview")) ? new QLinuxWebViewPrivate(parent) : nullptr;
}

void QLinuxWebViewPlugin::prepare() const { }

QT_END_NAMESPACE

#include "qlinuxwebviewplugin.moc"
