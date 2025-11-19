// Copyright (C) 2018 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QDARWINWEBVIEWPLUGIN_H
#define QDARWINWEBVIEWPLUGIN_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qwebviewplugin_p.h"
#include "qabstractwebview_p.h"

#include <QtCore/qobject.h>

QT_BEGIN_NAMESPACE

class QWebView2WebViewPlugin : public QWebViewPlugin
{
    Q_OBJECT

public:
    QAbstractWebView *create(const QString &key) const override;

    void prepare() const override;
};

QT_END_NAMESPACE

#endif // QDARWINWEBVIEWPLUGIN_H
