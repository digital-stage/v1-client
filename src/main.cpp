#include <QApplication>
#include "app.h"

#ifndef QT_NO_SYSTEMTRAYICON

#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QSplashScreen>
#include <unistd.h>
#include <QTimer>
#include <QTranslator>


int main(int argc, char *argv[])
{
    QApplication qApplication(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system."));
        return 1;
    }

    QTranslator translator;
    if (translator.load(QLocale(), QLatin1String("DigitalStage"), QLatin1String("_"), QLatin1String(":/i18n"))) {
        QCoreApplication::installTranslator(&translator);
    }

    QPixmap pixmap(":/images/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    QTimer::singleShot(1000, &splash, &QWidget::close); // keep displayed for 5 seconds

    App app;
    app.show();

    return qApplication.exec();
}

#else

#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString text("QSystemTrayIcon is not supported on this platform");

    QLabel *label = new QLabel(text);
    label->setWordWrap(true);

    label->show();

    app.exec();
}

#endif
