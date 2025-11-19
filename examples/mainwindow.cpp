#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qwebview_p.h"
#include "qwebviewloadrequest_p.h"

#include <QAction>
#include <QIcon>
#include <QStyle>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_webview(nullptr)
{
    ui->setupUi(this);

    // action
    ui->actionBack->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    ui->actionForward->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    ui->actionRefresh->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));

    // webview
    m_webview = new QWebView(this);
    QWindow *window = m_webview->nativeWindow();
    if (window) {
        QWidget *container = QWidget::createWindowContainer(window, this, Qt::FramelessWindowHint);
        ui->browserLayout->addWidget(container);
    } else {
    }

    // signals
    connect(m_webview, &QWebView::titleChanged, this, &MainWindow::onTitleChanged);
    connect(m_webview, &QWebView::urlChanged, this, &MainWindow::onUrlChanged);
    connect(m_webview, &QWebView::loadingChanged, this, &MainWindow::onLoadingChanged);
    connect(m_webview, &QWebView::loadProgressChanged, this, &MainWindow::onLoadProgressChanged);

    // init url
    ui->urlEdit->setText("https://pyqt.site");
    emit ui->urlEdit->returnPressed();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_urlEdit_returnPressed()
{
    QString url = ui->urlEdit->text().trimmed();
    if (url.isEmpty() || !m_webview) {
        return;
    }

    m_webview->setUrl(QUrl(url));
}

void MainWindow::on_actionBack_triggered(bool checked)
{
    if (m_webview && m_webview->canGoBack()) {
        m_webview->goBack();
    }
}

void MainWindow::on_actionForward_triggered(bool checked)
{
    if (m_webview && m_webview->canGoForward()) {
        m_webview->goForward();
    }
}

void MainWindow::on_actionRefresh_triggered(bool checked)
{
    if (m_webview) {
        if (m_webview->isLoading()) {
            m_webview->stop();
        } else {
            m_webview->reload();
        }
    }
}

void MainWindow::onTitleChanged()
{
    setWindowTitle(m_webview->title());
}

void MainWindow::onUrlChanged()
{
    ui->urlEdit->blockSignals(true);
    ui->urlEdit->setText(m_webview->url().toString());
    ui->urlEdit->blockSignals(false);
}

void MainWindow::onLoadingChanged(const QWebViewLoadRequestPrivate &loadRequest)
{
    switch (loadRequest.m_status) {
    case QWebView::LoadStartedStatus:
        ui->actionRefresh->setIcon(style()->standardIcon(QStyle::SP_BrowserStop));
        ui->logEdit->appendPlainText(QString("start, %1, %2")
                                             .arg(loadRequest.m_url.toString())
                                             .arg(loadRequest.m_errorString));
        break;
    case QWebView::LoadStoppedStatus:
        ui->actionRefresh->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
        ui->logEdit->appendPlainText(QString("stop, %1, %2")
                                             .arg(loadRequest.m_url.toString())
                                             .arg(loadRequest.m_errorString));
        break;
    case QWebView::LoadSucceededStatus:
        ui->actionRefresh->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
        ui->logEdit->appendPlainText(QString("success, %1, %2")
                                             .arg(loadRequest.m_url.toString())
                                             .arg(loadRequest.m_errorString));
        break;
    case QWebView::LoadFailedStatus:
        ui->actionRefresh->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
        ui->logEdit->appendPlainText(QString("failed, %1, %2")
                                             .arg(loadRequest.m_url.toString())
                                             .arg(loadRequest.m_errorString));
        break;
    default:
        break;
    }
}

void MainWindow::onLoadProgressChanged()
{
    int value = m_webview->loadProgress();
    ui->statusbar->showMessage(QString("loading: %1").arg(value));
    if (value == 100) {
        ui->statusbar->clearMessage();
    }
}
