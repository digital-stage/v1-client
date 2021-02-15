#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QDebug>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Digital Stage Client");

    settingsFile="config.ini";

    // Create Login
    loginPane = new LoginPane();
    connect(loginPane, &LoginPane::logIn, this, &MainWindow::onLogIn);

    // Show tray icon
    trayIcon = new QSystemTrayIcon(this);

    QIcon icon = QIcon(":/images/icon.png");
    icon.setIsMask(true);

    trayIcon->setIcon(icon);
    trayIcon->setToolTip(tr("Digital Stage"));

    trayIcon->show();

    QAction * viewLoginAction = new QAction(tr("Login"), this);
    QAction * quitAction = new QAction(tr("Close"), this);
    QAction * logoutAction = new QAction(tr("Logout"), this);
    QAction * openStageAction = new QAction(tr("Open stage"), this);

    connect(viewLoginAction, SIGNAL(triggered()), this, SLOT(show()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(exit()));
    connect(logoutAction, SIGNAL(triggered()), this, SLOT(onLogOut()));
    connect(openStageAction, SIGNAL(triggered()), this, SLOT(openStage()));

    // Login menu
    loginMenu = new QMenu(this);
    loginMenu->addAction(viewLoginAction);
    loginMenu->addAction(quitAction);

    // Status menu
    statusMenu = new QMenu(this);
    statusMenu->addAction(openStageAction);
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
            const string& retrievedToken = auth->signIn(credentials->email.toStdString(), credentials->password.toStdString());
            if( !retrievedToken.empty()) {
                this->token = QString::fromStdString(retrievedToken);
                qDebug() << "[INIT]" << "Retrieved token: " << token;
                startClient(token);
                return;
            }
            qDebug() << "[INIT]" << "Could not sign in, showing login now";
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
    qDebug() << "Show login";
    trayIcon->setContextMenu(loginMenu);

    loginPane->setEmail(initialEmail);
    loginPane->setPassword(initialPassword);

    setCentralWidget(loginPane);
    this->show();
}

void MainWindow::openStage() {
    QDesktopServices::openUrl(QUrl(STAGE_URL));
}

void MainWindow::onLogIn(const QString email, const QString password) {
    loginPane->resetError();
    qDebug() << "[onLogIn]" << "Try to login with token: " << email << password;
    const string& retrievedToken = auth->signIn(email.toStdString(), password.toStdString());
    if( !retrievedToken.empty() ) {
        qDebug() << "[onLogIn]" << "Retrieved token: " << retrievedToken.c_str();
        token = QString::fromStdString(retrievedToken);
        keyStore->store({email, password});
        saveEmail(email);
        startClient(token);
        return;
    }
    qDebug() << "[onLogIn]" << "Could not sign in";
    loginPane->setError(tr("Unknown email or wrong password"));
}

void MainWindow::onLogOut() {
    qDebug() << "[onLogOut]" << "Signing out";
    //TODO: Check if token is initialized (or is this not neccessary in c++?)
    auth->signOut(token.toStdString());
    trayIcon->setContextMenu(loginMenu);
    loginPane->resetError();
    loginPane->setPassword("");
    this->show();
}

void MainWindow::startClient(QString token)
{
    qDebug() << "Starting client using token" << token;
//if( this->isVisible() )
    this->close();
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
            this->close();
            //hide();
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
