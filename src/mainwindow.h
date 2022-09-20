#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QSystemTrayIcon>

#include "floating_settings_window.h"
#include "tray_icon.h"
#include "base.h"
#include "updaterthread.h"

#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDir>

#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSslSocket>
#include <QDesktopServices>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
    enum Step {
        wait,
        init,
        check_update,
        update,
        exec,
    };

    enum Status {
        start,
        update_success,
        update_fail,
        exec_success,
        exec_fail,
        exit,
    };

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void startTflash();

private:
    Ui::MainWindow *ui;
    QPoint mousePosition;
    bool isMousePressed = false;

    FloatingSettingsWindow *floating_settings_window_;
    TrayIcon *tray_icon_;
    //TrayIcon *tray_icon_;
    QTimer *frameTimer;
    QTimer *waitForExecTimer;
    QTimer *waitForUpdateAgainTimer;

    QNetworkAccessManager *downloaderNetworkAccessManager;
    QNetworkReply *dwonloaderNetworkReply;

    QNetworkAccessManager *mainServerNetworkAccessManager;
    QNetworkReply *mainServerNetworkReply;

    QNetworkAccessManager *mainServerLogNetworkAccessManager;
    QNetworkReply *mainServerLogNetworkReply;
    Step step = Step::init;
    bool isProcessing = false;
    UpdaterThread *updaterThread;

    qint64 numberOfFiles;
    qint64 updateSize;
    qint64 downloadedSize = 0;
    int updatingFileIndex = 0;
    QFile downloadingFile;
    int downloadRueqestCount = 4;
    bool isTryAgainDownload = false;
    QString execFileName;
    QString arguments;
    QString LoadSerialKey();
    QString loadUUID();
    QString updaterVersion = "0.5.5";
    QString uuid;

    bool startCheckUpdate();
    bool startUpdate();
    bool startExec();
    void checkUpdate(const QByteArray &data);
    void saveFile(const QString &path, const QByteArray &data);
    QString mkMutiDir(const QString path);
    void execClient();
    bool startCheckActive();
    bool SaveSerialKey(QString key);
    bool startCheckAcceptLicense();
    void initUI();
    void writeRemoteLog(QString Message, Status status = Status::start);
    void exitApp();
    QString GetSysInfo();
    void OpenWebDownloadURL();

    void installApplication();
    void createDesktopIcon();
    int openURLTimes = 0;
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;

private slots:
    void dealWithLocalCmd(LocalCmd cmd);
    void networkRequestReply(QNetworkReply *reply);
    void mainServerNetworkRequestReply(QNetworkReply *reply);
    void mainServerLogNetworkRequestReply(QNetworkReply *reply);
    void startMain();
    void showProcessText(QString showText);
    void dealWithFinishedCheckFile(qint64 numberOfFiles, qint64 updateSize);
    void downloadProgress(qint64 bytesDownload, qint64 bytesTotal);
    void updateFile();

    void on_pushButtonClose_clicked();
};

#endif // MAINWINDOW_H
