#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
//#include <QJsonValue>
// ref: https://www.cnblogs.com/buyishi/p/9017540.html

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    md5Thread = new MD5Thread();

    connect(md5Thread, &MD5Thread::dataAdded, this, &MainWindow::onMD5ThreadDataAdded);
    connect(md5Thread, &MD5Thread::md5Hashed, this, &MainWindow::onMD5ThreadMD5Hashed);
    connect(md5Thread, &MD5Thread::currentProgress, this, &MainWindow::onMD5ThreadCurrentProgress);
    connect(md5Thread, &MD5Thread::finishedCalc, this, &MainWindow::onMD5ThreadFinishedCalc);
    md5Thread->start(); // 启动线程
    ui->progressBarTotal->setValue(0);
    ui->progressBarCurrent->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    md5Thread->QuitLoop();
    md5Thread->quit();
    //delete md5Thread;
}

void MainWindow::on_openDirButton_clicked()
{
    QString dirname = QFileDialog::getExistingDirectory(this, "请选择T-Flash编译目录", "./");
    QDir::setCurrent(dirname);
    ui->labelCurrentDir->setText(dirname);
    // 清除。
    for(int i = 0; i < filesInfoJsonArray.size(); i++) {
        filesInfoJsonArray.pop_back();
    }
    qDebug() << dirname;
}

void MainWindow::onMD5ThreadDataAdded(QString file_path, uint hashProgress)
{
    ui->labelCurrentFile->setText(file_path);
    ui->progressBarCurrent->setRange(1, 100);
    ui->progressBarCurrent->setValue(hashProgress);
    //qDebug() << "filename: " << filename << "  progress: " << hashProgress;

}

void MainWindow::onMD5ThreadMD5Hashed(QString file_path, QString hash, qint64 size)
{
    file_path.remove(0, 2);
    qDebug() << "file_path: " << file_path << "  hash: " << hex << " size: " << size;
    QJsonObject fileInfoJson;
    fileInfoJson.insert("path", file_path);
    fileInfoJson.insert("size", QString::number(size));
    fileInfoJson.insert("hash", hash);
    filesInfoJsonArray.append(fileInfoJson);
}

void MainWindow::onMD5ThreadCurrentProgress(int current, int NumberOfFiles)
{
    numberOfFiles = NumberOfFiles;
    ui->progressBarTotal->setRange(1, NumberOfFiles);
    ui->progressBarTotal->setValue(current);
    ui->lableNumberOfFiles->setText(QString::number(current) + " / " + QString::number(NumberOfFiles));
    //qDebug() << "filename: " << current << "  / " << NumberOfFiles;
}

void MainWindow::onMD5ThreadFinishedCalc()
{
    QMessageBox::information(this, "提示", "已经完成，生成文件为 version");
    qDebug() << "finished calc";
    QJsonDocument fileHashJsonDocument;
    saveFileInfoObject.insert("version", ui->lineEditInputVersion->text());
    saveFileInfoObject.insert("exec", ui->lineEditInputExec->text());
    saveFileInfoObject.insert("exec_arguments", ui->lineEditInputExecArgs->text());
    //saveFileInfoObject.insert("save_dir", ui->lineEditInputExec->text());
    saveFileInfoObject.insert("number_of_files", QString::number(numberOfFiles));
    saveFileInfoObject.insert("data", filesInfoJsonArray);

    QJsonArray cdnJsonObject;
    QStringList cdns = ui->plainTextEdit_CDN->toPlainText().split("\n");
    for(const QString &cdn : cdns) {
        cdnJsonObject.append(cdn);
    }

    saveFileInfoObject.insert("cdn", cdnJsonObject);
    QString updateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString description = ui->plainTextEdit_Description->toPlainText();
    saveFileInfoObject.insert("description", description);
    saveFileInfoObject.insert("update_time", updateTime);
    fileHashJsonDocument.setObject(saveFileInfoObject);
    QByteArray byte_array = fileHashJsonDocument.toJson(QJsonDocument::Compact);
    qDebug() << QString(byte_array);
    isStartCalc = false;
    QFile file("./version");
    file.open(QFile::WriteOnly);
    file.write(byte_array, byte_array.length());
    file.close();

    openExplorerFile("./");

    // 写日志
    writeLog("版本: " + ui->lineEditInputVersion->text());
    writeLog("文件数量: " + QString::number(numberOfFiles));
    writeLog("更新时间: " + updateTime);
    writeLog("更新描述: " + description);
    writeLog("\n");

}
void MainWindow::writeLog(QString log) {
    QString logPath = QDir::homePath() + "/Documents/TFlashReleaseLog.txt";
    QFile file(logPath);
    file.open(QFile::Append);
    file.write(log.toUtf8() + "\n");
    file.close();
}

void MainWindow::openExplorerFile(const QString &path)
{
    //打开资源管理器并高亮文件
    const QString explorer = "explorer";
    QStringList param;
    if(!QFileInfo(path).isDir()){
        param<<QLatin1String("/select,");
    }

    param<<QDir::toNativeSeparators(path);
    QProcess::startDetached(explorer,param);
}

void MainWindow::on_pushButtonStartCalc_clicked()
{
    if(isStartCalc == false) {
        isStartCalc = true;
        md5Thread->StartCalc("./"); // 开始计算md5
    }

}
