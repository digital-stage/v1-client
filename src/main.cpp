#include "app.h"
#include <QApplication>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QMessageBox>
#include <QSplashScreen>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QTranslator>

#if defined(__APPLE__) || defined(__linux__)
void catchUnixSignals(std::initializer_list<int> quitSignals)
{
  auto handler = [](int sig) -> void {
    // blocking and non aysnc-signal-safe func are valid
    printf("\nquit the application by signal(%d).\n", sig);
    QCoreApplication::quit();
  };

  sigset_t blocking_mask;
  sigemptyset(&blocking_mask);
  for(auto sig : quitSignals)
    sigaddset(&blocking_mask, sig);

  struct sigaction sa;
  sa.sa_handler = handler;
  sa.sa_mask = blocking_mask;
  sa.sa_flags = 0;

  for(auto sig : quitSignals)
    sigaction(sig, &sa, nullptr);
}
#endif

int main(int argc, char* argv[])
{
#if defined(__APPLE__) || defined(__linux__)
  catchUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP});
#endif

  QApplication qApplication(argc, argv);

  if(!QSystemTrayIcon::isSystemTrayAvailable()) {
    QMessageBox::critical(nullptr, QObject::tr("Systray"),
                          QObject::tr("I couldn't detect any system tray "
                                      "on this system."));
    return 1;
  }

  QTranslator translator;
  if(translator.load(QLocale(), QLatin1String("DigitalStage"),
                     QLatin1String("_"), QLatin1String(":/i18n"))) {
    QCoreApplication::installTranslator(&translator);
  }

  QPixmap pixmap(":/images/splash.png");
  QSplashScreen splash(pixmap);
  splash.show();
  QTimer::singleShot(1000, &splash,
                     &QWidget::close); // keep displayed for 5 seconds

  App app;
  app.show();

  return qApplication.exec();
}

#else

#include <QLabel>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  QString text("QSystemTrayIcon is not supported on this platform");

  QLabel* label = new QLabel(text);
  label->setWordWrap(true);

  label->show();

  app.exec();
}

#endif
