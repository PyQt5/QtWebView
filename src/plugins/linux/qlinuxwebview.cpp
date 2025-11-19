// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

// clang-format off
#include <gio/gio.h>
#include <webkit2/webkit2.h>
// clang-format on

#include "qlinuxwebview_p.h"
#include <qwebviewloadrequest_p.h>
#include <QtWidgets/QtWidgets>

// clang-format off
#include <cairo/cairo.h>
#include <gdk/gdkx.h>
#include <gio/gio.h>
#include <glib.h>
#include <gtk/gtkx.h>
// clang-format on

#ifndef Q_ASSERT_SUCCEEDED
#  define Q_ASSERT_SUCCEEDED(hr) \
      Q_ASSERT_X(SUCCEEDED(hr), Q_FUNC_INFO, qPrintable(qt_error_string(hr)));
#endif

QLinuxWebViewSettingsPrivate::QLinuxWebViewSettingsPrivate(QObject *p) : QAbstractWebViewSettings(p)
{
}

bool QLinuxWebViewSettingsPrivate::localStorageEnabled() const
{
    return true;
}

bool QLinuxWebViewSettingsPrivate::javaScriptEnabled() const
{
    return m_javaScriptEnabled;
}

bool QLinuxWebViewSettingsPrivate::localContentCanAccessFileUrls() const
{
    return m_allowFileAccess;
}

bool QLinuxWebViewSettingsPrivate::allowFileAccess() const
{
    return m_allowFileAccess;
}

void QLinuxWebViewSettingsPrivate::setLocalContentCanAccessFileUrls(bool enabled)
{
    Q_UNUSED(enabled);
    qWarning("setLocalContentCanAccessFileUrls() not supported on this platform");
}

void QLinuxWebViewSettingsPrivate::setJavaScriptEnabled(bool enabled)
{
    m_javaScriptEnabled = enabled;
}

void QLinuxWebViewSettingsPrivate::setLocalStorageEnabled(bool enabled)
{
    Q_UNUSED(enabled);
    qWarning("setLocalStorageEnabled() not supported on this platform");
}

void QLinuxWebViewSettingsPrivate::setAllowFileAccess(bool enabled)
{
    m_allowFileAccess = enabled;
}

QLinuxWebViewPrivate::QLinuxWebViewPrivate(QObject *parent)
    : QAbstractWebView(parent),
      m_settings(new QLinuxWebViewSettingsPrivate(this)),
      m_webview(nullptr),
      m_widget(nullptr),
      m_window(nullptr),
      m_isLoading(false)
{
    // Initialize GTK
    gtk_init(nullptr, nullptr);

    // Create WebView
    m_webview = WEBKIT_WEB_VIEW(webkit_web_view_new());
    WebKitWebView *webview = (WebKitWebView *)m_webview;
    if (webview && WEBKIT_IS_WEB_VIEW(webview)) {
        m_widget = gtk_plug_new(0);
        GtkWidget *widget = (GtkWidget *)m_widget;
        if (widget) {
            gtk_container_add(GTK_CONTAINER(widget), GTK_WIDGET(webview));
            gtk_widget_show_all(widget);
            gtk_widget_realize(widget);
            void *hWnd = reinterpret_cast<void *>(gtk_plug_get_id(GTK_PLUG(widget)));
            if (hWnd) {
                // Create a QWindow without a parent
                // This window is used for initializing the WebView2
                m_window = QWindow::fromWinId(WId(hWnd));
                m_window->setFlag(Qt::FramelessWindowHint); // No border
                QTimer::singleShot(0, this, [this, hWnd]() { emit initialize(hWnd); });
            } else {
                qWarning() << "Can not get plug widget handle";
            }
        } else {
            qWarning() << "Failed to create plug widget";
        }
    } else {
        qWarning() << "Failed to create WebKit view";
    }
};

void QLinuxWebViewPrivate::initialize(void *hWnd)
{
    connect(m_window, &QWindow::widthChanged, this, &QLinuxWebViewPrivate::updateWindowGeometry,
            Qt::QueuedConnection);
    connect(m_window, &QWindow::heightChanged, this, &QLinuxWebViewPrivate::updateWindowGeometry,
            Qt::QueuedConnection);
    connect(m_window, &QWindow::screenChanged, this, &QLinuxWebViewPrivate::updateWindowGeometry,
            Qt::QueuedConnection);
}

QLinuxWebViewPrivate::~QLinuxWebViewPrivate()
{
    if (m_webview) {
        g_object_unref(m_webview);
        m_webview = nullptr;
    }
    if (m_widget) {
        GtkWidget *widget = (GtkWidget *)m_widget;
        gtk_widget_hide(widget);
        gtk_widget_destroy(widget);
        m_widget = nullptr;
    }

    if (m_window) {
        m_window->destroy();
    }
}

QString QLinuxWebViewPrivate::httpUserAgent() const
{
    return "";
}

void QLinuxWebViewPrivate::setHttpUserAgent(const QString &userAgent) { }

void QLinuxWebViewPrivate::setUrl(const QUrl &url)
{
    m_url = url;
    if (m_webview && url.isValid()) {
        webkit_web_view_load_uri((WebKitWebView *)m_webview, url.toString().toStdString().c_str());
    }
}

bool QLinuxWebViewPrivate::canGoBack() const
{
    return false;
}

bool QLinuxWebViewPrivate::canGoForward() const
{
    return false;
}

QString QLinuxWebViewPrivate::title() const
{
    return QString();
}

int QLinuxWebViewPrivate::loadProgress() const
{
    return m_isLoading ? 0 : 100;
}

bool QLinuxWebViewPrivate::isLoading() const
{
    return m_isLoading;
}

QWindow *QLinuxWebViewPrivate::nativeWindow() const
{
    return m_window;
}

void QLinuxWebViewPrivate::goBack() { }

void QLinuxWebViewPrivate::goForward() { }

void QLinuxWebViewPrivate::reload() { }

void QLinuxWebViewPrivate::stop() { }

void QLinuxWebViewPrivate::loadHtml(const QString &html, const QUrl &baseUrl)
{
    if (m_webview) {
        webkit_web_view_load_html((WebKitWebView *)m_webview, html.toUtf8().constData(),
                                  baseUrl.toString().toUtf8().constData());
    }
}

void QLinuxWebViewPrivate::setCookie(const QString &domain, const QString &name,
                                     const QString &value)
{
}

void QLinuxWebViewPrivate::deleteCookie(const QString &domainName, const QString &cookieName) { }

void QLinuxWebViewPrivate::deleteAllCookies() { }

void QLinuxWebViewPrivate::updateWindowGeometry()
{
    if (m_widget) {
        gtk_widget_set_size_request((GtkWidget *)m_widget,
                                    m_window->width() * m_window->devicePixelRatio(),
                                    m_window->height() * m_window->devicePixelRatio());
    }
}

void QLinuxWebViewPrivate::runJavaScriptPrivate(const QString &script, int callbackId) { }

QAbstractWebViewSettings *QLinuxWebViewPrivate::getSettings() const
{
    return m_settings;
}
