#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QIcon>
#include <QMessageBox>
#include <QDebug>
#include <QProcess>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QUuid>
#include <QSysInfo>
#include <QSettings>
#include <Windows.h>

/*
 * Qt 解决qt.network.ssl: QSslSocket::connectToHostEncrypted: TLS initialization failed问题
 * ref: https://blog.csdn.net/no_say_you_know/article/details/124045847
*/
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowTitle("主页面");
    setWindowIcon(QIcon(":/UI/logo.png"));

    //ui->pushButton_login->setShortcut(Qt::Key_Return);
    QPoint pos;
    pos.setX((QApplication::desktop()->width() - width()) / 2);
    pos.setY((QApplication::desktop()->height() - height()) / 2);
    move(pos);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    tray_icon_ = new TrayIcon();
    connect(tray_icon_, &TrayIcon::localCmd, this, &MainWindow::dealWithLocalCmd);
    QGraphicsOpacityEffect *pButtonOpacity = new QGraphicsOpacityEffect(this);
    pButtonOpacity->setOpacity(1);
    this->ui->BackgroundLabel->setGraphicsEffect(pButtonOpacity);
    QPropertyAnimation *pOpacityAnimation1 = new QPropertyAnimation(pButtonOpacity, "opacity");
    pOpacityAnimation1->setDuration(1000);
    pOpacityAnimation1->setStartValue(0);
    pOpacityAnimation1->setEndValue(1);
    //this->ui->BackgroundLabel->show();
    pOpacityAnimation1->start();

    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);

    frameTimer = new QTimer;
    frameTimer->start(2000); // 帧调用2s调用一次
    connect(frameTimer, &QTimer::timeout, this, &MainWindow::startMain);

    downloaderNetworkAccessManager = new QNetworkAccessManager();
    connect(downloaderNetworkAccessManager, &QNetworkAccessManager::finished, this, &MainWindow::networkRequestReply);

    mainServerNetworkAccessManager = new QNetworkAccessManager();
    connect(mainServerNetworkAccessManager, &QNetworkAccessManager::finished, this, &MainWindow::mainServerNetworkRequestReply);

    mainServerLogNetworkAccessManager = new QNetworkAccessManager();
    connect(mainServerLogNetworkAccessManager, &QNetworkAccessManager::finished, this, &MainWindow::mainServerLogNetworkRequestReply);


    updaterThread = new UpdaterThread();
    connect(updaterThread, &UpdaterThread::showProcessText, this, &MainWindow::showProcessText);
    connect(updaterThread, &UpdaterThread::finishedCheckFile, this, &MainWindow::dealWithFinishedCheckFile);

    updaterThread->start();

    waitForExecTimer = new QTimer;
    connect(waitForExecTimer, &QTimer::timeout, this, &MainWindow::execClient);

    waitForUpdateAgainTimer = new QTimer();
    connect(waitForUpdateAgainTimer, &QTimer::timeout, this, &MainWindow::updateFile);

    uuid = loadUUID(); //  加载uuid

    initUI();
    writeRemoteLog("启动客户端程序", Status::start);
    //qDebug() << GetSysInfo();
    qDebug()<< "SSL版本" << QSslSocket::sslLibraryBuildVersionString();

    installApplication(); // 安装程序
}

QString fileMD5(QString name) {
    QFile file(name);
    QString hash = "";
    if (file.open(QFile::ReadOnly))
    {
        QCryptographicHash md5(QCryptographicHash::Md5);
        QByteArray buffer;
        // 一次读取8M来进行计算Hash
        while (!(buffer = file.read(8 * 1024 * 1024)).isEmpty())
        {
            md5.addData(buffer);
        }
        hash = md5.result().toHex();
        file.close();
    }
    return hash;
}


MainWindow::~MainWindow()
{
    delete ui;
    delete tray_icon_;
}


