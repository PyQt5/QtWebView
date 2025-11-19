// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qdarwinwebview_p.h"
#include "qdarwinwebviewplugin.h"
#include <qwebviewplugin_p.h>
#include <QtCore/qbytearray.h>

QT_BEGIN_NAMESPACE

QAbstractWebView *QDarwinWebViewPlugin::create(const QString &key) const
{
    return (key == QLatin1String("webview")) ? new QDarwinWebViewPrivate() : nullptr;
}

void QDarwinWebViewPlugin::prepare() const { }

QT_END_NAMESPACE

#include "qdarwinwebviewplugin.moc"
