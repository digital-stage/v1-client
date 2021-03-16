#include "app.h"
#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>
#include <iostream>

App::App()
{
  QApplication::setQuitOnLastWindowClosed(false);

  ovController = new OvController();
  loginDialog = new LoginDialog();
  auth = new ds::ds_auth_service_t(AUTH_SERVER);
  keyStore = new KeyStore();

  // Init tray
  trayIcon = new QSystemTrayIcon(this);

  QIcon icon = QIcon(":/images/icon.png");
  icon.setIsMask(true);

  trayIcon->setIcon(icon);
  trayIcon->setToolTip(tr("Digital Stage"));

  QAction* quitAction = new QAction(tr("Close"), this);
  connect(quitAction, &QAction::triggered, qApp, &QApplication::exit);

  // Login menu
  loginMenu = new QMenu();
  QAction* viewLoginAction = new QAction(tr("Login"), this);
  loginMenu->addAction(viewLoginAction);
  connect(viewLoginAction, &QAction::triggered, loginDialog,
          &LoginDialog::show);
  loginMenu->addAction(quitAction);

  // Digital stage status menu
  dsStatusMenu = new QMenu();
  QAction* openDigitalStageFrontendAction = new QAction(tr("Open stage"), this);
  dsStatusMenu->addAction(openDigitalStageFrontendAction);
  connect(openDigitalStageFrontendAction, &QAction::triggered, this,
          &App::openDigitalStageFrontend);
  QAction* logoutAction = new QAction(tr("Logout"), this);
  dsStatusMenu->addAction(logoutAction);
  connect(logoutAction, &QAction::triggered, this, &App::signOut);
  dsStatusMenu->addSeparator();
  dsStatusMenu->addAction(quitAction);

  // Orlando viols status menu
  ovStatusMenu = new QMenu();
  QAction* openOrlandoViolsFrontendAction = new QAction(tr("Open stage"), this);
  ovStatusMenu->addAction(openOrlandoViolsFrontendAction);
  connect(openOrlandoViolsFrontendAction, &QAction::triggered, this,
          &App::openOrlandoViolsFrontend);
  QAction* switchAction = new QAction(tr("Switch to digital stage"), this);
  connect(switchAction, &QAction::triggered, ovController, [=]() {
    QString frontend("ds");
    ovController->start(frontend);
    switchFrontend(frontend);
  });
  ovStatusMenu->addAction(switchAction);
  ovStatusMenu->addSeparator();
  ovStatusMenu->addAction(quitAction);

  // Class Actions
  connect(trayIcon, &QSystemTrayIcon::activated, this, &App::iconActivated);
  connect(loginDialog, &LoginDialog::logIn, this, &App::signIn);
  connect(loginDialog, &LoginDialog::switchToOrlandoViols, this, [=]() {
    QString frontend("ov");
    ovController->start(frontend);
    switchFrontend(frontend);
  });
  connect(qApp, &QApplication::aboutToQuit, this, &App::onExit);
  connect(ovController, &OvController::started, this, &App::onStarted);
  connect(ovController, &OvController::stopped, this, &App::onStopped);
  connect(ovController, &OvController::errored, this, &App::onError);
}

void App::onExit()
{
  std::cout << "onExit" << std::endl;
  this->ovController->stop();
}

void App::show()
{
  trayIcon->setContextMenu(loginMenu);
  trayIcon->show();

  const auto frontend = keyStore->restoreFrontendSelection();
  std::cout << "Using frontend " << frontend.toStdString() << std::endl;
  switchFrontend(frontend);
}

void App::autoSignIn() {
  // Try to sign in by restoring the credentials
  loginDialog->resetError();
  email = keyStore->restoreEmail();
  if( !email.isEmpty() ) {
    this->loginDialog->setEmail(email);
    KeyStore::Credentials* credentials = keyStore->restore(email);
    if(credentials != nullptr) {
      this->loginDialog->setPassword(credentials->password);
      const string& retrievedToken =
          auth->signInSync(credentials->email.toStdString(),
                           credentials->password.toStdString());
      if(!retrievedToken.empty()) {
        token = QString::fromStdString(retrievedToken);
        this->switchFrontend("ds");
      }
    }
  }
}

void App::signIn(const QString& emailValue, const QString& password)
{
  loginDialog->resetError();
  const string& retrievedToken =
      auth->signInSync(emailValue.toStdString(), password.toStdString());
  if(!retrievedToken.empty()) {
    email = emailValue;
    keyStore->storeEmail(email);
    token = QString::fromStdString(retrievedToken);
    ovController->setToken(token);
    keyStore->store({email, password});
    this->switchFrontend("ds");
    return;
  }
  loginDialog->setError(tr("Unknown email or wrong password"));
}

void App::signOut()
{
  stop();
  auth->signOut(token.toStdString());
  keyStore->remove(email);
  trayIcon->setContextMenu(loginMenu);
  loginDialog->resetError();
  loginDialog->setPassword("");
  loginDialog->show();
}

void App::openDigitalStageFrontend()
{
  QDesktopServices::openUrl(QUrl(STAGE_URL));
}

void App::openOrlandoViolsFrontend()
{
  QDesktopServices::openUrl(QUrl(OV_FRONTEND_URL));
}

void App::start()
{
  std::cout << "App::start" << std::endl;
  ovController->start(frontend);
}

void App::stop()
{
  std::cout << "App::stop" << std::endl;
  ovController->stop();
}


void App::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
  switch(reason) {
  case QSystemTrayIcon::DoubleClick:
    if(token.isEmpty() && !loginDialog->isVisible())
      loginDialog->show();
    break;
  default:;
  }
}
void App::switchFrontend(const QString& value)
{
  frontend = value;
  this->keyStore->storeFrontendSelection(frontend);
  if(frontend == "ov") {
    // Since no login is necessary directly start client
    this->start();
  } else {
    if(!token.isEmpty()) {
      ovController->setToken(token);
      this->start();
    } else {
      this->autoSignIn();
    }
  }
}
App::~App() = default;

void App::onError(const QString& error)
{
  QMessageBox msgBox;
  msgBox.setText(error);
  msgBox.exec();
}
void App::onStarted(const QString& value)
{
  frontend = value;
  loginDialog->hide();
  if(value == "ds") {
    trayIcon->setContextMenu(dsStatusMenu);
  } else {
    trayIcon->setContextMenu(ovStatusMenu);
  }
}
void App::onStopped()
{
  trayIcon->setContextMenu(loginMenu);
  loginDialog->show();
}