void MainWindow::installApplication() {
    //copyFile()

    QString srcMd5 = fileMD5(QCoreApplication::applicationFilePath());
    QString desMd5 = fileMD5(WorkspaceDir + "/start.exe");
    if(srcMd5 == desMd5) {
        qDebug() << "已安装，版本一样。";
        return;
    }

    QFile file(QCoreApplication::applicationFilePath());
    if (file.open(QFile::ReadOnly))
    {
        qDebug() << "未安装或版本不一样，安装新版本中";
        QByteArray buffer;
        buffer = file.readAll();
        file.close();
        saveFile(WorkspaceDir + "/start.exe", buffer);
        createDesktopIcon();
    }

}

void MainWindow::createDesktopIcon() {
    //qDebug() <<"创建快捷方式中...";
    QString sName = "怦天乐园.lnk";
    QString sPath = WorkspaceDir + "/start.exe";
    QFile::link(sPath, QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).append("/").append(sName));
}

QString MainWindow::loadUUID(){
    QFile file(WorkspaceDir + "uuid");
    if(file.open(QFile::ReadOnly)) {
        QByteArray data = file.readAll();
        qDebug()<< "已经存在uuid: " << data;
        file.close();
        return data;
    }

    // uuid文件不存在，则创建uuid文件
    file.open(QFile::WriteOnly);
    QUuid id = QUuid::createUuid();
    file.write(id.toString().toUtf8());
    qDebug()<< "创建uuid: " << id.toString();
    return id.toString();
}


void MainWindow::mousePressEvent(QMouseEvent *event) {
    isMousePressed = true;
    mousePosition = event->pos();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *) {
    isMousePressed = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if(isMousePressed) {
        move(event->globalPos() - mousePosition);
    }
}

void MainWindow::dealWithLocalCmd(LocalCmd cmd) {
    switch (cmd) {
    case LocalCmd::exit: {
        exitApp();
    } break;
    case LocalCmd::show_main_window: {
        this->show();
    } break;
    default: {

    } break;
    }
}

void MainWindow::exitApp(){
    writeRemoteLog("退出程序", Status::exit);
    ui->labelShowText->setText("正在退出程序");
    QTimer *t = new QTimer();
    connect(t, &QTimer::timeout, this, []{
        QApplication* app;
        app->exit(0);
    });
    t->start(1); // 1 秒后退出
}

void MainWindow::initUI() {
    QString html;
    html = "<html><head/><body><p><a href=\"";
    html += WWW_URL;
    html += "\">";
    html += "<span style=\" text-decoration: underline; color:#ffffff;\">";
    html += "官方网站";
    html += "</span></a></p></body></html>";

    ui->labelShowText_Website->setText(html);


    html = "<html><head/><body><p><a href=\"";
    html += WWW_URL;
    html += "/feedback\">";
    html += "<span style=\" text-decoration: underline; color:#ffffff;\">";
    html += "问题反馈";
    html += "</span></a></p></body></html>";
    ui->labelShowText_Website_fallback->setText(html);




    ui->labelShowText_Website->setOpenExternalLinks(true);
    ui->labelShowText_Website_fallback->setOpenExternalLinks(true);

    ui->labelShowText->setText("正在启动中");
    ui->labelShowText_Version->clear();
    //ui->labelShowText->clear();
    ui->labelShowText_2->clear();
    ui->progressBar->hide();
    // 下载文件资源路径： ~/tflash/client
    // 密钥路径: ~/tflash/key
    ui->widget_Update->hide();

    ui->label_uuid->setText("uuid" + uuid);
    isProcessing = false;
    step = Step::init;
}
void MainWindow::startMain() {
    if(isProcessing == true) return;

    qDebug() << "帧调用";
    switch (step) {
    case Step::init: {
        isProcessing = true;


        isProcessing = false;
        step = Step::check_update;
    } break;
    case Step::check_update: {
        isProcessing = true;

        startCheckUpdate();
        qDebug() << " 检查更新";

    }break;
    case Step::update: {
        isProcessing = true;
        startUpdate();
    }break;
    case Step::exec: {
        isProcessing = true;
        startExec();
    }break;
    default:break;
    }
}

