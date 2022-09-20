#include "mainwindow.h"

#include <QApplication>
QString WorkspaceDir;
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("怦天互娱平台");
    app.setApplicationVersion("0.1");
    app.setOrganizationName("怦天互娱团队");
    app.setApplicationDisplayName("怦天互娱启动器");
    WorkspaceDir = QDir::homePath() + "/Documents/pwnsky/paradise";
    MainWindow mw;
    mw.show();
    return app.exec();
}
