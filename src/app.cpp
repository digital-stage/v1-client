#include "app.h"
#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QSettings>
#include <QUrl>
#include <ov_client_digitalstage.h>
#include <ov_client_orlandoviols.h>
#include <ov_render_tascar.h>

App::App()
{
  QApplication::setQuitOnLastWindowClosed(false);

  // Init tray
  this->trayIcon_ = new QSystemTrayIcon(this);

  QIcon icon = QIcon(":/images/icon.png");
  icon.setIsMask(true);

  this->trayIcon_->setIcon(icon);
  this->trayIcon_->setToolTip(tr("Digital Stage"));

  QAction* quitAction = new QAction(tr("Close"), this);

  // Login menu
  this->loginMenu_ = new QMenu();
  QAction* viewLoginAction = new QAction(tr("Login"), this);
  this->loginMenu_->addAction(viewLoginAction);
  this->loginMenu_->addAction(quitAction);

  // Status menu
  this->statusMenu_ = new QMenu();
  QAction* openStageAction = new QAction(tr("Open stage"), this);
  this->statusMenu_->addAction(openStageAction);
  QAction* logoutAction = new QAction(tr("Logout"), this);
  this->statusMenu_->addAction(logoutAction);
  this->statusMenu_->addSeparator();
  this->statusMenu_->addAction(quitAction);
  this->ovFrontendMenu_ = new QMenu();
  this->ovFrontendMenu_->addAction(openStageAction);
  QAction* switchAction = new QAction(tr("Switch to digital stage"), this);
  this->ovFrontendMenu_->addAction(switchAction);
  this->ovFrontendMenu_->addSeparator();
  this->ovFrontendMenu_->addAction(quitAction);

  // Login dialog
  this->loginDialog_ = new LoginDialog();

  // Helper
  // this->auth_ = new Auth(AUTH_SERVER);
  this->auth_ = new ds::ds_auth_service_t(AUTH_SERVER);
  this->keyStore_ = new KeyStore();

  // Actions
  this->connect(this->trayIcon_, &QSystemTrayIcon::activated, this,
                &App::iconActivated);
  this->connect(this->loginDialog_, &LoginDialog::logIn, this, &App::onSignIn);
  this->connect(this->loginDialog_, &LoginDialog::switchFrontend, this,
                &App::switchFrontend);
  this->connect(viewLoginAction, &QAction::triggered, this->loginDialog_,
                &LoginDialog::show);
  this->connect(quitAction, &QAction::triggered, qApp, &QApplication::exit);
  this->connect(logoutAction, &QAction::triggered, this, &App::onSignOut);
  this->connect(openStageAction, &QAction::triggered, this, &App::openStage);
  this->connect(switchAction, &QAction::triggered, this, &App::switchFrontend);

  this->connect(qApp, &QApplication::aboutToQuit, this, &App::onExit);

  this->isInitialized_ = false;

  this->trayIcon_->show();
}

void App::onExit()
{
  std::cout << "onExit" << std::endl;
  if(this->isRunning_)
    this->stop();
  /**/
}

