#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QWebView;
class QWebViewLoadRequestPrivate;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_urlEdit_returnPressed();
    void on_actionBack_triggered(bool checked = false);
    void on_actionForward_triggered(bool checked = false);
    void on_actionRefresh_triggered(bool checked = false);

    void onTitleChanged();
    void onUrlChanged();
    void onLoadingChanged(const QWebViewLoadRequestPrivate &loadRequest);
    void onLoadProgressChanged();

private:
    Ui::MainWindow *ui;
    QWebView *m_webview;
};

#endif // MAINWINDOW_H
