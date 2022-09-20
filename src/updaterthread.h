#ifndef UPDATERTHREAD_H
#define UPDATERTHREAD_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QFileInfo>

#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


struct FileInfo {
    QString path;
    qint64 size;
};


class UpdaterThread : public QThread
{
    Q_OBJECT
signals:
    void status(QString status);
    void currentProgress(int current, int NumberOfFiles);
    void showProcessText(QString showText);
    void finishedCheckFile(qint64 numberOfFiles, qint64 updateSize);

public:
    UpdaterThread();
    ~UpdaterThread();
    void setDir(const QString & dirPath);
    QString FileMD5(QString name);
    void StartCalc(QJsonArray filesJsonArray);
    void StopCalc();
    void QuitLoop();
    void StartUpdate();
    QList<FileInfo> waitForUpdateFiles; // 等待更新的文件。

protected:
    void run();
private:
    bool isCalc = false;
    bool isUpdate = false;
    bool isLoop = true;
    QString dirPath;

    quint64 totalUpdateSize = 0;
    //bool FindFile(const QString & dirPath);
    void StartCheckFiles();
    QJsonArray filesJsonArray;

};
#endif // UPDATERTHREAD_H
