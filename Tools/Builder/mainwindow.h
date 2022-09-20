#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "md5thread.h"
#include <QDateTime>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void writeLog(QString log);
private slots:
    void on_openDirButton_clicked();
    void onMD5ThreadDataAdded(QString file_path, uint hashProgress);
    void onMD5ThreadMD5Hashed(QString file_path, QString hash, qint64 size);
    void onMD5ThreadCurrentProgress(int current, int NumberOfFiles);
    void onMD5ThreadFinishedCalc();
    void on_pushButtonStartCalc_clicked();

private:
    Ui::MainWindow *ui;
    MD5Thread *md5Thread;
    QJsonObject saveFileInfoObject;
    QJsonArray  filesInfoJsonArray;
    int numberOfFiles;
    bool isStartCalc = false;
    void openExplorerFile(const QString &path);
};
#endif // MAINWINDOW_H
