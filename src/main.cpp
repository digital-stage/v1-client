#include <QApplication>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QSplashScreen>
#include "mainwindow.h"
#include <unistd.h>
#include <future>
#include <chrono>
#include <QDebug>

bool sign_in (QSplashScreen *splash, QWidget *w) {
    qDebug() << "Loading...";
    //TODO: Load login data from keyvault and token, if available
    //TODO: If there is a token: verify it first
    //TODO: Otherwise get new token with existing login data
    //TODO: or show login
    std::this_thread::sleep_for(std::chrono::milliseconds(2000) );

    splash->hide();

    qDebug() << "Closed splash screen";
  return true;
}


int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(systray);
    //Q_INIT_RESOURCE(resources);

    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);

    QPixmap pixmap(":/images/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("Now loading...");

    MainWindow window;

    std::future<bool> fut = std::async (sign_in, &splash, &window);

    window.show();

    return app.exec();
}

#else

#include <QLabel>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString text("QSystemTrayIcon is not supported on this platform");

    QLabel *label = new QLabel(text);
    label->setWordWrap(true);

    label->show();
    qDebug() << text;

    app.exec();
}

#endif
