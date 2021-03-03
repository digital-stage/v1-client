#ifndef APP_H
#define APP_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDialog>
#include "logindialog.h"
#include "api/auth.h"
#include "api/keystore.h"
#include <ds_service.h>
#include <ds_auth_service.h>

#ifndef STAGE_URL
#define STAGE_URL "https://live.digital-stage.org"
#endif

#ifndef AUTH_SERVER
#define AUTH_SERVER "https://auth.digital-stage.org"
#endif

#ifndef API_SERVER
#define API_SERVER "wss://api.digital-stage.org"
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
    QString restoreEmail();
    void storeEmail(const QString& email);
    void service();

    bool isRunning_;

    bool isInitialized_;

    QSystemTrayIcon *trayIcon_;
    QMenu *loginMenu_;
    QMenu *statusMenu_;

    LoginDialog *loginDialog_;

    ds::ds_auth_service_t *auth_;
    //Auth *auth_;
    KeyStore *keyStore_;

    QString email_;
    QString token_;

    std::thread servicethread_;
    ds::ds_service_t *service_;
};

#endif // APP_H
