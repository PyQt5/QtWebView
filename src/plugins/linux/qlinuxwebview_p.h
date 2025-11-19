// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QLINUXWEBVIEW_P_H
#define QLINUXWEBVIEW_P_H

#include <qabstractwebview_p.h>

#include <QMap>
#include <QPointer>

QT_BEGIN_NAMESPACE

class QLinuxWebViewSettingsPrivate final : public QAbstractWebViewSettings
{
    Q_OBJECT
public:
    explicit QLinuxWebViewSettingsPrivate(QObject *p = nullptr);

    bool localStorageEnabled() const final;
    bool javaScriptEnabled() const final;
    bool localContentCanAccessFileUrls() const final;
    bool allowFileAccess() const final;

public Q_SLOTS:
    void setLocalContentCanAccessFileUrls(bool enabled) final;
    void setJavaScriptEnabled(bool enabled) final;
    void setLocalStorageEnabled(bool enabled) final;
    void setAllowFileAccess(bool enabled) final;

private:
    bool m_allowFileAccess = false;
    bool m_localContentCanAccessFileUrls = false;
    bool m_javaScriptEnabled = true;
};

class QLinuxWebViewPrivate : public QAbstractWebView
{
    Q_OBJECT
public:
    explicit QLinuxWebViewPrivate(QObject *parent = nullptr);
    ~QLinuxWebViewPrivate() override;

    QString httpUserAgent() const override;
    void setHttpUserAgent(const QString &userAgent) override;
    void setUrl(const QUrl &url) override;
    bool canGoBack() const override;
    bool canGoForward() const override;
    QString title() const override;
    int loadProgress() const override;
    bool isLoading() const override;

    QWindow *nativeWindow() const override;

public Q_SLOTS:
    void goBack() override;
    void goForward() override;
    void reload() override;
    void stop() override;
    void loadHtml(const QString &html, const QUrl &baseUrl = QUrl()) override;
    void setCookie(const QString &domain, const QString &name, const QString &value) override;
    void deleteCookie(const QString &domain, const QString &name) override;
    void deleteAllCookies() override;

private Q_SLOTS:
    void updateWindowGeometry();
    void initialize(void *hWnd);

protected:
    void runJavaScriptPrivate(const QString &script, int callbackId) override;
    QAbstractWebViewSettings *getSettings() const override;

private:
    void urlChangedCallback();
    void titleChangedCallback();
    void loadProgressCallback();
    void loadChangedCallback(uint32_t ev);
    void loadFailedCallback(uint32_t ev, const char *url, const char *message);

private:
    void *m_webview; // WebKitWebView
    void *m_widget; // GtkWidget
    QLinuxWebViewSettingsPrivate *m_settings;
    QWindow *m_window;
    QUrl m_url;
};

QT_END_NAMESPACE

#endif // QLINUXWEBVIEW_P_H
