#include "app.h"
#include <QApplication>
#include <QAction>
#include <QSettings>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <ov_render_tascar.h>
#include <ov_client_digitalstage.h>

App::App() {
    QApplication::setQuitOnLastWindowClosed(false);

    // Init tray
    this->trayIcon_ = new QSystemTrayIcon(this);

    QIcon icon = QIcon(":/images/icon.png");
    icon.setIsMask(true);

    this->trayIcon_->setIcon(icon);
    this->trayIcon_->setToolTip(tr("Digital Stage"));

    QAction *quitAction = new QAction(tr("Close"), this);

    // Login menu
    this->loginMenu_ = new QMenu();
    QAction *viewLoginAction = new QAction(tr("Login"), this);
    this->loginMenu_->addAction(viewLoginAction);
    this->loginMenu_->addAction(quitAction);

    // Status menu
    this->statusMenu_ = new QMenu();
    QAction *openStageAction = new QAction(tr("Open stage"), this);
    this->statusMenu_->addAction(openStageAction);
    QAction *logoutAction = new QAction(tr("Logout"), this);
    this->statusMenu_->addAction(logoutAction);
    this->statusMenu_->addSeparator();
    this->statusMenu_->addAction(quitAction);

    // Login dialog
    this->loginDialog_ = new LoginDialog();

    // Helper
    //this->auth_ = new Auth(AUTH_SERVER);
    this->auth_ = new ds::ds_auth_service_t(AUTH_SERVER);
    this->keyStore_ = new KeyStore();

    // Actions
    this->connect(this->trayIcon_, &QSystemTrayIcon::activated, this, &App::iconActivated);
    this->connect(this->loginDialog_, &LoginDialog::logIn, this, &App::onSignIn);
    this->connect(viewLoginAction, &QAction::triggered, this->loginDialog_, &LoginDialog::show);
    this->connect(quitAction, &QAction::triggered, qApp, &QApplication::exit);
    this->connect(logoutAction, &QAction::triggered, this, &App::onSignOut);
    this->connect(openStageAction, &QAction::triggered, this, &App::openStage);

    //this->connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(exit()));

    const std::string device_id(getmacaddr());
    const int pinglogport(0);
    ov_render_tascar_t backend(device_id, pinglogport);
    std::cout << "HAVE MAC: " << backend.get_deviceid() << std::endl;
    this->service_ = new ds::ds_service_t(backend, API_SERVER);

    this->isInitialized_ = false;

    this->trayIcon_->show();
}

App::~App() {
    if (this->service_)
        this->service_->stop();
    delete this->service_;
    delete this->trayIcon_;
    delete this->loginMenu_;
    delete this->statusMenu_;
    delete this->auth_;
    delete this->keyStore_;
    delete this->loginDialog_;
}


void App::init() {
    this->email_ = restoreEmail();
    this->loginDialog_->setEmail(email_);
    if (!this->email_.isEmpty()) {
        KeyStore::Credentials *credentials = this->keyStore_->restore(this->email_);
        if (credentials != NULL) {
            const string &retrievedToken = this->auth_->signIn(credentials->email.toStdString(),
                                                               credentials->password.toStdString());
            if (!retrievedToken.empty()) {
                this->token_ = QString::fromStdString(retrievedToken);
                if (this->loginDialog_->isVisible())
                    this->loginDialog_->hide();
                this->trayIcon_->setContextMenu(statusMenu_);
                this->start();
                return;
            }
            this->loginDialog_->setPassword(credentials->password);
        }
    } else {
        this->loginDialog_->setPassword("");
    }
    this->trayIcon_->setContextMenu(loginMenu_);
    this->loginDialog_->show();
}

void App::show() {
    if (!this->isInitialized_) {
        isInitialized_ = true;
        this->init();
    }
}

void App::onSignIn(const QString email, const QString password) {
    this->loginDialog_->resetError();
    const string &retrievedToken = this->auth_->signIn(email.toStdString(), password.toStdString());
    if (!retrievedToken.empty()) {
        this->email_ = email;
        this->token_ = QString::fromStdString(retrievedToken);
        this->keyStore_->store({email, password});
        this->storeEmail(email);
        if (this->loginDialog_->isVisible())
            this->loginDialog_->hide();
        this->trayIcon_->setContextMenu(this->statusMenu_);
        this->start();
        return;
    }
    this->loginDialog_->setError(tr("Unknown email or wrong password"));
    this->trayIcon_->setContextMenu(loginMenu_);
    if (!this->loginDialog_->isVisible())
        this->loginDialog_->show();
}

void App::onSignOut() {
    this->stop();
    this->auth_->signOut(this->token_.toStdString());
    this->keyStore_->remove(this->email_);
    this->trayIcon_->setContextMenu(loginMenu_);
    this->loginDialog_->resetError();
    this->loginDialog_->setPassword("");
    this->loginDialog_->show();
}

void App::openStage() {
    QDesktopServices::openUrl(QUrl(STAGE_URL));
}

void App::start() {
    this->service_->start(this->token_.toStdString());
}

void App::stop() {
    if (this->service_)
        this->service_->stop();
}


QString App::restoreEmail() {
    QSettings settings("org.digital-stage", "Client");
    const QString email = settings.value("email", "").toString();
    return email;
}


void App::storeEmail(const QString &email) {
    QSettings settings("org.digital-stage", "Client");
    settings.setValue("email", email);
}

void App::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
        case QSystemTrayIcon::DoubleClick:
            if (this->token_.isEmpty() && !this->loginDialog_->isVisible())
                this->loginDialog_->show();
            break;
        default:;
    }
}
