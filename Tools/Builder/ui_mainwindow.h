/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QPushButton *pushButtonStartCalc;
    QLabel *label;
    QPushButton *openDirButton;
    QProgressBar *progressBarTotal;
    QProgressBar *progressBarCurrent;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *labelShowCurrentDirText_2;
    QLabel *labelCurrentDir;
    QLabel *labelCurrentFile;
    QLabel *labelShowCurrentDirText;
    QLabel *lableNumberOfFiles;
    QLabel *labelShowCurrentDirText_3;
    QLineEdit *lineEditInputVersion;
    QLabel *labelShowCurrentDirText_4;
    QLabel *labelShowCurrentDirText_5;
    QPlainTextEdit *plainTextEdit_Description;
    QLabel *labelShowCurrentDirText_6;
    QLineEdit *lineEditInputExec;
    QLineEdit *lineEditInputExecArgs;
    QLabel *labelShowCurrentDirText_7;
    QLabel *labelShowCurrentDirText_8;
    QPlainTextEdit *plainTextEdit_CDN;
    QLabel *labelShowCurrentDirText_9;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(784, 721);
        pushButtonStartCalc = new QPushButton(MainWindow);
        pushButtonStartCalc->setObjectName(QString::fromUtf8("pushButtonStartCalc"));
        pushButtonStartCalc->setGeometry(QRect(440, 640, 181, 71));
        label = new QLabel(MainWindow);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(190, 10, 471, 51));
        QFont font;
        font.setPointSize(18);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8(""));
        openDirButton = new QPushButton(MainWindow);
        openDirButton->setObjectName(QString::fromUtf8("openDirButton"));
        openDirButton->setGeometry(QRect(110, 640, 171, 71));
        progressBarTotal = new QProgressBar(MainWindow);
        progressBarTotal->setObjectName(QString::fromUtf8("progressBarTotal"));
        progressBarTotal->setGeometry(QRect(170, 230, 411, 23));
        progressBarTotal->setValue(24);
        progressBarCurrent = new QProgressBar(MainWindow);
        progressBarCurrent->setObjectName(QString::fromUtf8("progressBarCurrent"));
        progressBarCurrent->setGeometry(QRect(170, 180, 411, 23));
        progressBarCurrent->setValue(24);
        label_2 = new QLabel(MainWindow);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 190, 72, 15));
        label_3 = new QLabel(MainWindow);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 230, 72, 15));
        labelShowCurrentDirText_2 = new QLabel(MainWindow);
        labelShowCurrentDirText_2->setObjectName(QString::fromUtf8("labelShowCurrentDirText_2"));
        labelShowCurrentDirText_2->setGeometry(QRect(20, 70, 101, 30));
        QFont font1;
        font1.setPointSize(12);
        labelShowCurrentDirText_2->setFont(font1);
        labelCurrentDir = new QLabel(MainWindow);
        labelCurrentDir->setObjectName(QString::fromUtf8("labelCurrentDir"));
        labelCurrentDir->setGeometry(QRect(120, 80, 521, 16));
        labelCurrentFile = new QLabel(MainWindow);
        labelCurrentFile->setObjectName(QString::fromUtf8("labelCurrentFile"));
        labelCurrentFile->setGeometry(QRect(120, 120, 531, 16));
        labelShowCurrentDirText = new QLabel(MainWindow);
        labelShowCurrentDirText->setObjectName(QString::fromUtf8("labelShowCurrentDirText"));
        labelShowCurrentDirText->setGeometry(QRect(20, 110, 101, 30));
        labelShowCurrentDirText->setFont(font1);
        lableNumberOfFiles = new QLabel(MainWindow);
        lableNumberOfFiles->setObjectName(QString::fromUtf8("lableNumberOfFiles"));
        lableNumberOfFiles->setGeometry(QRect(120, 160, 541, 16));
        labelShowCurrentDirText_3 = new QLabel(MainWindow);
        labelShowCurrentDirText_3->setObjectName(QString::fromUtf8("labelShowCurrentDirText_3"));
        labelShowCurrentDirText_3->setGeometry(QRect(20, 150, 101, 30));
        labelShowCurrentDirText_3->setFont(font1);
        lineEditInputVersion = new QLineEdit(MainWindow);
        lineEditInputVersion->setObjectName(QString::fromUtf8("lineEditInputVersion"));
        lineEditInputVersion->setGeometry(QRect(160, 350, 161, 31));
        QFont font2;
        font2.setPointSize(15);
        lineEditInputVersion->setFont(font2);
        labelShowCurrentDirText_4 = new QLabel(MainWindow);
        labelShowCurrentDirText_4->setObjectName(QString::fromUtf8("labelShowCurrentDirText_4"));
        labelShowCurrentDirText_4->setGeometry(QRect(10, 350, 121, 30));
        labelShowCurrentDirText_4->setFont(font1);
        labelShowCurrentDirText_5 = new QLabel(MainWindow);
        labelShowCurrentDirText_5->setObjectName(QString::fromUtf8("labelShowCurrentDirText_5"));
        labelShowCurrentDirText_5->setGeometry(QRect(40, 410, 161, 30));
        labelShowCurrentDirText_5->setFont(font1);
        plainTextEdit_Description = new QPlainTextEdit(MainWindow);
        plainTextEdit_Description->setObjectName(QString::fromUtf8("plainTextEdit_Description"));
        plainTextEdit_Description->setGeometry(QRect(200, 410, 511, 87));
        labelShowCurrentDirText_6 = new QLabel(MainWindow);
        labelShowCurrentDirText_6->setObjectName(QString::fromUtf8("labelShowCurrentDirText_6"));
        labelShowCurrentDirText_6->setGeometry(QRect(10, 510, 121, 30));
        labelShowCurrentDirText_6->setFont(font1);
        lineEditInputExec = new QLineEdit(MainWindow);
        lineEditInputExec->setObjectName(QString::fromUtf8("lineEditInputExec"));
        lineEditInputExec->setGeometry(QRect(160, 510, 521, 31));
        lineEditInputExec->setFont(font2);
        lineEditInputExecArgs = new QLineEdit(MainWindow);
        lineEditInputExecArgs->setObjectName(QString::fromUtf8("lineEditInputExecArgs"));
        lineEditInputExecArgs->setGeometry(QRect(160, 550, 521, 31));
        lineEditInputExecArgs->setFont(font2);
        labelShowCurrentDirText_7 = new QLabel(MainWindow);
        labelShowCurrentDirText_7->setObjectName(QString::fromUtf8("labelShowCurrentDirText_7"));
        labelShowCurrentDirText_7->setGeometry(QRect(10, 550, 121, 30));
        labelShowCurrentDirText_7->setFont(font1);
        labelShowCurrentDirText_8 = new QLabel(MainWindow);
        labelShowCurrentDirText_8->setObjectName(QString::fromUtf8("labelShowCurrentDirText_8"));
        labelShowCurrentDirText_8->setGeometry(QRect(20, 260, 101, 30));
        labelShowCurrentDirText_8->setFont(font1);
        plainTextEdit_CDN = new QPlainTextEdit(MainWindow);
        plainTextEdit_CDN->setObjectName(QString::fromUtf8("plainTextEdit_CDN"));
        plainTextEdit_CDN->setGeometry(QRect(160, 260, 411, 81));
        labelShowCurrentDirText_9 = new QLabel(MainWindow);
        labelShowCurrentDirText_9->setObjectName(QString::fromUtf8("labelShowCurrentDirText_9"));
        labelShowCurrentDirText_9->setGeometry(QRect(10, 590, 331, 30));
        labelShowCurrentDirText_9->setFont(font1);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButtonStartCalc->setText(QCoreApplication::translate("MainWindow", "\347\224\237\346\210\220\346\226\207\344\273\266md5\350\241\250", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\346\200\246\345\244\251\344\271\220\345\233\255\350\265\204\346\272\220\346\240\241\351\252\214\346\226\207\344\273\266\347\224\237\346\210\220\345\231\250", nullptr));
        openDirButton->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\347\233\256\345\275\225", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266\350\277\233\345\272\246", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\346\200\273\350\277\233\345\272\246", nullptr));
        labelShowCurrentDirText_2->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\347\233\256\345\275\225\357\274\232", nullptr));
        labelCurrentDir->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\347\233\256\345\275\225", nullptr));
        labelCurrentFile->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\346\226\207\344\273\266", nullptr));
        labelShowCurrentDirText->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\346\226\207\344\273\266\357\274\232", nullptr));
        lableNumberOfFiles->setText(QCoreApplication::translate("MainWindow", "1 / 10", nullptr));
        labelShowCurrentDirText_3->setText(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266\346\225\260\351\207\217\357\274\232", nullptr));
        lineEditInputVersion->setText(QCoreApplication::translate("MainWindow", "0.0.1", nullptr));
        labelShowCurrentDirText_4->setText(QCoreApplication::translate("MainWindow", "\345\256\242\346\210\267\347\253\257\347\211\210\346\234\254\357\274\232", nullptr));
        labelShowCurrentDirText_5->setText(QCoreApplication::translate("MainWindow", "\345\256\242\346\210\267\347\253\257\346\233\264\346\226\260\346\217\217\350\277\260\357\274\232", nullptr));
        plainTextEdit_Description->setPlainText(QCoreApplication::translate("MainWindow", "\346\226\260\345\242\236\345\206\205\345\256\271", nullptr));
        labelShowCurrentDirText_6->setText(QCoreApplication::translate("MainWindow", "\350\277\220\350\241\214\346\226\207\344\273\266", nullptr));
        lineEditInputExec->setText(QCoreApplication::translate("MainWindow", "Paradise.exe", nullptr));
        lineEditInputExecArgs->setText(QCoreApplication::translate("MainWindow", "pwnsky.com", nullptr));
        labelShowCurrentDirText_7->setText(QCoreApplication::translate("MainWindow", "\350\277\220\350\241\214\345\217\202\346\225\260", nullptr));
        labelShowCurrentDirText_8->setText(QCoreApplication::translate("MainWindow", "\350\265\204\346\272\220CDN", nullptr));
        plainTextEdit_CDN->setPlainText(QCoreApplication::translate("MainWindow", "http://pd.pwnsky.com:4000/", nullptr));
        labelShowCurrentDirText_9->setText(QCoreApplication::translate("MainWindow", "arg[1]\344\270\272 \346\233\264\346\226\260\345\231\250\347\211\210\346\234\254", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
