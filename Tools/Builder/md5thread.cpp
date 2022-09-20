#include "md5thread.h"
#include <QCryptographicHash>
#include <QDir>
#include <QDebug>
#include <QFile>


MD5Thread::MD5Thread()
{
    qDebug("MD5Thread(const QString &) called");
}

MD5Thread::~MD5Thread()
{
    qDebug("~MD5Thread() called");
}

void MD5Thread::run()
{
    while (isLoop == true) {
        if(isCalc) {
            //qDebug() << "start calc";
            FindFile(dirPath); // 获取该目录下的所有文件

            // 开始计算Hash
            StartCalcFileMD5();
            isCalc = false;
            emit finishedCalc();
            //qDebug() << "finished!";
        }
        usleep(1000);
        //qDebug() << "loop";
    }
    qDebug() << "退出MD5线程";
}

//void MD5Thread::quit() {
//    isQuit = true;
//}

void MD5Thread::StartCalc(QString dir) {
    if(isCalc == false) {
        // 先设置路径，再设置true，不然会出现条件竞争。
        files.clear();
        dirPath = dir;
        isCalc = true;
    }else {
        emit status("已经在计算中...");
    }
}

void MD5Thread::StopCalc() {
    isCalc = false;
}

void MD5Thread::QuitLoop() {
    //qDebug() << "QuitLoop";
    isLoop = false;
}

void MD5Thread::FileMD5(QString name) {
    QFile file(name);
    if (file.open(QFile::ReadOnly))
    {
        QCryptographicHash md5(QCryptographicHash::Md5);
        QByteArray buffer;
        qint64 addedDataSize = 0;
        // 一次读取8M来进行计算Hash
        while (!(buffer = file.read(8 * 1024 * 1024)).isEmpty())
        {
            md5.addData(buffer);
            emit dataAdded(name, static_cast<uint>((addedDataSize += buffer.size()) * 100.0 / file.size()));
        }
        emit md5Hashed(name, md5.result().toHex(), file.size());
        file.close();
    }
}

void MD5Thread::StartCalcFileMD5() {
    int total = files.length();
    int i = 0;
    for (QFileInfo fileinfo : files) {
        i ++;
        emit currentProgress(i, total);
        FileMD5(fileinfo.filePath());
    }
}

bool MD5Thread::FindFile(const QString & dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists())
        return false;
    dir.setFilter(QDir::Dirs | QDir::Files);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    int i = 0;
    do{
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.fileName()=="."|fileInfo.fileName()=="..")
        {
            i++;
            continue;
        }
        bool bisDir=fileInfo.isDir();
        if(bisDir) // 如果是目录，则继续递归
        {
            FindFile(fileInfo.filePath());
        }
        else {
            // nFiles++;
            // qDebug() <<QString("%1 %2 %3").arg(fileInfo.size(), 10)
            //                        .arg(fileInfo.fileName(),10).arg(fileInfo.path())<<endl;
            // qDebug() << "path: " + dirPath;
            // 计算md5
            // FileMD5(dirPath + "/" + fileInfo.fileName());
            //qDebug() << "path: " <<  fileInfo.path();
            //files.push_back(dirPath + "/" + fileInfo.fileName());
            files.push_back(fileInfo);
        }
        i++;
    }while(i < list.size());
    return true;
}