bool MainWindow::startCheckUpdate() {
    ui->textBrowser_Msg->clear();
    numberOfFiles = 0;
    updateSize = 0;
    downloadedSize = 0;

    ui->labelShowText->setText("检查更新中");
    QNetworkRequest networkRequest;
    networkRequest.setRawHeader("Origin", "http://pwnsky.com");
    networkRequest.setRawHeader("Accept", "application/json");
    networkRequest.setUrl(QUrl(RESOURCE_URL_VERSION));

    dwonloaderNetworkReply = downloaderNetworkAccessManager->get(networkRequest);
    connect(dwonloaderNetworkReply, &QNetworkReply::downloadProgress, this, &MainWindow::downloadProgress);
    return true;
}

void MainWindow::OpenWebDownloadURL() {

    openURLTimes++;
    if(openURLTimes == 3) {
        QDesktopServices::openUrl(QUrl(WWW_DOWNLOAD_URL, QUrl::TolerantMode));
        //exitApp();
    }
}

bool MainWindow::startUpdate() {
    ui->labelLogo->hide();
    ui->widget_Update->show();
    ui->progressBar->show();
    ui->progressBar->setRange(0, updateSize);
    ui->progressBar->setValue(0);
    isTryAgainDownload = false;
    updatingFileIndex = 0; // 更新的文件
    ui->labelShowText->setText("正在更新中");
    updateFile();
    return true;
}

void MainWindow::updateFile() {

    if(isTryAgainDownload == true) {
        //qDebug() << "重新尝试更新";
        waitForUpdateAgainTimer->stop(); //关闭定时器
    }
    //currentUpdateFile = updaterThread->waitForUpdateFiles[updatingFileIndex].path;
    QString url = RESOURCE_URL_BASE;
    QString fileName = updaterThread->waitForUpdateFiles[updatingFileIndex].path;
    qint64  fileSize = updaterThread->waitForUpdateFiles[updatingFileIndex].size;
    url += "/" + fileName.fromUtf8(fileName.toUtf8());

    //downloadingFile.setFileName(DATA_DIR + fileName);
    //downloadingFile.open(QIODevice::WriteOnly);
    qDebug() << "request download: " << url;
    QNetworkRequest networkRequest;
    networkRequest.setRawHeader("Origin", "http://pwnsky.com");
    networkRequest.setRawHeader("Accept", "application/json");
    networkRequest.setUrl(QUrl(url));
    dwonloaderNetworkReply = downloaderNetworkAccessManager->get(networkRequest);
    connect(dwonloaderNetworkReply, &QNetworkReply::downloadProgress, this, &MainWindow::downloadProgress);
}

bool MainWindow::startExec() {
    ui->labelShowText->setText("正在启动中...");
    ui->labelShowText_2->setText("");
    waitForExecTimer->start(2000); // 3 s后才启动

}

void MainWindow::writeRemoteLog(QString Message, Status status){
    /*
    qDebug()<<"write remote log: " << Message << "status: " << status;
    QNetworkRequest networkRequest;
    networkRequest.setRawHeader("Origin", "https://pwnsky.com");
    networkRequest.setRawHeader("Accept", "application/json");
    networkRequest.setRawHeader("Content-Type", "application/json");
    networkRequest.setUrl(QUrl(API_URL_LOG_ADD));
    QJsonDocument qjd;
    QJsonObject qjb;
    qjb.insert("message", Message);
    qjb.insert("status", int(status));
    qjb.insert("uuid", uuid);
    qjd.setObject(qjb);
    mainServerLogNetworkReply = mainServerLogNetworkAccessManager->post(networkRequest, qjd.toJson(QJsonDocument::Compact));
    */
}