void App::init()
{
  this->useOrlandoViolsFrontend = restoreFrontendSelection();
  if(this->useOrlandoViolsFrontend) {
    this->trayIcon_->setContextMenu(ovFrontendMenu_);
  } else {
    this->email_ = restoreEmail();
    this->loginDialog_->setEmail(email_);
    if(!this->email_.isEmpty()) {
      KeyStore::Credentials* credentials =
          this->keyStore_->restore(this->email_);
      if(credentials != nullptr) {
        const string& retrievedToken =
            this->auth_->signIn(credentials->email.toStdString(),
                                credentials->password.toStdString());
        if(!retrievedToken.empty()) {
          this->token_ = QString::fromStdString(retrievedToken);
          if(this->loginDialog_->isVisible())
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
}

void App::show()
{
  if(!this->isInitialized_) {
    isInitialized_ = true;
    this->init();
  }
}

void App::onSignIn(const QString email, const QString password)
{
  this->loginDialog_->resetError();
  const string& retrievedToken =
      this->auth_->signIn(email.toStdString(), password.toStdString());
  if(!retrievedToken.empty()) {
    this->email_ = email;
    this->token_ = QString::fromStdString(retrievedToken);
    this->keyStore_->store({email, password});
    this->storeEmail(email);
    if(this->loginDialog_->isVisible())
      this->loginDialog_->hide();
    this->trayIcon_->setContextMenu(this->statusMenu_);
    this->start();
    return;
  }
  this->loginDialog_->setError(tr("Unknown email or wrong password"));
  this->trayIcon_->setContextMenu(loginMenu_);
  if(!this->loginDialog_->isVisible())
    this->loginDialog_->show();
}

void App::onSignOut()
{
  this->stop();
  this->auth_->signOut(this->token_.toStdString());
  this->keyStore_->remove(this->email_);
  this->trayIcon_->setContextMenu(loginMenu_);
  this->loginDialog_->resetError();
  this->loginDialog_->setPassword("");
  this->loginDialog_->show();
}

void App::openStage()
{
  QDesktopServices::openUrl(
      QUrl(this->useOrlandoViolsFrontend ? OV_FRONTEND_URL : STAGE_URL));
}

void App::start()
{
  this->isRunning_ = true;
  this->servicethread_ = std::thread(&App::service, this);
}

void App::stop()
{
  this->isRunning_ = false;
  this->servicethread_.join();
}

void App::service()
{
  const std::string device_id(getmacaddr());
  const int pinglogport(0);
  ov_render_tascar_t backend(device_id, pinglogport);
  if(this->useOrlandoViolsFrontend) {
    std::string lobby(ovstrrep("http://oldbox.orlandoviols.com/", "\\/", "/"));
    std::cout << "Starting OV client ..." << std::endl;
    this->service_ = new ov_client_orlandoviols_t(backend, lobby);
  } else {
    auto* service = new ds::ds_service_t(backend, API_SERVER);
    service->set_token(this->token_.toStdString());
    std::cout << "Starting DS client ..." << std::endl;
    this->service_ = service;
  }
  this->service_->start_service();
  while(this->isRunning_) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if(this->service_->is_going_to_stop()) {
      std::cout << "is going to stop" << std::endl;
      this->isRunning_ = false;
    }
  }
  std::cout << "Shut down service" << std::endl;
  this->service_->stop_service();
  std::cout << "DONE" << std::endl;
}

bool App::restoreFrontendSelection()
{
  QSettings settings("org.digital-stage", "Client");
  return settings.value("useOvFrontend", false).toBool();
}

QString App::restoreEmail()
{
  QSettings settings("org.digital-stage", "Client");
  const QString email = settings.value("email", "").toString();
  return email;
}

void App::storeEmail(const QString& email)
{
  QSettings settings("org.digital-stage", "Client");
  settings.setValue("email", email);
}

void App::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
  switch(reason) {
  case QSystemTrayIcon::DoubleClick:
    if(this->token_.isEmpty() && !this->loginDialog_->isVisible())
      this->loginDialog_->show();
    break;
  default:;
  }
}
void App::switchFrontend(bool shallUseOrlandoViolsFrontend)
{
  if(this->useOrlandoViolsFrontend != shallUseOrlandoViolsFrontend) {
    this->useOrlandoViolsFrontend = shallUseOrlandoViolsFrontend;
    if(this->useOrlandoViolsFrontend) {
      // Switch to orlando viols
      this->trayIcon_->setContextMenu(ovFrontendMenu_);
      if(this->loginDialog_->isVisible())
        this->loginDialog_->hide();
      if(this->isRunning_)
        this->stop();
      this->start();
    } else {
      if(this->isRunning_) {
        this->stop();
      }
      this->trayIcon_->setContextMenu(loginMenu_);
      this->loginDialog_->show();
    }
  }
}
App::~App() {}
