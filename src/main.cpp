#include <QApplication>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QSplashScreen>
#include "mainwindow.h"
#include <unistd.h>
#include <QTimer>


int main(int argc, char *argv[])
{
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
    QTimer::singleShot(1000, &splash, &QWidget::close); // keep displayed for 5 seconds

    MainWindow window;
    window.init();

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