// 启动客户端
void MainWindow::execClient() {
    waitForExecTimer->stop();
    QString currentdir = QDir::currentPath();
    QDir::setCurrent(WorkspaceDir + "/platform/");
    QString program = execFileName;
    arguments = updaterVersion + " " + uuid + " " + arguments; // 更新器版本 + authkey
    QStringList argumentsList = arguments.split(" ");
    QProcess *myProcess = new QProcess();
    myProcess->start(execFileName, argumentsList);
    QProcess::ProcessState status = myProcess->state();

    //NotRunning,
    //Starting,
    //Running
    if(status == QProcess::ProcessState::NotRunning) {
        writeRemoteLog("运行客户端失败", Status::exec_fail);
        ui->labelShowText->setText("启动失败，请点击官方网站下载最新版本");
        OpenWebDownloadURL();
    }else {
        writeRemoteLog("运行客户端成功", Status::exec_success);
        ui->labelShowText->setText("启动完成");
        exitApp();
    }

    QDir::setCurrent(currentdir);
    //this->hide();
    frameTimer->stop();
}


void MainWindow::mainServerNetworkRequestReply(QNetworkReply *reply) {
    qDebug() << "mainServerNetworkRequestReply";

    if(!(reply->error() == QNetworkReply::NetworkError::NoError)) {
        if(reply->error() == QNetworkReply::NetworkError::ContentNotFoundError) {
            ui->labelShowText->setText("文件未找到，请点击官方网站下载最新版本");
            OpenWebDownloadURL();
            return;
        }
        //QMessageBox::information(this, "提示", "网络错误");
        ui->labelShowText->setText("网络连接失败，请检查网络是否有错误；若网络正常，请点击官方网站下载最新版本");
        OpenWebDownloadURL();
        if(step == Step::check_update) {
            isProcessing = false; // 3秒后自动尝试连接
        }else if(step == Step::update){
            waitForUpdateAgainTimer->start(1000); // 1 秒之后重新下载
        }
        return ;
    }
}

void MainWindow::mainServerLogNetworkRequestReply(QNetworkReply *reply) {

}


void MainWindow::networkRequestReply(QNetworkReply *reply) {
    qDebug() << "recv: ";
    if(!(reply->error() == QNetworkReply::NetworkError::NoError)) {
        if(reply->error() == QNetworkReply::NetworkError::ContentNotFoundError) {
            ui->labelShowText->setText("文件未找到，请点击官方网站下载最新版本");
            OpenWebDownloadURL();
            return;
        }
        //QMessageBox::information(this, "提示", "网络错误");
        ui->labelShowText->setText("网络连接失败，请检查网络是否有错误；若网络正常，请点击官方网站下载最新版本");
        OpenWebDownloadURL();
        if(step == Step::check_update) {
            isProcessing = false; // 3秒后自动尝试连接
        }else if(step == Step::update){
            waitForUpdateAgainTimer->start(1000); // 1 秒之后重新下载
        }
        return ;
    }

    const QByteArray &data = reply->readAll(); // 0 拷贝

    if(step == Step::check_update) {
        checkUpdate(data);
    }else if(step == Step::update) {
        QString filePath = reply->url().path().remove(0, 1);
        qDebug() << "recved: " << filePath;
        saveFile(WorkspaceDir + "/platform/" + filePath, data);
        updatingFileIndex ++;
        downloadedSize += data.length();
        if(updatingFileIndex == numberOfFiles) {
            ui->labelShowText->setText("已更新完毕");
            //qDebug() <<
            step = Step::check_update; //验证数据完整性
            isProcessing = false;
        }else {
            updateFile(); // 没有下载完毕，继续下载。
        }
    }
}


QString MainWindow::mkMutiDir(const QString path){
    QDir dir(path);
    if ( dir.exists(path)){
        return path;
    }
    QString parentDir = mkMutiDir(path.mid(0,path.lastIndexOf('/')));
    QString dirname = path.mid(path.lastIndexOf('/') + 1);
    QDir parentPath(parentDir);
    if ( !dirname.isEmpty() )
        parentPath.mkpath(dirname);
    return parentDir + "/" + dirname;
}


