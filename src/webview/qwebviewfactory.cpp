// Copyright (C) 2018 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwebviewfactory_p.h"
#include "qwebviewplugin_p.h"
#include <private/qfactoryloader_p.h>
#include <QtCore/qglobal.h>

#ifdef Q_OS_WIN
#include "qwebview2webviewplugin.h"
#endif

#ifdef Q_OS_MACOS
#include "qdarwinwebviewplugin.h"
#endif

#ifdef Q_OS_LINUX
#include "qlinuxwebviewplugin.h"
#endif

QT_BEGIN_NAMESPACE

class QNullWebViewSettings : public QAbstractWebViewSettings
{
public:
    explicit QNullWebViewSettings(QObject *p) : QAbstractWebViewSettings(p) {}
    bool localStorageEnabled() const override { return false; }
    bool javaScriptEnabled() const override { return false; }
    bool localContentCanAccessFileUrls() const override { return false; }
    bool allowFileAccess() const override { return false; }
    void setLocalContentCanAccessFileUrls(bool) override {}
    void setJavaScriptEnabled(bool) override {}
    void setLocalStorageEnabled(bool) override {}
    void setAllowFileAccess(bool) override {}
};

class QNullWebView : public QAbstractWebView
{
public:
    explicit QNullWebView(QObject *p = nullptr)
        : QAbstractWebView(p)
        , m_settings(new QNullWebViewSettings(this))
    {}

    QString httpUserAgent() const override { return QString(); }
    void setHttpUserAgent(const QString &userAgent) override { Q_UNUSED(userAgent); }
    void setUrl(const QUrl &url) override { Q_UNUSED(url); }
    bool canGoBack() const override { return false; }
    bool canGoForward() const override { return false; }
    QString title() const override { return QString(); }
    int loadProgress() const override { return 0; }
    bool isLoading() const override { return false; }
    void goBack() override { }
    void goForward() override { }
    void stop() override { }
    void reload() override { }
    void loadHtml(const QString &html, const QUrl &baseUrl) override
    { Q_UNUSED(html); Q_UNUSED(baseUrl); }
    void runJavaScriptPrivate(const QString &script, int callbackId) override
    { Q_UNUSED(script); Q_UNUSED(callbackId); }
    void setCookie(const QString &domain, const QString &name, const QString &value) override
    { Q_UNUSED(domain); Q_UNUSED(name); Q_UNUSED(value); }
    void deleteCookie(const QString &domain, const QString &name) override
    { Q_UNUSED(domain); Q_UNUSED(name); }
    void deleteAllCookies() override {}
    QWindow *nativeWindow() const override { return nullptr; }

protected:
    QAbstractWebViewSettings *getSettings() const override
    {
        return m_settings;
    }

private:
    QNullWebViewSettings *m_settings = nullptr;
};

QAbstractWebView *QWebViewFactory::createWebView()
{
    QAbstractWebView *wv = nullptr;
    QWebViewPlugin *plugin = getPlugin();
    if (plugin)
        wv = plugin->create(QStringLiteral("webview"));

    if (!wv || !plugin) {
        qWarning("No WebView plug-in found!");
        wv = new QNullWebView;
    }

    return wv;
}

bool QWebViewFactory::requiresExtraInitializationSteps()
{
    return true;
}

QWebViewPlugin *QWebViewFactory::getPlugin()
{
#ifdef Q_OS_WIN
    return new QWebView2WebViewPlugin;
#endif
#ifdef Q_OS_MACOS
    return new QDarwinWebViewPlugin;
#endif
#ifdef Q_OS_LINUX
    return new QLinuxWebViewPlugin;
#endif
    return nullptr;
}

bool QWebViewFactory::loadedPluginHasKey(const QString key)
{
    return true;
}

QT_END_NAMESPACE
