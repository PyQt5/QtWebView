// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QUrl>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QWindow>
#include <QDebug>

#include "qwebview_p.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget w;
    w.resize(800, 600);
    QVBoxLayout *layout = new QVBoxLayout(&w);

    QWebView *view = new QWebView(&w);
    QWindow *window = view->nativeWindow();
    qInfo() << view << window;
    QWidget *container = QWidget::createWindowContainer(window, &w);
    qInfo() << container;
    layout->addWidget(container);

    view->setUrl(QUrl("https://doc.qt.io/qt/reference-overview.html"));

    w.show();

    return app.exec();
}
