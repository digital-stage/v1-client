#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "login.h"
#include <QSettings>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mySettingsFile="my.ini";
    loadSettings();
    mylogin = new Login;
    connect(ui->actionIn, &QAction::triggered, this, &MainWindow::showLogin);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::showLogin()
{
    mylogin->setUser(myUsername);
    int ret = mylogin->exec();
    if (ret == QDialog::Accepted){
        Login::UserId aUserId = mylogin->getSettings();
        this->saveSettings(aUserId.User, aUserId.ID);
    }
    return 0;
}

int MainWindow::loadSettings()
{
    // I am very very sorry for this temp hack. We will use proper keystore ...soon.
    QSettings settings(mySettingsFile, QSettings::IniFormat);
    myUsername = settings.value("user", "").toString();
    myId = settings.value("id", "").toString();
    qDebug() << "loaded Settings" << myUsername << myId;
    return 0;
}

int MainWindow::saveSettings(QString user, QString id)
{
    // I am very very sorry for this temp hack. We will use proper keystore ...soon.
    QSettings settings(mySettingsFile, QSettings::IniFormat);
    settings.setValue("user", user);
    settings.setValue("id", id);
    qDebug() << "settings saved: " << user << id;
    return 0;
}
