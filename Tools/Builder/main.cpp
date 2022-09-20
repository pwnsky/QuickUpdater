#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("T-Flash-Builder");
    app.setApplicationVersion("0.1");
    app.setOrganizationName("PWNSKY");
    app.setApplicationDisplayName("T-Flash-Builder");
    MainWindow w;
    w.show();
    return app.exec();
}
