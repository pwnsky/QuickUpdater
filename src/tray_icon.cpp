#include "tray_icon.h"

TrayIcon::TrayIcon(QObject *parent) : QObject(parent) {
    system_tray_icon_.show();
    system_tray_icon_.setIcon(QIcon(":/UI/logo.png"));
    connect(&system_tray_icon_, &QSystemTrayIcon::activated, this, &TrayIcon::dealWithTrayIconActivated);
    action_1_.setIcon(QIcon(":/UI/logo.png"));
    action_1_.setText("打开");
    action_2_.setIcon(QIcon(":/UI/logo.png"));
    action_2_.setText("退出");
    connect(&action_1_, &QAction::triggered, this, &TrayIcon::clickAction_1);
    connect(&action_2_, &QAction::triggered, this, &TrayIcon::clickAction_2);

    menu_.addAction(&action_1_);
    menu_.addAction(&action_2_);
    //menu
    system_tray_icon_.setContextMenu(&menu_);
}

void TrayIcon::clickAction_1() {
    emit localCmd(LocalCmd::show_main_window);
}

void TrayIcon::clickAction_2() {
    emit localCmd(LocalCmd::exit);
}


void TrayIcon::dealWithTrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        emit localCmd(LocalCmd::show_main_window);
        break;
    case QSystemTrayIcon::Trigger: {
        emit localCmd(LocalCmd::show_main_window);
    } break;
    case QSystemTrayIcon::MiddleClick: {
        emit localCmd(LocalCmd::show_main_window);
    }break;
    default:
        break;
    }
}