void MainWindow::saveFile(const QString &path, const QByteArray &data) {
    QStringList pathSplit = path.split("/");
    QString fileName = pathSplit.last();
    QString filePath = path;
    filePath.remove(filePath.length() - fileName.length(), fileName.length());

    //QDir dir("./");
    mkMutiDir(filePath); // 递归创建文件夹
    QFile file(path);
    file.open(QFile::WriteOnly);
    file.write(data);
    file.close();
}

void MainWindow::checkUpdate(const QByteArray &data) {
    ui->labelShowText->setText("资源获取成功，正在计算更新大小");
    QJsonDocument jsonDocument;
    QJsonObject jsonObject;
    QJsonParseError jsonError;
    jsonDocument = QJsonDocument::fromJson(data, &jsonError);
    if(jsonError.error != QJsonParseError::NoError) {
        qDebug() << "json 文件解析失败";
        return;
    }
    jsonObject = jsonDocument.object();

    //    if(jsonObject["version"] != "0.02") {
    //    }

    QString msg = "更新描述：" + jsonObject["description"].toString();
    msg += "\n更新日期：" + jsonObject["update_time"].toString();

    ui->labelShowText_Version->setText("最新版本：" + jsonObject["version"].toString());
    ui->textBrowser_Msg->append(msg);
    execFileName = jsonObject["exec"].toString();
    arguments = jsonObject["exec_arguments"].toString();

    // 采用线程来计算md5以及更新
    updaterThread->StartCalc(jsonObject["data"].toArray());
}

void MainWindow::showProcessText(QString showText) {
    ui->labelShowText->setText(showText);
}

