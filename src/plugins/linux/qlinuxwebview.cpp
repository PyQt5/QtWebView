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
      m_window(nullptr)
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

    g_signal_connect_swapped(m_widget, "destroy", G_CALLBACK(+[](QLinuxWebViewPrivate *instance) {
                                 qDebug() << "webview container destroy";
                             }),
                             this);
    g_signal_connect_swapped(m_webview, "destroy", G_CALLBACK(+[](QLinuxWebViewPrivate *instance) {
                                 qDebug() << "webview destroy";
                             }),
                             this);

    // url change
    g_signal_connect_swapped(m_webview, "notify::uri",
                             G_CALLBACK(+[](QLinuxWebViewPrivate *instance, GParamSpec *pspec) {
                                 instance->urlChangedCallback();
                             }),
                             this);

    // title change
    g_signal_connect_swapped(m_webview, "notify::title",
                             G_CALLBACK(+[](QLinuxWebViewPrivate *instance, GParamSpec *pspec) {
                                 instance->titleChangedCallback();
                             }),
                             this);

    // load progress change
    g_signal_connect_swapped(m_webview, "notify::estimated-load-progress",
                             G_CALLBACK(+[](QLinuxWebViewPrivate *instance, GParamSpec *pspec) {
                                 instance->loadProgressCallback();
                             }),
                             this);

    // load status
    g_signal_connect_swapped(m_webview, "load-changed",
                             G_CALLBACK(+[](QLinuxWebViewPrivate *instance, WebKitLoadEvent event) {
                                 instance->loadChangedCallback(event);
                             }),
                             this);

    // load failed
    g_signal_connect_swapped(m_webview, "load-failed",
                             G_CALLBACK(+[](QLinuxWebViewPrivate *instance, WebKitLoadEvent event,
                                            char *url, GError *error) -> gboolean {
                                 instance->loadFailedCallback(event, url, error->message);
                                 return false;
                             }),
                             this);
}

QLinuxWebViewPrivate::~QLinuxWebViewPrivate()
{
    stop();

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
    if (m_webview) {
        WebKitSettings *settings =
                webkit_web_view_get_settings(static_cast<WebKitWebView *>(m_webview));
        if (settings) {
            return webkit_settings_get_user_agent(settings);
        }
    }

    return "";
}

void QLinuxWebViewPrivate::setHttpUserAgent(const QString &userAgent)
{
    if (m_webview) {
        WebKitSettings *settings =
                webkit_web_view_get_settings(static_cast<WebKitWebView *>(m_webview));
        if (settings) {
            return webkit_settings_set_user_agent(settings, userAgent.toUtf8().constData());
        }
    }
}

void QLinuxWebViewPrivate::setUrl(const QUrl &url)
{
    m_url = url;
    if (m_webview && url.isValid()) {
        webkit_web_view_load_uri((WebKitWebView *)m_webview, url.toString().toUtf8().constData());
    }
}

bool QLinuxWebViewPrivate::canGoBack() const
{
    if (m_webview) {
        return webkit_web_view_can_go_back(static_cast<WebKitWebView *>(m_webview));
    }

    return false;
}

bool QLinuxWebViewPrivate::canGoForward() const
{
    if (m_webview) {
        return webkit_web_view_can_go_forward(static_cast<WebKitWebView *>(m_webview));
    }

    return false;
}

QString QLinuxWebViewPrivate::title() const
{
    if (m_webview) {
        return webkit_web_view_get_title(static_cast<WebKitWebView *>(m_webview));
    }

    return "";
}

int QLinuxWebViewPrivate::loadProgress() const
{
    if (!m_webview) {
        return 0;
    }

    return webkit_web_view_get_estimated_load_progress(static_cast<WebKitWebView *>(m_webview))
            * 100;
}

bool QLinuxWebViewPrivate::isLoading() const
{
    if (m_webview) {
        return webkit_web_view_is_loading(static_cast<WebKitWebView *>(m_webview));
    }
    return false;
}

QWindow *QLinuxWebViewPrivate::nativeWindow() const
{
    return m_window;
}

void QLinuxWebViewPrivate::goBack()
{
    if (m_webview) {
        webkit_web_view_go_back(static_cast<WebKitWebView *>(m_webview));
    }
}

void QLinuxWebViewPrivate::goForward()
{
    if (m_webview) {
        webkit_web_view_go_forward(static_cast<WebKitWebView *>(m_webview));
    }
}

void QLinuxWebViewPrivate::reload()
{
    if (m_webview) {
        webkit_web_view_reload(static_cast<WebKitWebView *>(m_webview));
    }
}

void QLinuxWebViewPrivate::stop()
{
    if (m_webview) {
        webkit_web_view_stop_loading(static_cast<WebKitWebView *>(m_webview));
    }
}

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

void QLinuxWebViewPrivate::urlChangedCallback()
{
    m_url = webkit_web_view_get_uri(static_cast<WebKitWebView *>(m_webview));
    emit urlChanged(QUrl(m_url));
}

void QLinuxWebViewPrivate::titleChangedCallback()
{
    emit titleChanged(title());
}

void QLinuxWebViewPrivate::loadProgressCallback()
{
    emit loadProgressChanged(loadProgress());
}

void QLinuxWebViewPrivate::loadChangedCallback(uint32_t ev)
{
    WebKitWebView *webview = static_cast<WebKitWebView *>(m_webview);
    WebKitLoadEvent event = static_cast<WebKitLoadEvent>(ev);

    QUrl url(webkit_web_view_get_uri(webview));

    switch (event) {
    case WEBKIT_LOAD_STARTED:
        QMetaObject::invokeMethod(
                this, "loadingChanged", Qt::QueuedConnection,
                Q_ARG(QWebViewLoadRequestPrivate,
                      QWebViewLoadRequestPrivate(url, QWebView::LoadStartedStatus, "")));
        break;
    case WEBKIT_LOAD_FINISHED:
        QMetaObject::invokeMethod(
                this, "loadingChanged", Qt::QueuedConnection,
                Q_ARG(QWebViewLoadRequestPrivate,
                      QWebViewLoadRequestPrivate(url, QWebView::LoadStoppedStatus, "")));
        break;
    default:
        break;
    }
}

void QLinuxWebViewPrivate::loadFailedCallback(uint32_t ev, const char *url, const char *message)
{
    QMetaObject::invokeMethod(
            this, "loadingChanged", Qt::QueuedConnection,
            Q_ARG(QWebViewLoadRequestPrivate,
                  QWebViewLoadRequestPrivate(QUrl(url), QWebView::LoadFailedStatus, message)));
}
