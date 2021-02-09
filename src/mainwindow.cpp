#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "login.h"
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Digital Stage Client");

    mySettingsFile="my.ini";
    loadSettings();

    // Create Login
    mylogin = new Login;
    connect(ui->actionIn, &QAction::triggered, this, &MainWindow::showLogin);

    // Show tray icon
    trayIcon = new QSystemTrayIcon(this);

    QIcon icon = QIcon(":/images/icon.png");
    icon.setIsMask(true);

    trayIcon->setIcon(icon);
    trayIcon->setToolTip("Digital Stage");

    trayIcon->show();

    QAction * viewLogin = new QAction("Login", this);
    QAction * viewStatus = new QAction("Login", this);
    QAction * quitAction = new QAction("Close", this);

    connect(viewLogin, SIGNAL(triggered()), this, SLOT(show()));
    connect(viewStatus, SIGNAL(triggered()), this, SLOT(show()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(exit()));

    // Login menu
    loginMenu = new QMenu(this);
    loginMenu->addAction(viewLogin);
    loginMenu->addAction(quitAction);

    // Status menu
    statusMenu = new QMenu(this);
    statusMenu->addSection("Running");
    statusMenu->addAction(quitAction);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    trayIcon->setContextMenu(loginMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::showLogin()
{
    trayIcon->setContextMenu(loginMenu);

    mylogin->setUser(myUsername);
    int ret = mylogin->exec();
    if (ret == QDialog::Accepted){
        Login::UserId aUserId = mylogin->getSettings();
        this->saveSettings(aUserId.User, aUserId.ID);
    }

    return 0;
}

int MainWindow::showStatus()
{

    trayIcon->setContextMenu(statusMenu);
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

void MainWindow::exit() {
    trayWarning = false;
    qApp->quit();
}

void MainWindow::closeEvent(QCloseEvent * event)
{
#ifdef Q_OS_MACOS
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif
    if (trayIcon->isVisible() && !qApp->closingDown()) {
        if( trayWarning ) {
            QMessageBox::information(this, tr("Systray"),
                                     tr("The program will keep running in the "
                                        "system tray. To terminate the program, "
                                        "choose <b>Quit</b> in the context menu "
                                        "of the system tray entry."));
            trayWarning = false;
        }

        hide();
        event->ignore();
    }
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        if(!this->isVisible())
            this->show();
        break;
    default:
        ;
    }
}
