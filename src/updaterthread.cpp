#include "updaterthread.h"
#include <QCryptographicHash>
#include <QDir>
#include <QDebug>
#include <QFile>
#include "base.h"

UpdaterThread::UpdaterThread()
{
    qDebug("UpdaterThread called");
}

UpdaterThread::~UpdaterThread()
{
    qDebug("~UpdaterThread() called");
}

void UpdaterThread::run()
{
    while (isLoop == true) {
        if(isCalc) {
            // 开始计算Hash以及是否更新
            StartCheckFiles();
            qDebug() << "MD5 计算完毕";
            emit finishedCheckFile(waitForUpdateFiles.length(), totalUpdateSize);
            isCalc = false;
        }
        usleep(1000);
    }
    qDebug() << "退出线程";
}

void UpdaterThread::StartCalc(QJsonArray fja) {
    if(isCalc == false) {
        // 先设置路径，再设置true，不然会出现条件竞争。
        //dirPath = dir;
        totalUpdateSize = 0;
        waitForUpdateFiles.clear();
        filesJsonArray = fja;
        isCalc = true;
    }else {
        emit status("正在计算所需下载的资源...");
    }
}

void UpdaterThread::StopCalc() {
    isCalc = false;
}

void UpdaterThread::QuitLoop() {
    //qDebug() << "QuitLoop";
    isLoop = false;
}

QString UpdaterThread::FileMD5(QString name) {
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

void UpdaterThread::StartCheckFiles() {
    qDebug() << "开始计算hash";
    for (int i = 0; i < filesJsonArray.count(); i++) {
        QJsonObject fileinfoJson = filesJsonArray[i].toObject();
        FileInfo fileInfo;
        fileInfo.path = fileinfoJson["path"].toString();
        fileInfo.size = fileinfoJson["size"].toString().toLong();

        QString hash = FileMD5(WorkspaceDir + "/platform/" + fileinfoJson["path"].toString());
        if(hash != fileinfoJson["hash"].toString()) {
            waitForUpdateFiles.push_back(fileInfo);
            totalUpdateSize += fileInfo.size;
            //qDebug() << "未下载: " << fileInfo.path << "大小: " << fileInfo.size;
        }else if(hash == fileinfoJson["hash"].toString()) {
            // 不需要更新的文件。
            //qDebug() << "不需要下载: " << fileInfo.path << "大小: " << fileInfo.size;
        }
    }
}



