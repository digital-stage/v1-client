#ifndef APP_H
#define APP_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDialog>
#include "logindialog.h"
#include "api/auth.h"
#include "api/keystore.h"

#ifndef STAGE_URL
#define STAGE_URL "https://live.digital-stage.org"
#endif

#ifndef AUTH_SERVER
#define AUTH_SERVER "https://auth.digital-stage.org"
#endif

#ifndef API_SERVER
#define API_SERVER "https://api.digital-stage.org"
#endif

class App : public QObject
{
    Q_OBJECT

public:
    App();
    ~App();
    void show();
    void close();

private slots:
    void onSignIn(const QString email, const QString password);
    void onSignOut();
    void openStage();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

protected:
    void init();
    void start();
    void stop();

private:
    const QString restoreEmail();
    void storeEmail(const QString email);

    bool isInitialized_;

    QSystemTrayIcon *trayIcon_;
    QMenu *loginMenu_;
    QMenu *statusMenu_;

    LoginDialog *loginDialog_;

    Auth *auth_;
    KeyStore *keyStore_;

    QString email_;
    QString token_;
};

#endif // APP_H
