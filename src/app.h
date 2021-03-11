#ifndef APP_H
#define APP_H

#include "api/auth.h"
#include "api/keystore.h"
#include "libov/src/ds_auth_service.h"
#include "logindialog.h"
#include "ov/ov_controller.h"
#include <QDialog>
#include <QMenu>
#include <QString>
#include <QSystemTrayIcon>
#include <QWidget>

#ifndef OV_FRONTEND_URL
#define OV_FRONTEND_URL "https://box.orlandoviols.com"
#endif

#ifndef STAGE_URL
#define STAGE_URL "https://live.digital-stage.org"
#endif

#ifndef AUTH_SERVER
#define AUTH_SERVER "https://auth.digital-stage.org"
#endif

#ifndef API_SERVER
#define API_SERVER "wss://api.digital-stage.org"
#endif

class App : public QObject {
  Q_OBJECT

public:
  App();
  ~App();
  void show();

private slots:
  void autoSignIn();
  void signIn(const QString& email, const QString& password);
  void signOut();
  void onExit();
  void openDigitalStageFrontend();
  void openOrlandoViolsFrontend();
  void iconActivated(QSystemTrayIcon::ActivationReason reason);
  void switchFrontend(const QString& frontend);
  void onStarted(const QString& frontend);
  void onStopped();
  void onError(const QString& error);

protected:
  void start();
  void stop();

private:
  bool isInitialized;

  QSystemTrayIcon* trayIcon;
  QMenu* loginMenu;
  QMenu* dsStatusMenu;
  QMenu* ovStatusMenu;

  LoginDialog* loginDialog;

  ds::ds_auth_service_t* auth;
  // Auth *auth_;
  KeyStore* keyStore;

  QString email;  // we need this redundancy for signing out
  QString token;

  OvController* ovController;
};

#endif // APP_H
