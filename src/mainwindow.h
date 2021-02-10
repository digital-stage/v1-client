#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <src/api/auth.h>
#include <src/api/keystore.h>
#include "loginpane.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();

protected:
    void closeEvent(QCloseEvent * event) override;
    const QString loadEmail();
    void saveEmail(const QString email);
    void showLogin(QString initialEmail, QString initialPassword);
    void showStatus();

public slots:
    void onLogIn(const QString email, const QString password);
    void onLogOut();

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void exit();

private:
    QSystemTrayIcon *trayIcon;
    Ui::MainWindow *ui;

    LoginPane *loginPane;
    QMenu *loginMenu;

    QMenu *statusMenu;

    QString settingsFile;
    Auth *auth;
    KeyStore *keyStore;

    bool trayWarning = true;
};
#endif // MAINWINDOW_H
