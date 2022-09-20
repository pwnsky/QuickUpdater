#ifndef MD5THREAD_H
#define MD5THREAD_H

#include <QThread>
#include <QFile>
#include <QFileInfo>

class MD5Thread : public QThread
{

    Q_OBJECT
signals:
    void dataAdded(QString filename, uint hashProgress);
    void md5Hashed(QString file_path, QString hash, qint64 size);
    void status(QString status);
    void currentProgress(int current, int NumberOfFiles);
    void finishedCalc();

public:
    MD5Thread();
    ~MD5Thread();
    void setDir(const QString & dirPath);
    void FileMD5(QString name);
    //void quit();


    void StartCalc(QString dirname);
    void StopCalc();
    void QuitLoop();

protected:
    void run();
private:
    bool isCalc = false;
    bool isLoop = true;
    QString dirPath;
    QList<QFileInfo> files;
    bool FindFile(const QString & dirPath);
    void StartCalcFileMD5();


};

#endif // MD5THREAD_H
