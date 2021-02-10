#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QDebug>

#ifndef AUTH_SERVER
#define AUTH_SERVER "https://auth.digital-stage.org"
#endif

#ifndef API_SERVER
#define API_SERVER "https://api.digital-stage.org"
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Digital Stage Client");

    settingsFile="config.ini";

    // Create Login
    loginPane = new LoginPane(this);
    connect(loginPane, &LoginPane::logIn, this, &MainWindow::onLogIn);

    // Show tray icon
    trayIcon = new QSystemTrayIcon(this);

    QIcon icon = QIcon(":/images/icon.png");
    icon.setIsMask(true);

    trayIcon->setIcon(icon);
    trayIcon->setToolTip("Digital Stage");

    trayIcon->show();

    QAction * viewLoginAction = new QAction("Login", this);
    QAction * quitAction = new QAction("Close", this);
    QAction * logoutAction = new QAction("Logout", this);

    connect(viewLoginAction, SIGNAL(triggered()), this, SLOT(show()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(exit()));
    connect(logoutAction, SIGNAL(triggered()), this, SLOT(onLogOut()));

    // Login menu
    loginMenu = new QMenu(this);
    loginMenu->addAction(viewLoginAction);
    loginMenu->addAction(quitAction);

    // Status menu
    statusMenu = new QMenu(this);
    statusMenu->addSection("Running");
    statusMenu->addAction(logoutAction);
    statusMenu->addSeparator();
    statusMenu->addAction(quitAction);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(exit()));

    this->auth = new Auth(AUTH_SERVER);
    this->keyStore = new KeyStore();
}

void MainWindow::init() {
    const QString email = loadEmail();
    if( !email.isEmpty() ) {
        qDebug() << "[INIT]" << "Restored email from last session: " << email;
        KeyStore::Credentials* credentials = keyStore->restore(email);
        if( credentials != NULL ) {
            qDebug() << "[INIT]" << "Restored password from keystore: " << credentials->password;
            const string& token = auth->signIn(credentials->email.toStdString(), credentials->password.toStdString());
            if( !token.empty()) {
                qDebug() << "[INIT]" << "Retrieved token: " << token.c_str();
                showStatus();
                return;
            }
            qDebug() << "[INIT]" << "Could not sign in, showing login now" << token.c_str();
            showLogin(credentials->email, credentials->password);
            return;
        }
    }
    showLogin(email, "");
}

MainWindow::~MainWindow()
{
    delete trayIcon;
    delete loginPane;
    delete loginMenu;
    delete statusMenu;
    delete auth;
    delete keyStore;
    delete ui;
}

void MainWindow::showLogin(QString initialEmail, QString initialPassword)
{
    trayIcon->setContextMenu(loginMenu);

    loginPane->setEmail(initialEmail);
    loginPane->setPassword(initialPassword);

    setCentralWidget(loginPane);
}

void MainWindow::onLogIn(const QString email, const QString password) {
    loginPane->resetError();
    qDebug() << "[onLogIn]" << "Try to login with token: " << email << password;
    const string& token = auth->signIn(email.toStdString(), password.toStdString());
    if( !token.empty() ) {
        qDebug() << "[onLogIn]" << "Retrieved token: " << token.c_str();
        showStatus();
        return;
    }
    qDebug() << "[onLogIn]" << "Could not sign in" << token.c_str();
    loginPane->setError("Unknown email or wrong password");
}

void MainWindow::onLogOut() {
    qDebug() << "[onLogOut]" << "Signing out";
    auth->signOut();
    trayIcon->setContextMenu(loginMenu);
    loginPane->resetError();
    loginPane->setPassword("");
    show();
}

void MainWindow::showStatus()
{
    hide();
    trayIcon->setContextMenu(statusMenu);
}

const QString MainWindow::loadEmail()
{
    QSettings settings(settingsFile, QSettings::IniFormat);
    const QString email = settings.value("email", "").toString();
    qDebug() << "loaded Settings" << email;
    return email;
}


void MainWindow::saveEmail(const QString email)
{
    QSettings settings(settingsFile, QSettings::IniFormat);
    settings.setValue("email", email);
    qDebug() << "settings saved: " << email;
}

void MainWindow::exit() {
    trayWarning = false;
    qApp->quit();
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    if( !qApp->closingDown()  ) {
        qDebug() << "Not closing down";
#ifdef Q_OS_MACOS
        if (!event->spontaneous() || !isVisible()) {
            qDebug() << "MAC BLA";
            return;
        }
#endif
        if (trayIcon->isVisible()) {
            qDebug() << "Tray visible";
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
            return;
        }
    }
    qDebug() << "Closing down";
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