void MainWindow::dealWithFinishedCheckFile(qint64 numberOfFiles, qint64 updateSize) {
    QString showText = "需要更新" + QString::number(numberOfFiles, 'f', 2) + "个文件，总更新大小为: " + QString::number((double)updateSize / (1024.0f * 1024.0f), 'f', 2) + " MB";
    qDebug() << showText;
    this->numberOfFiles = numberOfFiles;
    this->updateSize = updateSize;
    ui->labelShowText->setText(showText);
    if(updateSize == 0) {
        ui->progressBar->setValue(100);
        ui->labelShowText->setText("已达到最新版本");
        step = Step::exec;
        isProcessing = false;
        // 不需要更新
        return;
    }
    // 正在更新当中
    step = Step::update;
    isProcessing = false;
    //QMessageBox::information(NULL, "提示", "是否需要更新", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

void MainWindow::downloadProgress(qint64 bytesDownload, qint64 bytesTotal)    //更新进度条
{
    if(step == Step::update) {
        double totalDownloadSize = (double)(downloadedSize + bytesDownload) / (1024.0f * 1024.0f);
        double totalLeftSize = (double)(updateSize - downloadedSize - bytesDownload) / (1024.0f * 1024.0f);
        double fileDownloadedSize = (double)(bytesDownload) / (1024.0f * 1024.0f);

        QString showText;
        showText += "当前文件：" + QString::number(fileDownloadedSize, 'f', 2);
        showText += "MB / " + QString::number((double)(bytesTotal) / (1024.0f * 1024.0f), 'f', 2) + " MB";

        QString showText2  = "正在更新 " + QString::number(updatingFileIndex + 1);
        showText2 += " / " + QString::number(numberOfFiles, 'f', 2);
        showText2 += " 已下载: " + QString::number(totalDownloadSize, 'f', 2);
        showText2 += "MB 剩余： " + QString::number(totalLeftSize, 'f', 2) + " MB";
        ui->labelShowText->setText(showText);
        ui->labelShowText_2->setText(showText2);
        ui->progressBar->setValue(downloadedSize + bytesDownload);
    }
    //qDebug() << "downloadProgress: " << bytesSent << "of" << bytesTotal;


}

QString MainWindow::LoadSerialKey() {
    QFile file(WorkspaceDir + "key");
    if(!file.open(QFile::ReadOnly)) {
        return "none";
    }
    QByteArray data = file.readAll();
    file.close();
    return data;
}

bool MainWindow::SaveSerialKey(QString key) {
    saveFile(WorkspaceDir + "key", key.toUtf8());
    return true;
}

QString MainWindow::GetSysInfo()
{
    QString result;
    // 获取操作系统信息
    QString m_osDescirbe = QSysInfo::prettyProductName();

    result += " [操作系统]: " + m_osDescirbe;
    // 获取CPU信息
    QSettings *CPU = new QSettings("HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",QSettings::NativeFormat);
    result += " [CPU]: " + CPU->value("ProcessorNameString").toString();
    delete CPU;

    // 获取内存信息
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx(&statex);
    float total = statex.ullTotalPhys  * 1.0/ (1024 * 1024 * 1024);
    float avail = statex.ullAvailPhys * 1.0 / (1024 * 1024 * 1024);

    result += " [内存]: " + QString("%1 GB / %2 GB" ).
            arg(QString::asprintf("%.3f", avail)).arg(QString::asprintf("%.3f", total));
    // 获取显卡信息
    QString dcard;
    QSettings *DCard = new QSettings("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\nvlddmkm\\Device0",QSettings::NativeFormat);
    QString type = DCard->value("Device Description").toString();
    delete DCard;

    QString dType = type;
    dType.trimmed();
    if(!dType.isEmpty())
        dcard = dType;

    DCard = new QSettings("HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Services\\igfx\\Device0",QSettings::NativeFormat);
    type = DCard->value("Device Description").toString();
    delete DCard;

    dType = type;
    dType.trimmed();
    if(!dType.isEmpty())
        dcard = dcard + "\n" +dType;

    DCard = new QSettings("HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Services\\amdkmdap\\Device0",QSettings::NativeFormat);
    type = DCard->value("Device Description").toString();
    delete DCard;

    dType = type;
    dType.trimmed();
    if(!dType.isEmpty())
        dcard = dcard + "\n" +dType;
    dcard.trimmed();

    result += " [显卡]: " + dcard;


    // 获取显示屏信息
    QString m_screenDescribe = "";
    QList<QSize> screenSizeList;
    QList <int> screenCountList;

    for(int i=0; i<QApplication::desktop()->screenCount(); i++)
    {
        QRect screenRect = QApplication::desktop()->screenGeometry(i);
        QSize size(screenRect.width(), screenRect.height());

        bool bExist = false;
        for(int j=0; j<screenSizeList.length(); j++)
        {
            QSize existSize = screenSizeList.at(j);
            if(size == existSize)
            {
                screenCountList[j]++;
                bExist = true;
                break;
            }
        }

        if(!bExist)
        {
            screenSizeList.append(size);
            screenCountList.append(1);
        }
    }

    QSize m_maxScreenSize = screenSizeList.at(0);
    for(int i=0; i<screenSizeList.length(); i++)
    {
        int width = screenSizeList.at(i).width();
        int height = screenSizeList.at(i).height();
        if(width > m_maxScreenSize.width() && height > m_maxScreenSize.height())
            m_maxScreenSize = screenSizeList.at(i);

        m_screenDescribe += QString ("显示器分辨率 (%1 x %2) x %3个").arg(width).arg(height).arg(screenCountList.at(i));
        if(i!= screenSizeList.length()-1)
            m_screenDescribe += "、 ";
    }

    result += " [显示器]: " + m_screenDescribe;


    return result;

}

void MainWindow::on_pushButtonClose_clicked()
{
    QMessageBox::StandardButton rb = QMessageBox::information(NULL, "提示", "是否最小化更新程序", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes) {
        qDebug() << "ok";
        this->hide();
    }else {
        exitApp();
        qDebug() << "no";
    }
}
